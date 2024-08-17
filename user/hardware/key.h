/**
 * @brief   : 按键
 * @author  : yanglx2022
 * @version : V0.1.0
 * @date    : 2024-05-29
 * @note    : 
 */

#ifndef KEY_H
#define KEY_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

void key_init(void);
uint8_t key_state(void);


#ifdef __cplusplus
}
#endif
#endif /* KEY_H */


