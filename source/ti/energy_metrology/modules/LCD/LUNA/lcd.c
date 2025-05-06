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

//*****************************************************************************
// the includes
//*****************************************************************************
#include "lcd.h"

/* LCD memory map for segment A digits    */
const char segment_A_digit[10][2] =
{
     {0x50, 0xF0},  /* "0" */
     {0x00, 0x60},  /* "1" */
     {0x60, 0xB0},  /* "2" */
     {0x20, 0xF0},  /* "3" */
     {0x30, 0x60},  /* "4" */
     {0x30, 0xD0},  /* "5" */
     {0x70, 0xD0},  /* "6" */
     {0x00, 0x70},  /* "7" */
     {0x70, 0xF0},  /* "8" */
     {0x30, 0xF0}   /* "9" */
};

/* LCD memory map for segment B digits    */
const char segment_B_digit[10][2] =
{
     {0x0F, 0x05},  /* "0" */
     {0x00, 0x05},  /* "1" */
     {0x0D, 0x03},  /* "2" */
     {0x09, 0x07},  /* "3" */
     {0x02, 0x07},  /* "4" */
     {0x0B, 0x06},  /* "5" */
     {0x0F, 0x06},  /* "6" */
     {0x01, 0x05},  /* "7" */
     {0x0F, 0x07},  /* "8" */
     {0x0B, 0x07}   /* "9" */
};

/*!
 * @brief initialize LCD pin configuration
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_init(LCD_instance *lcdHandle)
{
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_1].pin1 = lcdHandle->lcdPin2;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_1].pin2 = lcdHandle->lcdPin3;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_2].pin1 = lcdHandle->lcdPin4;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_2].pin2 = lcdHandle->lcdPin5;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_3].pin1 = lcdHandle->lcdPin6;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_3].pin2 = lcdHandle->lcdPin7;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_4].pin1 = lcdHandle->lcdPin8;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_4].pin2 = lcdHandle->lcdPin9;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_5].pin1 = lcdHandle->lcdPin10;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_5].pin2 = lcdHandle->lcdPin11;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_6].pin1 = lcdHandle->lcdPin12;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_6].pin2 = lcdHandle->lcdPin13;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_7].pin1 = lcdHandle->lcdPin14;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_7].pin2 = lcdHandle->lcdPin15;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_8].pin1 = lcdHandle->lcdPin16;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_8].pin2 = lcdHandle->lcdPin17;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_9].pin1 = lcdHandle->lcdPin18;
    lcdHandle->lcdSegAPinPosition[LCD_SEGA_POSITION_9].pin2 = lcdHandle->lcdPin19;

    lcdHandle->lcdSegBPinPosition[LCD_SEGB_POSITION_1].pin1 = lcdHandle->lcdPin2;
    lcdHandle->lcdSegBPinPosition[LCD_SEGB_POSITION_1].pin2 = lcdHandle->lcdPin3;
    lcdHandle->lcdSegBPinPosition[LCD_SEGB_POSITION_2].pin1 = lcdHandle->lcdPin4;
    lcdHandle->lcdSegBPinPosition[LCD_SEGB_POSITION_2].pin2 = lcdHandle->lcdPin5;
    lcdHandle->lcdSegBPinPosition[LCD_SEGB_POSITION_3].pin1 = lcdHandle->lcdPin6;
    lcdHandle->lcdSegBPinPosition[LCD_SEGB_POSITION_3].pin2 = lcdHandle->lcdPin7;
    lcdHandle->lcdSegBPinPosition[LCD_SEGB_POSITION_4].pin1 = lcdHandle->lcdPin8;
    lcdHandle->lcdSegBPinPosition[LCD_SEGB_POSITION_4].pin2 = lcdHandle->lcdPin9;

}

/*!
 * @brief Clear all segments in A
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_clearSegmentA(LCD_instance *lcdHandle)
{
    for (LCD_SEGA_POSITION lcdPosition = LCD_SEGA_POSITION_1; lcdPosition < LCD_SEGA_POSITION_MAX; lcdPosition++)
    {
    LCD_showCharonSegmentA(lcdHandle, 'A', lcdHandle->lcdSegAPinPosition[lcdPosition]);
    }
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin12, LCD_SEG_44_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin16, LCD_SEG_45_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin14, LCD_SEG_46_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin18, LCD_SEG_47_MASK);
}

/*!
 * @brief show char on segment A in defined position
 * @param[in] lcdHandle  The LCD instance
 * @param[in] ch   The character to display on LCD
 * @param[in] lcdPinPosition  The position on LCD
 */
void LCD_showCharonSegmentA(LCD_instance *lcdHandle, char ch, LCD_pin lcdPinPosition)
{
        HAL_LCD_CHAN chan = lcdHandle->lcdChan;
        uint32_t pin1 = lcdPinPosition.pin1;
        uint32_t pin2 = lcdPinPosition.pin2;

        uint8_t  mem;
        uint32_t memMask;

        if (ch >= '0' && ch <= '9')
        {
            mem     = HAL_getLCDMemory(chan, pin1) & 0x8F;
            memMask = (mem | segment_A_digit[ch - ASCII_DIGIT_OFFSET][0]);
            HAL_writeLCDMemory(chan, pin1, memMask);

            mem     = HAL_getLCDMemory(chan, pin2) & 0x0F;
            memMask = (mem | segment_A_digit[ch - ASCII_DIGIT_OFFSET][1]);
            HAL_writeLCDMemory(chan, pin2, memMask);
        }
        else
        {
            /* invalid character - clear the segments   */
            mem     = HAL_getLCDMemory(chan, pin1) & 0x8F;
            memMask = (mem | 0x00);
            HAL_writeLCDMemory(chan, pin1, memMask);

            mem     = HAL_getLCDMemory(chan, pin2) & 0x0F;
            memMask = (mem | 0x00);
            HAL_writeLCDMemory(chan, pin2, memMask);
        }
}

/*!
 * @brief show char on segment B in defined position
 * @param[in] lcdHandle  The LCD instance
 * @param[in] ch   The character to display on LCD
 * @param[in] lcdPinPosition  The position on LCD
 */
void LCD_showCharonSegmentB(LCD_instance *lcdHandle, char ch, LCD_pin lcdPinPosition)
{
        HAL_LCD_CHAN chan = lcdHandle->lcdChan;
        uint32_t pin1 = lcdPinPosition.pin1;
        uint32_t pin2 = lcdPinPosition.pin2;

        uint8_t  mem;
        uint32_t memMask;

        if (ch >= '0' && ch <= '9')
        {
            mem     = HAL_getLCDMemory(chan, pin1) & 0xF0;
            memMask = (mem | segment_B_digit[ch - ASCII_DIGIT_OFFSET][0]);
            HAL_writeLCDMemory(chan, pin1, memMask);

            mem     = HAL_getLCDMemory(chan, pin2) & 0xF8;
            memMask = (mem | segment_B_digit[ch - ASCII_DIGIT_OFFSET][1]);
            HAL_writeLCDMemory(chan, pin2, memMask);
        }
        else
        {
            /* invalid character - clear the segments   */
            mem     = HAL_getLCDMemory(chan, pin1) & 0xF0;
            memMask = (mem | 0x00);
            HAL_writeLCDMemory(chan, pin1, memMask);

            mem     = HAL_getLCDMemory(chan, pin2) & 0xF8;
            memMask = (mem | 0x00);
            HAL_writeLCDMemory(chan, pin2, memMask);
        }
}

/*!
 * @brief Display a particular segment on LCD
 * @param[in] lcdHandle  The LCD instance
 * @param[in] memIndex   Segment pin
 * @param[in] memMask    Bit mask for segment
 */
void LCD_displaySegment(LCD_instance *lcdHandle, uint32_t memIndex, uint32_t memMask)
{
    HAL_LCD_CHAN chan = lcdHandle->lcdChan;
    uint32_t mask;

    mask = HAL_getLCDMemory(chan, memIndex) | memMask;
    HAL_writeLCDMemory(chan, memIndex, mask);
}

/*!
 * @brief Clear a particular segment on LCD
 * @param[in] lcdHandle  The LCD instance
 * @param[in] memIndex   Segment pin
 * @param[in] memMask    Bit mask for segment
 */
void LCD_clearSegment(LCD_instance *lcdHandle, uint32_t memIndex, uint32_t memMask)
{
    HAL_LCD_CHAN chan = lcdHandle->lcdChan;
    uint32_t mask;

    mask = HAL_getLCDMemory(chan, memIndex) & ~memMask;
    HAL_writeLCDMemory(chan, memIndex, mask);
}

/*!
 * @brief Clear all segments related to units on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_clearUnits(LCD_instance *lcdHandle)
{
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin16, LCD_SEG_19_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin17, LCD_SEG_15_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin18, LCD_SEG_20_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin19, LCD_SEG_16_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_17_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_21_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_22_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_23_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_18_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin18, LCD_SEG_31_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin18, LCD_SEG_29_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin18, LCD_SEG_32_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin19, LCD_SEG_30_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin19, LCD_SEG_33_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin16, LCD_SEG_34_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_35_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin16, LCD_SEG_36_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_37_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_38_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin11, LCD_SEG_39_MASK);
    LCD_clearSegment(lcdHandle, lcdHandle->lcdPin2, LCD_SEG_40_MASK);
}

/*!
 * @brief Display voltage units (V) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayV(LCD_instance *lcdHandle)
{
    LCD_clearUnits(lcdHandle);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin18, LCD_SEG_31_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin19, LCD_SEG_33_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin16, LCD_SEG_34_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_35_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin16, LCD_SEG_36_MASK);
}

/*!
 * @brief Display current units (I) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayI(LCD_instance *lcdHandle)
{
    LCD_clearUnits(lcdHandle);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin18, LCD_SEG_32_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin16, LCD_SEG_36_MASK);
}

/*!
 * @brief Display active power units (W) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayW(LCD_instance *lcdHandle)
{
    LCD_clearUnits(lcdHandle);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin18, LCD_SEG_20_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin19, LCD_SEG_16_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_17_MASK);
}

/*!
 * @brief Display reactive power units (VAR) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayVAR(LCD_instance *lcdHandle)
{
    LCD_clearUnits(lcdHandle);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin18, LCD_SEG_20_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_17_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_21_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_22_MASK);
}

/*!
 * @brief Display apparent power units (VA) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayVA(LCD_instance *lcdHandle)
{
    LCD_clearUnits(lcdHandle);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin18, LCD_SEG_20_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_17_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_21_MASK);
}

/*!
 * @brief Display active energy units (Wh) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayKWH(LCD_instance *lcdHandle)
{
    LCD_clearUnits(lcdHandle);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin16, LCD_SEG_19_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin17, LCD_SEG_15_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin18, LCD_SEG_20_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin19, LCD_SEG_16_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_17_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_23_MASK);
}

/*!
 * @brief Display reactive energy units (VARh) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayKVARH(LCD_instance *lcdHandle)
{
    LCD_clearUnits(lcdHandle);

    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin16, LCD_SEG_19_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin17, LCD_SEG_15_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin18, LCD_SEG_20_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_17_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_21_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_22_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_23_MASK);
}

/*!
 * @brief Display apparent energy units (VAh) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayKVAH(LCD_instance *lcdHandle)
{
    LCD_clearUnits(lcdHandle);

    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin16, LCD_SEG_19_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin17, LCD_SEG_15_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin18, LCD_SEG_20_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_17_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_21_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_23_MASK);
}

/*!
 * @brief Display frequency units (Hz) on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayHz(LCD_instance *lcdHandle)
{
    LCD_clearUnits(lcdHandle);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin1, LCD_SEG_18_MASK);
}

/*!
 * @brief Display colons on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_displayColons(LCD_instance *lcdHandle)
{
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin12, LCD_SEG_44_MASK);
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin16, LCD_SEG_45_MASK);
}

/*!
 * @brief Display decimal point for 3 fraction parts on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_diplayThreeFractionDigits(LCD_instance *lcdHandle)
{
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin14, LCD_SEG_46_MASK);
}

/*!
 * @brief Display decimal point for 2 fraction parts on LCD
 * @param[in] lcdHandle  The LCD instance
 */
void LCD_diplayTwoFractionDigits(LCD_instance *lcdHandle)
{
    LCD_displaySegment(lcdHandle, lcdHandle->lcdPin18, LCD_SEG_47_MASK);
}
