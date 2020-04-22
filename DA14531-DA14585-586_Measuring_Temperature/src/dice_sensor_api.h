#ifndef _DICE_SENSOR_API_
#define _DICE_SENSOR_API_

#define ROLL_INTERRUPT_PIN 
#define ROLL_INTERRUPT_PORT

#include "stdint.h"

void dice_sensor_api_set_roll_interupt(void);
void dice_sensor_api_roll_isr(void);

void dice_sensor_api_get_rotation_velocity(float* vector);
void dice_sensor_api_get_orientation(int16_t* vector);
uint8_t dice_sensor_api_get_up_side(void);

static void dice_sensor_api_set_isr(void);
static int16_t toInt(uint16_t twos_complement);

#endif // _DICE_SENSOR_API_
