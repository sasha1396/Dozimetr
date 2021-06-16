
#ifndef PWM_H_
#define PWM_H_

#include <inttypes.h>

#define F_CPU 8000000UL

void init_pwm(void);
void sound_generator(char n);
void SwitchPwmFan(char value);
void SwitchPwmSolder(char value);
void SwitchPwmDryer(char value);

#endif