#ifndef _DICE_SENSOR_API_
#define _DICE_SENSOR_API_

#define ROLL_INTERRUPT_PIN 
#define ROLL_INTERRUPT_PORT

#include "stdint.h"

bool gyroscope_available(void);
bool accelerometer_available(void);
bool magnetometer_available(void);

void init_gyroscope(void);
void init_accelerometer(void);
void init_magnetometer(void);

void dice_sensor_api_set_roll_interupt(void);
void dice_sensor_api_roll_isr(void);

void dice_sensor_api_get_rotation_velocity(float* vector);
void dice_sensor_api_get_orientation(int16_t* vector);
uint8_t dice_sensor_api_get_up_side(void);

static void dice_sensor_api_set_isr(void);
static int16_t toInt(uint16_t twos_complement);
static void dice_sensor_api_restart_systick(void);
static void dice_sensor_api_wait_for_gyroscope(void);
static void dice_sensor_api_calculate_velocity(const int16_t *vector1, const int16_t *vector2, const uint32_t *dTime, float *result);
static void dice_sensor_api_store_roll();

#endif // _DICE_SENSOR_API_
