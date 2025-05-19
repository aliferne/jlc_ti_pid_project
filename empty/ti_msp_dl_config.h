/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000



/* Defines for SG90 */
#define SG90_INST                                                          TIMG0
#define SG90_INST_IRQHandler                                    TIMG0_IRQHandler
#define SG90_INST_INT_IRQN                                      (TIMG0_INT_IRQn)
#define SG90_INST_CLK_FREQ                                                 20000
/* GPIO defines for channel 0 */
#define GPIO_SG90_C0_PORT                                                  GPIOA
#define GPIO_SG90_C0_PIN                                          DL_GPIO_PIN_12
#define GPIO_SG90_C0_IOMUX                                       (IOMUX_PINCM34)
#define GPIO_SG90_C0_IOMUX_FUNC                      IOMUX_PINCM34_PF_TIMG0_CCP0
#define GPIO_SG90_C0_IDX                                     DL_TIMER_CC_0_INDEX

/* Defines for SG90S */
#define SG90S_INST                                                         TIMA0
#define SG90S_INST_IRQHandler                                   TIMA0_IRQHandler
#define SG90S_INST_INT_IRQN                                     (TIMA0_INT_IRQn)
#define SG90S_INST_CLK_FREQ                                                20000
/* GPIO defines for channel 0 */
#define GPIO_SG90S_C0_PORT                                                 GPIOB
#define GPIO_SG90S_C0_PIN                                          DL_GPIO_PIN_8
#define GPIO_SG90S_C0_IOMUX                                      (IOMUX_PINCM25)
#define GPIO_SG90S_C0_IOMUX_FUNC                     IOMUX_PINCM25_PF_TIMA0_CCP0
#define GPIO_SG90S_C0_IDX                                    DL_TIMER_CC_0_INDEX



/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_FREQUENCY                                           32000000
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM22)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM21)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM21_PF_UART0_TX
#define UART_0_BAUD_RATE                                                  (9600)
#define UART_0_IBRD_32_MHZ_9600_BAUD                                       (208)
#define UART_0_FBRD_32_MHZ_9600_BAUD                                        (21)





/* Port definition for Pin Group LED */
#define LED_PORT                                                         (GPIOB)

/* Defines for GREEN: GPIOB.22 with pinCMx 50 on package pin 21 */
#define LED_GREEN_PIN                                           (DL_GPIO_PIN_22)
#define LED_GREEN_IOMUX                                          (IOMUX_PINCM50)
/* Defines for DOWN: GPIOA.28 with pinCMx 3 on package pin 35 */
#define KEY_DOWN_PORT                                                    (GPIOA)
// pins affected by this interrupt request:["DOWN"]
#define KEY_GPIOA_INT_IRQN                                      (GPIOA_INT_IRQn)
#define KEY_GPIOA_INT_IIDX                      (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define KEY_DOWN_IIDX                                       (DL_GPIO_IIDX_DIO28)
#define KEY_DOWN_PIN                                            (DL_GPIO_PIN_28)
#define KEY_DOWN_IOMUX                                            (IOMUX_PINCM3)
/* Defines for UP: GPIOA.9 with pinCMx 20 on package pin 55 */
#define KEY_UP_PORT                                                      (GPIOA)
#define KEY_UP_PIN                                               (DL_GPIO_PIN_9)
#define KEY_UP_IOMUX                                             (IOMUX_PINCM20)
/* Defines for LEFT: GPIOA.8 with pinCMx 19 on package pin 54 */
#define KEY_LEFT_PORT                                                    (GPIOA)
#define KEY_LEFT_PIN                                             (DL_GPIO_PIN_8)
#define KEY_LEFT_IOMUX                                           (IOMUX_PINCM19)
/* Defines for RIGHT: GPIOA.31 with pinCMx 6 on package pin 39 */
#define KEY_RIGHT_PORT                                                   (GPIOA)
#define KEY_RIGHT_PIN                                           (DL_GPIO_PIN_31)
#define KEY_RIGHT_IOMUX                                           (IOMUX_PINCM6)
/* Defines for MID: GPIOB.4 with pinCMx 17 on package pin 52 */
#define KEY_MID_PORT                                                     (GPIOB)
// pins affected by this interrupt request:["MID"]
#define KEY_GPIOB_INT_IRQN                                      (GPIOB_INT_IRQn)
#define KEY_GPIOB_INT_IIDX                      (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define KEY_MID_IIDX                                         (DL_GPIO_IIDX_DIO4)
#define KEY_MID_PIN                                              (DL_GPIO_PIN_4)
#define KEY_MID_IOMUX                                            (IOMUX_PINCM17)



/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_SG90_init(void);
void SYSCFG_DL_SG90S_init(void);
void SYSCFG_DL_UART_0_init(void);

void SYSCFG_DL_SYSTICK_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
