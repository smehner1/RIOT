/*
 * Copyright (C) 2015 Eistec AB
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @defgroup        cpu_k60 Freescale Kinetis K60
 * @ingroup         cpu
 * @brief           CPU specific implementations for the Freescale Kinetis K60
 * @{
 *
 * @file
 * @brief           Implementation specific CPU configuration options
 *
 * @author          Joakim Nohlgård <joakim.nohlgard@eistec.se>
 */

#ifndef CPU_CONF_H
#define CPU_CONF_H

#include "cpu_conf_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#if defined(CPU_MODEL_MK60DN512VLL10) || defined(CPU_MODEL_MK60DN256VLL10)
#include "vendor/MK60D10.h"

/** The expected CPUID value, can be used to implement a check that we are
 * running on the right hardware */
#define K60_EXPECTED_CPUID 0x410fc241u

/* K60 rev 2.x replaced the RNG module in 1.x by the RNGA PRNG module */
#define KINETIS_RNGA            (RNG)
#else
#error Unknown CPU model. Update Makefile.include in the board directory.
#endif

/**
 * @brief This CPU provides an additional ADC clock divider as CFG1[ADICLK]=1
 */
#define KINETIS_HAVE_ADICLK_BUS_DIV_2 1

/**
 * @brief   ARM Cortex-M specific CPU configuration
 * @{
 */
#define CPU_DEFAULT_IRQ_PRIO            (1U)
#define CPU_IRQ_NUMOF                   (104U)
#define CPU_FLASH_BASE                  (0x00000000)
/** @} */

/**
 * @name GPIO pin mux function numbers
 */
/** @{ */
#define PIN_MUX_FUNCTION_ANALOG 0
#define PIN_MUX_FUNCTION_GPIO 1
/** @} */
/**
 * @name GPIO interrupt flank settings
 */
/** @{ */
#define PIN_INTERRUPT_RISING 0b1001
#define PIN_INTERRUPT_FALLING 0b1010
#define PIN_INTERRUPT_EDGE 0b1011
/** @} */

/**
 * @name Timer hardware information
 */
/** @{ */
#define LPTMR_CLKEN()  (bit_set32(&SIM->SCGC5, SIM_SCGC5_LPTMR_SHIFT)) /**< Enable LPTMR0 clock gate */
#define PIT_CLKEN()    (bit_set32(&SIM->SCGC6, SIM_SCGC6_PIT_SHIFT)) /**< Enable PIT clock gate */
/** @} */

/**
 * @name Power mode hardware details
 */
/** @{ */
#define KINETIS_PMCTRL SMC->PMCTRL
#define KINETIS_PMCTRL_SET_MODE(x) (KINETIS_PMCTRL = SMC_PMCTRL_STOPM(x) | SMC_PMCTRL_LPWUI_MASK)
/* Clear LLS protection, clear VLPS, VLPW, VLPR protection */
/* Note: This register can only be written once after each reset, so we must
 * enable all power modes that we wish to use. */
#define KINETIS_PMPROT_UNLOCK() (SMC->PMPROT |= SMC_PMPROT_ALLS_MASK | SMC_PMPROT_AVLP_MASK)

/**
 * @name STOP mode bitfield values
 * @{
 */
/** @brief Normal STOP */
#define KINETIS_POWER_MODE_NORMAL (0b000)
/** @brief VLPS STOP */
#define KINETIS_POWER_MODE_VLPS   (0b010)
/** @brief LLS STOP */
#define KINETIS_POWER_MODE_LLS    (0b011)
/** @} */

/**
 * @brief Wake up source number for the LPTMR0
 *
 * In order to let the hwtimer wake the CPU from low power modes, we need to
 * enable this wake up source.
 */
#define KINETIS_LLWU_WAKEUP_MODULE_LPTMR 0

/**
 * @brief IRQn name to enable LLWU IRQ in NVIC
 */
#define KINETIS_LLWU_IRQ LLW_IRQn

/**
 * @brief Enable clock gate on LLWU module.
 */
#define LLWU_UNLOCK() (BITBAND_REG32(SIM->SCGC4, SIM_SCGC4_LLWU_SHIFT) = 1)

/**
 * @brief Internal modules whose interrupts are mapped to LLWU wake up sources.
 *
 * Other modules CAN NOT be used to wake the CPU from LLS or VLLSx power modes.
 */
typedef enum llwu_wakeup_module {
    KINETIS_LPM_WAKEUP_MODULE_LPTMR = 0,
    KINETIS_LPM_WAKEUP_MODULE_CMP0 = 1,
    KINETIS_LPM_WAKEUP_MODULE_CMP1 = 2,
    KINETIS_LPM_WAKEUP_MODULE_CMP2 = 3,
    KINETIS_LPM_WAKEUP_MODULE_TSI = 4,
    KINETIS_LPM_WAKEUP_MODULE_RTC_ALARM = 5,
    KINETIS_LPM_WAKEUP_MODULE_RESERVED = 6,
    KINETIS_LPM_WAKEUP_MODULE_RTC_SECONDS = 7,
    KINETIS_LPM_WAKEUP_MODULE_END,
} llwu_wakeup_module_t;

/**
 * @brief enum that maps physical pins to wakeup pin numbers in LLWU module
 *
 * Other pins CAN NOT be used to wake the CPU from LLS or VLLSx power modes.
 */
typedef enum llwu_wakeup_pin {
    KINETIS_LPM_WAKEUP_PIN_PTE1 = 0,
    KINETIS_LPM_WAKEUP_PIN_PTE2 = 1,
    KINETIS_LPM_WAKEUP_PIN_PTE4 = 2,
    KINETIS_LPM_WAKEUP_PIN_PTA4 = 3,
    KINETIS_LPM_WAKEUP_PIN_PTA13 = 4,
    KINETIS_LPM_WAKEUP_PIN_PTB0 = 5,
    KINETIS_LPM_WAKEUP_PIN_PTC1 = 6,
    KINETIS_LPM_WAKEUP_PIN_PTC3 = 7,
    KINETIS_LPM_WAKEUP_PIN_PTC4 = 8,
    KINETIS_LPM_WAKEUP_PIN_PTC5 = 9,
    KINETIS_LPM_WAKEUP_PIN_PTC6 = 10,
    KINETIS_LPM_WAKEUP_PIN_PTC11 = 11,
    KINETIS_LPM_WAKEUP_PIN_PTD0 = 12,
    KINETIS_LPM_WAKEUP_PIN_PTD2 = 13,
    KINETIS_LPM_WAKEUP_PIN_PTD4 = 14,
    KINETIS_LPM_WAKEUP_PIN_PTD6 = 15,
    KINETIS_LPM_WAKEUP_PIN_END
} llwu_wakeup_pin_t;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* CPU_CONF_H */
/** @} */
