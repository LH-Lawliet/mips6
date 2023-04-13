#ifndef _RCC_H_
#define _RCC_H_

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct _Clock_t {
	uint32_t	ahb_freq;
	uint32_t	apb1_freq;
	uint32_t	apb1_timer_freq;
	uint32_t	apb2_freq;
	uint32_t	apb2_timer_freq;
} Clock_t;

enum {
	CLOCK_CONFIG_HSE_8MHz=0,
	CLOCK_CONFIG_HSE_48MHz,
	CLOCK_CONFIG_HSE_84MHz,
	CLOCK_CONFIG_HSE_96MHz,
	CLOCK_CONFIG_HSI_16MHz,
	CLOCK_CONFIG_HSI_48MHz,
	CLOCK_CONFIG_HSI_84MHz,
	CLOCK_CONFIG_HSI_96MHz,
	CLOCK_CONFIG_END
};

void rcc_config_clock(uint32_t config, Clock_t *sysclks);

#
#ifdef __cplusplus
}
#endif

#endif
