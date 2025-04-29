/**
 * @file number2string.c
 * @brief Implementation of number to string conversion functions
 * @author Mozart Fraga
 * @date March 23, 2025
 */

#include "number2string.h"
#include <string.h>

/**
 * Helper function to reverse a string
 */
static void ReverseStr(char* str, uint8_t length) {
    uint8_t start = 0;
    uint8_t end = length - 1;
    char temp;
    
    while (start < end) {
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

uint8_t IntToStr(int32_t num, char* str, uint8_t base) {
    uint8_t i = 0;
    uint8_t isNegative = 0;
    
    // Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return i;
    }
    
    // Handle negative numbers
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }
    
    // Process individual digits
    while (num != 0) {
        int32_t rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        num = num / base;
    }
    
    // If negative, append '-'
    if (isNegative)
        str[i++] = '-';
    
    // Terminate string
    str[i] = '\0';
    
    // Reverse the string
    ReverseStr(str, i);
    
    return i;
}

uint8_t UintToStr(uint32_t num, char* str, uint8_t base) {
    uint8_t i = 0;
    
    // Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return i;
    }
    
    // Process individual digits
    while (num != 0) {
        uint32_t rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        num = num / base;
    }
    
    // Terminate string
    str[i] = '\0';
    
    // Reverse the string
    ReverseStr(str, i);
    
    return i;
}

uint8_t FloatToStr(float num, char* str, uint8_t precision) {
    uint8_t i = 0;
    
    // Handle negative numbers
    if (num < 0) {
        str[i++] = '-';
        num = -num;
    }
    
    // Extract integer part
    int32_t intPart = (int32_t)num;
    
    // Extract fractional part
    float fracPart = num - (float)intPart;
    
    // Convert integer part to string
    i += IntToStr(intPart, &str[i], 10);
    
    // Check if precision is specified
    if (precision > 0) {
        // Add decimal point
        str[i++] = '.';
        
        // Multiply by 10^precision and take integer part
        uint32_t fracInt = 0;
        for (uint8_t j = 0; j < precision; j++) {
            fracPart *= 10;
        }
        fracInt = (uint32_t)fracPart;
        
        // Handle rounding
        if ((uint32_t)(fracPart * 10) % 10 >= 5) {
            fracInt++;
        }
        
        // Convert fractional part to string with leading zeros
        uint8_t fractionChars = UintToStr(fracInt, &str[i], 10);
        
        // Add leading zeros if needed
        while (fractionChars < precision) {
            // Shift characters to make room for zeros
            for (int8_t j = i + fractionChars; j >= i; j--) {
                str[j + 1] = str[j];
            }
            str[i] = '0';
            fractionChars++;
        }
        
        i += precision;
    }
    
    // Null terminate the string
    str[i] = '\0';
    
    return i;
}

uint8_t FormatInt(int32_t num, char* str, char separator) {
    // First convert the number to string
    char tempStr[16];  // Temporary buffer
    uint8_t len = IntToStr(num, tempStr, 10);
    
    // Handle negative sign separately
    uint8_t startPos = 0;
    uint8_t outPos = 0;
    
    if (tempStr[0] == '-') {
        str[outPos++] = '-';
        startPos = 1;
    }
    
    // Insert thousand separators
    uint8_t digitCount = 0;
    for (int8_t i = len - 1; i >= startPos; i--) {
        if (digitCount == 3 && i != startPos) {
            str[outPos++] = separator;
            digitCount = 0;
        }
        str[outPos++] = tempStr[i];
        digitCount++;
    }
    
    // Null terminate
    str[outPos] = '\0';
    
    // Now reverse the formatted part
    if (startPos == 1) {
        ReverseStr(str + 1, outPos - 1);
    } else {
        ReverseStr(str, outPos);
    }
    
    return outPos;
}

void PadZeros(char* str, uint8_t width) {
    uint8_t len = strlen(str);
    
    // Check if padding is needed
    if (len >= width) return;
    
    // Shift existing characters to the right
    for (int8_t i = len; i >= 0; i--) {
        str[i + (width - len)] = str[i];
    }
    
    // Fill with leading zeros
    for (uint8_t i = 0; i < (width - len); i++) {
        str[i] = '0';
    }
}

void RightAlign(char* str, uint8_t width) {
    uint8_t len = strlen(str);
    
    // Check if padding is needed
    if (len >= width) return;
    
    // Shift existing characters to the right
    for (int8_t i = len; i >= 0; i--) {
        str[i + (width - len)] = str[i];
    }
    
    // Fill with spaces
    for (uint8_t i = 0; i < (width - len); i++) {
        str[i] = ' ';
    }
}

void CenterAlign(char* str, uint8_t width) {
    uint8_t len = strlen(str);
    
    // Check if padding is needed
    if (len >= width) return;
    
    uint8_t leftPad = (width - len) / 2;
    
    // Shift existing characters
    for (int8_t i = len; i >= 0; i--) {
        str[i + leftPad] = str[i];
    }
    
    // Fill with spaces
    for (uint8_t i = 0; i < leftPad; i++) {
        str[i] = ' ';
    }
    
    // Fill right side with spaces
    for (uint8_t i = leftPad + len; i < width; i++) {
        str[i] = ' ';
    }
    
    // Ensure null termination
    str[width] = '\0';
}
