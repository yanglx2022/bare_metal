/**
 * @brief   : LED
 * @author  : yanglx2022
 * @version : V0.1.0
 * @date    : 2024-05-21
 * @note    : 
 */

#ifndef LED_H
#define LED_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

#define LED1                            (0x01)
#define LED2                            (0x02)
#define LED3                            (0x04)
#define LED4                            (0x08)

void led_init(void);
void led_ctrl(uint8_t led, uint8_t enable);


#ifdef __cplusplus
}
#endif
#endif /* LED_H */


