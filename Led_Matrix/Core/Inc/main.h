
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif



/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <sp_stm32_gpio.h>
#include <sp_stm32_clock.h>
#include <sp_stm32_spi.h>
#include <sp_led_matrix.h>
#include "sp_stm32_adc.h"
#include "sp_stm32_uart.h"

#define SCK_PIN 5
#define MOSI_PIN 7
#define CS_PIN 5
#define Analog_Pin 0



#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

