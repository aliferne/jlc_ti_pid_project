/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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

/*!****************************************************************************
 *  @file       lcd.h
 *  @brief      Energy Library LCD Module
 *
 *
 *  @anchor energy_library_LUNA_lcd_mapping
 *  # Overview
 *
 *  The LCD module provides the mapping to display digits, letters and special
 *  indicators on a LCD (LUNA)
 *
 *  <hr>
 ******************************************************************************/
/** @addtogroup LCD_LUNA
 * @{
 */

#ifndef LCD_H
#define LCD_H

//*****************************************************************************
// the includes
//*****************************************************************************
#include "hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief ASCII offset for digits   */
#define ASCII_DIGIT_OFFSET      (48)
/*! @brief ASCII offset for letters  */
#define ASCII_LETTER_OFFSET     (65)

#define LCD_SEG_1_MASK       0x08
#define LCD_SEG_2_MASK       0x08
#define LCD_SEG_3_MASK       0x08
#define LCD_SEG_4_MASK       0x80
#define LCD_SEG_5_MASK       0x08
#define LCD_SEG_6_MASK       0x02
#define LCD_SEG_7_MASK       0x01
#define LCD_SEG_8_MASK       0x01
#define LCD_SEG_9_MASK       0x02
#define LCD_SEG_10_MASK      0x01
#define LCD_SEG_11_MASK      0x04
#define LCD_SEG_12_MASK      0x01
#define LCD_SEG_13_MASK      0x04
#define LCD_SEG_14_MASK      0x04
#define LCD_SEG_15_MASK      0x08
#define LCD_SEG_16_MASK      0x08
#define LCD_SEG_17_MASK      0x08
#define LCD_SEG_18_MASK      0x80
#define LCD_SEG_19_MASK      0x08
#define LCD_SEG_20_MASK      0x08
#define LCD_SEG_21_MASK      0x10
#define LCD_SEG_22_MASK      0x20
#define LCD_SEG_23_MASK      0x40
#define LCD_SEG_24_MASK      0x80
#define LCD_SEG_25_MASK      0x80
#define LCD_SEG_26_MASK      0x80
#define LCD_SEG_27_MASK      0x04
#define LCD_SEG_28_MASK      0x02
#define LCD_SEG_29_MASK      0x02
#define LCD_SEG_30_MASK      0x04
#define LCD_SEG_31_MASK      0x01
#define LCD_SEG_32_MASK      0x04
#define LCD_SEG_33_MASK      0x02
#define LCD_SEG_34_MASK      0x02
#define LCD_SEG_35_MASK      0x04
#define LCD_SEG_36_MASK      0x04
#define LCD_SEG_37_MASK      0x02
#define LCD_SEG_38_MASK      0x01
#define LCD_SEG_39_MASK      0x04
#define LCD_SEG_40_MASK      0x80
#define LCD_SEG_41_MASK      0x08
#define LCD_SEG_42_MASK      0x08
#define LCD_SEG_43_MASK      0x01
#define LCD_SEG_44_MASK      0x80
#define LCD_SEG_45_MASK      0x80
#define LCD_SEG_46_MASK      0x80
#define LCD_SEG_47_MASK      0x80

/*! @enum LCD_SEGA_POSITION */
typedef enum
{
    /*! @brief Segment A position 1 on LCD  */
    LCD_SEGA_POSITION_1 = 0,
    /*! @brief Segment A position 2 on LCD  */
    LCD_SEGA_POSITION_2,
    /*! @brief Segment A position 3 on LCD  */
    LCD_SEGA_POSITION_3,
    /*! @brief Segment A position 4 on LCD  */
    LCD_SEGA_POSITION_4,
    /*! @brief Segment A position 5 on LCD  */
    LCD_SEGA_POSITION_5,
    /*! @brief Segment A position 6 on LCD  */
    LCD_SEGA_POSITION_6,
    /*! @brief Segment A position 7 on LCD  */
    LCD_SEGA_POSITION_7,
    /*! @brief Segment A position 8 on LCD  */
    LCD_SEGA_POSITION_8,
    /*! @brief Segment A position 9 on LCD  */
    LCD_SEGA_POSITION_9,
    /*! @brief Segment A max positions on LCD  */
    LCD_SEGA_POSITION_MAX
}LCD_SEGA_POSITION;

/*! @enum LCD_SEGB_POSITION */
typedef enum
{
    /*! @brief Segment B position B1 on LCD  */
    LCD_SEGB_POSITION_1 = 0,
    /*! @brief Segment B position B2 on LCD  */
    LCD_SEGB_POSITION_2,
    /*! @brief Segment B position B3 on LCD  */
    LCD_SEGB_POSITION_3,
    /*! @brief Segment B position B4 on LCD  */
    LCD_SEGB_POSITION_4,
    /*! @brief Segment B max positions on LCD  */
    LCD_SEGB_POSITION_MAX
}LCD_SEGB_POSITION;

/*! @brief Defines LCD pins for each position */
typedef struct
{
    /*! @brief Pin 1 for each position  */
    uint8_t pin1;
    /*! @brief Pin 2 for each position  */
    uint8_t pin2;
}LCD_pin;

/*! @brief Defines LCD instance */
typedef struct
{
    /*! @brief Stores LCD module   */
    HAL_LCD_CHAN lcdChan;
    /*! @brief Array to store segment A pins of each position */
    LCD_pin lcdSegAPinPosition[LCD_SEGA_POSITION_MAX];
    /*! @brief Array to store segment B pins of each position */
    LCD_pin lcdSegBPinPosition[LCD_SEGB_POSITION_MAX];

    uint8_t lcdPin1;
    uint8_t lcdPin2;
    uint8_t lcdPin3;
    uint8_t lcdPin4;
    uint8_t lcdPin5;
    uint8_t lcdPin6;
    uint8_t lcdPin7;
    uint8_t lcdPin8;
    uint8_t lcdPin9;
    uint8_t lcdPin10;
    uint8_t lcdPin11;
    uint8_t lcdPin12;
    uint8_t lcdPin13;
    uint8_t lcdPin14;
    uint8_t lcdPin15;
    uint8_t lcdPin16;
    uint8_t lcdPin17;
    uint8_t lcdPin18;
    uint8_t lcdPin19;
}LCD_instance;

/*!
 * @brief initialize LCD pin configuration
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_init(LCD_instance *lcdHandle);

/*!
 * @brief Clear all segments in A
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_clearSegmentA(LCD_instance *lcdHandle);

/*!
 * @brief show char on segment A in defined position
 * @param[in] lcdHandle  The LCD instance
 * @param[in] ch   The character to display on LCD
 * @param[in] lcdPinPosition  The position on LCD
 */
void LCD_showCharonSegmentA(LCD_instance *lcdHandle, char ch, LCD_pin lcdPinPosition);

/*!
 * @brief show char on segment B in defined position
 * @param[in] lcdHandle  The LCD instance
 * @param[in] ch   The character to display on LCD
 * @param[in] lcdPinPosition  The position on LCD
 */
void LCD_showCharonSegmentB(LCD_instance *lcdHandle, char ch, LCD_pin lcdPinPosition);

/*!
 * @brief Display a particular segment on LCD
 * @param[in] lcdHandle  The LCD instance
 * @param[in] memIndex   Segment pin
 * @param[in] memMask    Bit mask for segment
 */
void LCD_displaySegment(LCD_instance *lcdHandle, uint32_t memIndex, uint32_t memMask);

/*!
 * @brief Clear a particular segment on LCD
 * @param[in] lcdHandle  The LCD instance
 * @param[in] memIndex   Segment pin
 * @param[in] memMask    Bit mask for segment
 */
void LCD_clearSegment(LCD_instance *lcdHandle, uint32_t memIndex, uint32_t memMask);

/*!
 * @brief Clear all segments related to units on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_clearUnits(LCD_instance *lcdHandle);

/*!
 * @brief Display voltage units (V) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayV(LCD_instance *lcdHandle);

/*!
 * @brief Display current units (I) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayI(LCD_instance *lcdHandle);

/*!
 * @brief Display active power units (W) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayW(LCD_instance *lcdHandle);

/*!
 * @brief Display reactive power units (VAR) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayVAR(LCD_instance *lcdHandle);

/*!
 * @brief Display apparent power units (VA) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayVA(LCD_instance *lcdHandle);

/*!
 * @brief Display active energy units (Wh) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayKWH(LCD_instance *lcdHandle);

/*!
 * @brief Display reactive energy units (VARh) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayKVARH(LCD_instance *lcdHandle);

/*!
 * @brief Display apparent energy units (VAh) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayKVAH(LCD_instance *lcdHandle);

/*!
 * @brief Display frequency units (Hz) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayHz(LCD_instance *lcdHandle);

/*!
 * @brief Display colons on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayColons(LCD_instance *lcdHandle);

/*!
 * @brief Display decimal point for 3 fraction parts on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_diplayThreeFractionDigits(LCD_instance *lcdHandle);

/*!
 * @brief Display decimal point for 2 fraction parts on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_diplayTwoFractionDigits(LCD_instance *lcdHandle);

#ifdef __cplusplus
}
#endif
#endif /* LCD_H */
/** @}*/
