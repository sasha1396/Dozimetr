//***************************************************************************//
#ifndef ADC_H_
#define ADC_H_

#define F_CPU 8000000UL

#include <inttypes.h>

void init_adc(void);
unsigned int adc_device(unsigned char ADC_channel);

#endif