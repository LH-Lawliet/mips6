#include "adc.h"
#include "io.h"
/* ADC
 * 
 * 1. Initialize the ADC for regular channels only
 *
 *    adc_init(adc, mode, cb)
 *    
 *    adc can be configured in
 *    - single scan mode: only 1 scan is triggered
 *    - continuous scan mode: inputs are continuously resampled
 *    - triggered scan mode: the sampling is triggered by an external source
 *      (a timer). Extra information on the triggering source is given by
 *      adc_set_trigger_event.
 *
 *    The ADC resolution can be configured as well: 12, 10, 8 or 6 bits.
 *
 *
 * 2. Add channels to the scan loop: each time the sampling is triggered, all
 *    the channels enabled are sampled according to steadying time configured.
 *    
 *    adc_channel_enable(adc, channel, ADC_SMPL_480CYCLES);
 *
 *    Channel configuration
 *    - each call add the given channel to the scan loop. Up to 16 channels
 *      can be added among the 18 available channels: 0 to 15 for regular 
 *      channels, 16 for internal temperature sensor channel, 17 for the
 *      ADC reference channel and 18 for the Battery monitoring channel.
 *    - a channel can be added several times.
 *
 *
 * 3. In triggered mode, set the triggering source
 *
 *    adc_set_trigger_evt(adc, triggering_event)
 *
 *    Only one triggering event can be used at a time.
 *
 *
 * 4. Alternatively, when in single or continuous mode, the ADC sampling has
 *    to be started manuallly
 *
 *    adc_start(adc)
 *
 *    In continuous mode, the ADC has to be stopped manually.
 *
 *    adc_stop(adc)
 *
 *
 * Helper functions:
 *
 *    adc_get_sampling_freq(adc)
 *
 *      returns the sampling frequency by channel. The more the channels in 
 *      the scan loop, the less the sampling frequency is high. The frequency
 *      calculation is relevant for continuous and timer triggered modes only.
 *
 */

#define ADCCLK_MAX		30000000UL

static OnSample callback=NULL;
static uint32_t channel_index=0;		/* next channel index in SQR[] to sample */
static uint32_t n_to_convert=0;			/* how many channels to sample in each scan sequence */
static uint32_t adc_freq;				/* actual ADC frequency */
static uint32_t adc_sampling_freq;		/* estimation of the sampling frequency */
static TIM_t *trig_timer;
static GPIO_t *trig_pin;

void ADC_IRQHandler()
{
	uint32_t sr = _ADC1->SR;
	sr = _ADC1->SR;
	
	if (sr & (1<<1)) {					// End Of Conversion
		// get the current channel sampled according to SQR[]
		uint32_t index = 2-(channel_index/6);
		uint32_t shift = (channel_index % 6)*5;
		uint32_t cur_channel = (_ADC1->SQR[index] >> shift) & 0x1F;

		// call the callback function
		if (callback) callback(cur_channel, _ADC1->DR);
		
		// prepare for the next channel to sample
		if (n_to_convert>1) {
			channel_index = (channel_index+1) % n_to_convert;
		} else {
			channel_index = 0;
		}
		
	} else if (sr & (1<<5)) {			// OVerRun
		ADC1->SR &= ~(1<<5);
	} 
}

/*
 *                     poll     irq      irq     irq    dma      dma     dma
 *                     single   single   cont    trig   single   cont    trig
 * CR2[1]  (cont)        0        0       1       0      0        1       0
 * CR1[11] (discen)      0        0       0       1      0        0       1
 * callback              n        y       y       y      ?        ?       ?
 * CR1[5]  (EOCIE)       0        1       1       1      ?        ?       ?
 * CR2[8]  (dma)         0        0       0       0      1        1       1
 *
 * no cb --> poll single
 * 
 */
int adc_init(ADC_t *adc, uint32_t mode, OnSample cb)
{
	IRQn_t	irq_number;
	uint32_t irq_priority;
	
	if (adc == _ADC1) {
#ifdef USE_ADC1
		_RCC->APB2ENR |= 1<<8;
		
		// configure pins
		io_configure(ADC1_GPIO_PORT, ADC1_GPIO_PINS, PIN_MODE_ANALOG, NULL);
		
		irq_number = ADC_IRQn;
		irq_priority = ADC1_IRQ_PRIORITY;
#else
		return -1;
#endif
	} else {
		return -1;
	}
	
	for (int presc_cfg=0; presc_cfg<=3; presc_cfg++) {
		// set up ADCCLK prescaler config (2,4,6 or 8) so that Fadcclk < 30MHz
		if (sysclks.apb2_freq / (2*(presc_cfg+1)) < ADCCLK_MAX) {
			ADC->CCR = presc_cfg<<16;
			
			// Resolution (xx << 24)
			//  00: 12-bit (3 + 12 ADCCLK cycles min)
			//  01: 10-bit (3 + 10 ADCCLK cycles min)
			//  10: 8-bit  (3 + 8 ADCCLK cycles min)
			//  11: 6-bit  (3 + 6 ADCCLK cycles min)
			
			// Scan mode (1<<8) : convert inputs selected in ADC_SQRx or ADC_JSQRx
			adc->CR1 = 1<<8 | (mode & (3<<24));
			if ((mode & 3) == 2) adc->CR1 |= 1<<11;		// triggered mode
			
			// EOCS (1<<10) : bit set (and interrupt requested) after each
			//                channel conversion
			// CONT (1<<1)  : continuous conversion
			// ADON (1<<0)  : ADC on
			adc->CR2 = 1<<10 | 1;
			if ((mode & 3) == 1) adc->CR2 |= 1<<1;		// continuous mode
			
			
			/* A COMPLETER */
			if (cb) {
				adc->CR1 |= ADC_CR1_EOCIE;
				NVIC_SetPriority(irq_number, irq_priority);
				NVIC_EnableIRQ(irq_number);
				callback = cb;
			} 
		
            adc->CR1 = (adc->CR1&(~(1<<11)))|((((mode>>1)&1)<<11));

			trig_timer=NULL;
			trig_pin = NULL;
			
			// return ADCCLK
			adc_freq = sysclks.apb2_freq / (2*(presc_cfg+1));
			return (int)adc_freq;
		}
	}
	
	return -1;
}

/* adc_channel_sample
 *   sample the specified channel and generate an interrupt if 
 *   a callback was defined on init, else does the conversion in
 *   polling mode.
 */
uint16_t adc_channel_sample(ADC_t *adc, uint32_t channel)
{
	if (channel>18) return 0;
	
	n_to_convert = 1;
	
	/* special internal channels */
	if (channel == ADC_CHANNEL_TEMP) {
		ADC->CCR = (ADC->CCR & ~(3<<22)) | (2<<22);
	} else if (channel == ADC_CHANNEL_VREFINT) {
		ADC->CCR = (ADC->CCR & ~(3<<22)) | (2<<22);
	} else if (channel == ADC_CHANNEL_VBAT) {
		ADC->CCR = (ADC->CCR & ~(3<<22)) | (1<<22);
	}
	
	adc->SQR[0] = (n_to_convert-1) << 20;
	adc->SQR[1] = 0;
	adc->SQR[2] = channel;
	adc->SMPR[1] = ADC_SMPL_480CYCLES;
	
	adc->CR2 |= 1<<30;
	
	if (!callback) {
    	while (!(adc->SR & ADC_SR_EOC));

    	return (uint32_t)(adc->DR);
	}
	
	return 0;
}


/* adc_channel_enable
 *   add adc channel to the sequence of channels to be sampled
 */
int adc_channel_enable(ADC_t *adc, uint32_t channel, uint32_t in_sample_time)
{
	// we limit the number of channels that can be sampled in a scan loop to 8
	// to handle continuous (16 channels max) and triggered (8 channels max) 
	// modes the same way.
	if (channel>18 || n_to_convert==8) return -1;
	
	if (channel == ADC_CHANNEL_TEMP)  {
		ADC->CCR = (ADC->CCR & ~(3<<22)) | (2<<22);
	} else if (channel == ADC_CHANNEL_VREFINT) {
		ADC->CCR = (ADC->CCR & ~(3<<22)) | (2<<22);
	} else if (channel == ADC_CHANNEL_VBAT) {
		ADC->CCR = (ADC->CCR & ~(3<<22)) | (1<<22);
	}
	
	n_to_convert++;

	// add channel to the current sampling regular sequence	
	/* A COMPLETER */
    //adc_channel_sample(adc, channel); not working
    int32_t sqri = (int32_t)((n_to_convert-1)/6);
    int32_t soffset = ((n_to_convert-1)-(sqri*6))*5;
    adc->SQR[2-sqri] = adc->SQR[2-sqri]&(~(((1<<7)-1)<<soffset))|(channel<<soffset); 
		
	// set the number of channels to scan in the regular group of 
	// conversion sequence: write (n_to_convert-1)
	/* A COMPLETER */
    adc->SQR[0] = (n_to_convert-1)<<20;

	// copy the n_to_convert-1 in CR1[15:13] to handle triggered mode
	/* A COMPLETER */
    adc->CR1 = (adc->CR1 & (~(7<<13))) | ((n_to_convert-1)<<13);
	// set input sample time before conversion starts
	/* A COMPLETER */
    
    /*int32_t mode = (adc->CR1 >> 24)&3;
    int32_t b = 12;
    if (mode==1) { b = 10;}
    else if (mode==2) { b = 8;}
    else if (mode==3) { b = 6;};
    
    (b+in_sample_time)/sysclks.apb2_freq;
    */

    int32_t smpi = 0;
    if (channel>9) {
        smpi = 1;
    };
    int32_t offset = (channel-smpi*10)*3;
    adc->SMPR[1-smpi] = (adc->SMPR[1-smpi] & (~(7<<offset))) | (in_sample_time<<offset);
    return 0;
}

int adc_set_trigger_evt(ADC_t *adc, uint32_t evt)
{
	if (adc->CR1 & (1<<11)) {
		// configure ADC trigger source
		
		/* A COMPLETER */
		adc->CR2 = (adc->CR2&(~(((1<<6)-1)<<24)))|evt;  

		switch (evt) {
		case ADC_TRIG_ON_TIM1_CH1:
			break;
		case ADC_TRIG_ON_TIM1_CH2:
			break;
		case ADC_TRIG_ON_TIM1_CH3:
			break;
		case ADC_TRIG_ON_TIM2_CH2:
			break;
		case ADC_TRIG_ON_TIM2_CH3:
			break;
		case ADC_TRIG_ON_TIM2_CH4:
			break;
		case ADC_TRIG_ON_TIM2_TRGO:
			// connect timer internal update event to TRGO
			
			/* A COMPLETER */
			_TIM2->CR2 = (_TIM2->CR2&(~(7<<4)))|(2<<4);            
            trig_timer = _TIM2;
			break;
		case ADC_TRIG_ON_TIM3_CH1:
			break;
		case ADC_TRIG_ON_TIM3_TRGO:
			// connect timer internal update event to TRGO	
			/* A COMPLETER */    
			_TIM3->CR2 = (_TIM3->CR2&(~(7<<4)))|(2<<4);            
			trig_timer = _TIM3;
			break;
		case ADC_TRIG_ON_TIM4_CH4:
			break;
		case ADC_TRIG_ON_TIM5_CH1:
			break;
		case ADC_TRIG_ON_TIM5_CH2:
			break;
		case ADC_TRIG_ON_TIM5_CH3:
			break;
		case ADC_TRIG_ON_EXTI11_RISING:
			break;
		case ADC_TRIG_ON_EXTI11_FALLING:
			break;
		case ADC_TRIG_ON_EXTI11_BOTH:
			break;
		default:
			break;
		}
		return 0;
	}
	
	return -1;
}

void adc_start(ADC_t *adc)
{
	adc->CR2 |= 1<<30;
}

void adc_stop(ADC_t *adc)
{
	adc->CR2 &= ~(1<<30);
}

uint32_t adc_get_sampling_freq(ADC_t *adc)
{
	if ((adc->CR1 & (1<<11)) && trig_timer) {	// triggered mode
		adc_sampling_freq= sysclks.apb1_timer_freq/((trig_timer->PSC + 1)*(trig_timer->ARR + 1));
	} else {									// continuous mode
		uint32_t ncycles = 0;
		uint32_t index, shift, cfg;
		
		for (uint32_t i=0; i<n_to_convert; i++) {
			index = 1 -i/10;
			shift = (i % 10)*3;
			cfg = (adc->SMPR[index] >> shift) & 0x7;
			
			switch (cfg) {
			case ADC_SMPL_3CYCLES:
				ncycles += 3;
				break;
			case ADC_SMPL_15CYCLES:
				ncycles += 15;
				break;
			case ADC_SMPL_28CYCLES:
				ncycles += 28;
				break;
			case ADC_SMPL_56CYCLES:
				ncycles += 56;
				break;
			case ADC_SMPL_84CYCLES:
				ncycles += 84;
				break;
			case ADC_SMPL_112CYCLES:
				ncycles += 112;
				break;
			case ADC_SMPL_144CYCLES:
				ncycles += 144;
				break;
			case ADC_SMPL_480CYCLES:
				ncycles += 480;
				break;
			}
		}
		
		cfg = (adc->CR1 >> 24) & 0x3;
		switch (cfg) {
		case 0:			/* 12 bit ADC config */
			ncycles += n_to_convert*12;
			break;
		case 1:			/* 10 bit ADC config */
			ncycles += n_to_convert*10;
			break;
		case 2:			/* 8 bit ADC config */
			ncycles += n_to_convert*8;
			break;
		case 3:			/* 6 bit ADC config */
			ncycles += n_to_convert*6;
			break;
		}
		
		adc_sampling_freq = adc_freq / ncycles;
	}
	
	return adc_sampling_freq;
}
