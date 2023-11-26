/*
*******************************************************************************
Copywright ELXR LLC, 2023
All Rights Reserved
UNPUBLISHED, LICENSED SOFTWARE.

CONFIDENTIAL AND PROPRIETARY INFORMATION WHICH IS THE PROPERTY OF ELXR LLC.

File:       print.h

Author:     Gregory Johnson
Created:    02-13-2023
Modified:
*******************************************************************************
*/

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void print_buffer(const uint8_t* buffer, uint32_t len, const char* msg);
void print_partitions(void);

#ifdef __cplusplus
}
#endif
