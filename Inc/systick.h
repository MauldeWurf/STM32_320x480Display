/*
 * systick.h
 *
 *  Created on: Sep 10, 2025
 *      Author: stevo
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_


#include <stdint.h>
#include "stm32f4xx.h"

void systick_msec_delay(uint32_t delay);

#endif /* SYSTICK_H_ */
