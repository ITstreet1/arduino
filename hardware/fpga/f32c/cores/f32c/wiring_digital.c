/*
 * ULX2S board GPIO & hardwired pins
 */

#include "Arduino.h"
#include "wiring_analog.h" // this is needed to turn off pwm
#include <io.h>

__BEGIN_DECLS

void
pinMode(uint32_t pin, uint32_t mode)
{
	volatile uint32_t *port = (volatile uint32_t *) IO_GPIO_CTL;

	if (pin >= variant_pin_map_size ||
	    digitalPinToPort(pin) != (volatile uint32_t *)IO_GPIO_DATA)
		return;

	switch (mode) {
	case INPUT:
		*port &= ~(1<<variant_pin_map[pin].bit_pos);
		break;
	case OUTPUT:
		*port |=  (1<<variant_pin_map[pin].bit_pos);
		break;
	}
}


void
digitalWrite(uint32_t pin, uint32_t val)
{
	volatile uint32_t *port;
	int8_t pwm_channel;

	if (pin >= variant_pin_map_size)
		return;
		
	// if port has PWM channel, turn PWM off
	pwm_channel = variant_pin_map[pin].pwm;
	if(pwm_channel >= 0)
	{
          EMARD_TIMER[TC_CONTROL] &= ~pwm_enable_bitmask[pwm_channel].control_or;
          EMARD_TIMER[TC_APPLY] = pwm_enable_bitmask[pwm_channel].apply;
	}

	port = digitalPinToPort(pin);

	if (val)
		*port |=  (1<<variant_pin_map[pin].bit_pos);
	else
		*port &= ~(1<<variant_pin_map[pin].bit_pos);
}


int
digitalRead(uint32_t pin)
{
	volatile uint32_t *port;

	if (pin >= variant_pin_map_size)
		return 0;

	port = digitalPinToPort(pin);
	return ((*port & (1<<variant_pin_map[pin].bit_pos)) != 0);
}

__END_DECLS