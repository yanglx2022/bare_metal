/**
 * @brief   : 
 * @author  : yanglx2022
 * @version : V0.1.0
 * @date    : 2024-05-17
 * @note    : 
 */

#ifndef COMMON_H
#define COMMON_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "fsl_iomuxc.h"
#include "fsl_uart.h"
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"


void delay(uint32_t count);

#ifdef __cplusplus
}
#endif
#endif /* COMMON_H */


