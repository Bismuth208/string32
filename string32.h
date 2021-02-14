/*
 * Description:
 * Faster version of the standard <string.h> library
 * for ARM Cortex Microcontrollers.
 *
 * This library contain plenty of functions for everyday usage,
 * even if some functions is missing.
 *
 * Author: 
 * Alexandr Antonov (@Bismuth208)
 *
 * Format:
 *  1 Tab == 2 spaces
 *  UTF-8
 *  EOL - Unix
 *
 * Lang: C
 * Prefered Compiler: GCC
 *
 * Licence: MIT
 */

#ifndef _STRING32_H
#define _STRING32_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h> // seems strange, but hey? Where i suppose to get size_t ?

/* =================== Copying ======================= */
void* memcpy32(void* pv_dst, void const* pv_src, size_t x_len);
char* strcpy32(char* pc_dst, const char* pc_src);
char* strncpy32(char* pc_dst, const char* pc_src, size_t x_len);

/* ================ Concatenation ==================== */
char* strcat32(char* pc_dst, const char* pc_src);

/* ================== Comparison ===================== */
size_t memcmp32(void const* pv_ptr1, void const* pv_ptr2, size_t x_len);
size_t strcmp32(const char* pc_str1, const char* pc_str2);

/* ================== Searching ====================== */
// memchr
// strchr

/* ==================== Other ======================== */
void* memset32(void* pv_dst, uint32_t ul_val, size_t x_len);
size_t strlen32(void const* pv_src);

#ifdef __cplusplus
}
#endif

#endif /* _STRING32_H */
