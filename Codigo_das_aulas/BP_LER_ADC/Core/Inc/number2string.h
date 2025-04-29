/**
 * @file number2string.h
 * @brief Library for converting numbers to strings for LCD display
 * @author Mozart Fraga
 * @date March 23, 2025
 */

#ifndef NUM_TO_STR_H
#define NUM_TO_STR_H

#include <stdint.h>

/**
 * @brief Convert integer to string
 * @param num: Integer to convert
 * @param str: Character array to store the result
 * @param base: Numeric base (10 for decimal, 16 for hex, etc.)
 * @return Number of characters written
 */
uint8_t IntToStr(int32_t num, char* str, uint8_t base);

/**
 * @brief Convert unsigned integer to string
 * @param num: Unsigned integer to convert
 * @param str: Character array to store the result
 * @param base: Numeric base (10 for decimal, 16 for hex, etc.)
 * @return Number of characters written
 */
uint8_t UintToStr(uint32_t num, char* str, uint8_t base);

/**
 * @brief Convert float to string with specified precision
 * @param num: Float to convert
 * @param str: Character array to store the result
 * @param precision: Number of decimal places
 * @return Number of characters written
 */
uint8_t FloatToStr(float num, char* str, uint8_t precision);

/**
 * @brief Format number with thousand separators
 * @param num: Integer to format
 * @param str: Character array to store the result
 * @param separator: Character to use as thousand separator
 * @return Number of characters written
 */
uint8_t FormatInt(int32_t num, char* str, char separator);

/**
 * @brief Pad number string with leading zeros
 * @param str: String to pad
 * @param width: Desired width
 */
void PadZeros(char* str, uint8_t width);

/**
 * @brief Right-align string in fixed width
 * @param str: String to align
 * @param width: Desired width
 */
void RightAlign(char* str, uint8_t width);

/**
 * @brief Center string in fixed width
 * @param str: String to center
 * @param width: Desired width
 */
void CenterAlign(char* str, uint8_t width);

#endif /* NUM_TO_STR_H */