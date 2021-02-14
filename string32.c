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

#include "string32.h"


#ifdef __GNUC__
// Fits for GCC
#define _STRING32_LIB_OPTIMIZE_ATTR  __attribute__ ((optimize("O2")))
#endif // __GNUC__

#ifdef __ICCARM__
// Fits for IAR Embedded Workbench
#define _STRING32_LIB_OPTIMIZE_ATTR #pragma optimize=speed
#endif // __ICCARM__


#ifndef _STRING32_LIB_OPTIMIZE_ATTR
// If compiler is not supported just use this
#define _STRING32_LIB_OPTIMIZE_ATTR
#endif // _STRING32_LIB_OPTIMIZE_ATTR


// Enable this rule to add NULL checks to input pointers
// This can improve overall safety, but behavor will not like STD Lib.
//#define _STRING32_LIB_OPTIMIZE_NULL_CHECK



/* =================== Copying ======================= */

/*
 * @brief Copy block of memory
 * @param *pv_dst - Pointer to the destination array where the content is to be copied
 * @param *pv_src - Pointer to the source of data to be copied
 * @param x_len - Number of bytes to copy
 * @retval destination buffer pointer
 */
_STRING32_LIB_OPTIMIZE_ATTR
void* memcpy32(void* pv_dst, void const* pv_src, size_t x_len)
{
#ifdef _STRING32_LIB_OPTIMIZE_NULL_CHECK
  if ((pv_dst == NULL) || (pv_src == NULL)) {
    return NULL;
  }
#endif

  uint32_t* pul_dst = (uint32_t*) pv_dst;
  uint32_t const* pul_src = (uint32_t const*) pv_src;

  while (x_len >= sizeof(uint32_t)) {
    *pul_dst = *pul_src;

    ++pul_dst;
    ++pul_src;
    x_len -= sizeof(uint32_t);
  }

  uint8_t* puc_dst = (uint8_t*) pul_dst;
  uint8_t const* puc_src = (uint8_t const*) pul_src;

  while (x_len--) {
    *puc_dst = *puc_src;

    ++puc_dst;
    ++puc_src;
  }

  return pv_dst;
}

/*
 * @brief Copies the C string
 * @param *pc_dst - Pointer to the destination array where the content is to be copied
 * @param *pc_src - C string to be copied
 * @retval pointer to destination string
 */
_STRING32_LIB_OPTIMIZE_ATTR
char* strcpy32(char* pc_dst, const char* pc_src)
{
#ifdef _STRING32_LIB_OPTIMIZE_NULL_CHECK
  if ((pc_dst == NULL) || (pc_src == NULL)) {
    return NULL;
  }
#endif

  size_t x_len = strlen32(pc_src);

  return (char*) memcpy32((void*) pc_dst, (void const*) pc_src, x_len);
}

/*
 * @brief Copy characters from string
 * @param *pc_dst - Pointer to the destination array where the content is to be copied
 * @param *pc_src - C string to be copied
 * @param x_len - Maximum number of characters to be copied from source
 * @retval pointer to destination string
 */
_STRING32_LIB_OPTIMIZE_ATTR
char* strncpy32(char* pc_dst, const char* pc_src, size_t x_len)
{
#ifdef _STRING32_LIB_OPTIMIZE_NULL_CHECK
  if ((pc_dst == NULL) || (pc_src == NULL)) {
    return NULL;
  }
#endif

  return (char*) memcpy32((void*) pc_dst, (void const*) pc_src, x_len);
}

/* ================ Concatenation ==================== */

/*
 * @brief Concatenate strings
 * @param *pc_dst - Pointer to the destination array, which should contain a C string, 
                    and be large enough to contain the concatenated resulting string
 * @param *pc_src - C string to be appended. This should not overlap destination
 * @retval pointer to original string
 */
_STRING32_LIB_OPTIMIZE_ATTR
char* strcat32(char* pc_dst, const char* pc_src)
{
#ifdef _STRING32_LIB_OPTIMIZE_NULL_CHECK
  if ((pc_dst == NULL) || (pc_src == NULL)) {
    return NULL;
  }
#endif

  size_t x_len1 = strlen32(pc_dst);
  size_t x_len2 = strlen32(pc_src);

  memcpy32((void*) (pc_dst + x_len1), pc_src, x_len2);

  pc_dst[x_len1 + x_len2 + 1] = '\0';

  return pc_dst;
}

/* ================== Comparison ===================== */

/*
 * @brief Compare two blocks of memory 
 * @param *pv_ptr1 - Pointer to block of memory
 * @param *pv_ptr2 - Pointer to block of memory
 * @param x_len - Number of bytes to compare
 * @retval -1,0,+1 according to original memcmp
 */
_STRING32_LIB_OPTIMIZE_ATTR
size_t memcmp32(void const* pv_ptr1, void const* pv_ptr2, size_t x_len)
{
#ifdef _STRING32_LIB_OPTIMIZE_NULL_CHECK
  if ((pv_ptr1 == NULL) || (pv_ptr2 == NULL)) {
    return 0UL;
  }
#endif

  uint32_t const *pul_ptr1 = (uint32_t const *) pv_ptr1;
  uint32_t const *pul_ptr2 = (uint32_t const *) pv_ptr2;

  size_t x_res = 0L;

  if (pul_ptr1 == pul_ptr2) {
    goto MEMCMP32_DONE_END;
  } else {
    while (x_len >= sizeof(uint32_t)) {
      if (*pul_ptr1 != *pul_ptr2) {
        x_res = (*pul_ptr1 > *pul_ptr2) ? 1 : -1;
        goto MEMCMP32_DONE_END;
      }

      ++pul_ptr1;
      ++pul_ptr2;
      x_len -= sizeof(uint32_t);
    }

    uint8_t const *puc_ptr1 = (uint8_t const *) pul_ptr1;
    uint8_t const *puc_ptr2 = (uint8_t const *) pul_ptr2;

    while (x_len--) {
      if (*puc_ptr1 != *puc_ptr2) {
        x_res = (*puc_ptr1 > *puc_ptr2) ? 1 : -1;
        goto MEMCMP32_DONE_END;
      }

      ++puc_ptr1;
      ++puc_ptr2;
    }
  }

  MEMCMP32_DONE_END:
  return x_res;
}

/*
 * @brief Compares the C string str1 to the C string str2
 * @param *pc_str1 - C string to be compared
 * @param *pc_str2 - C string to be compared
 * @retval -1,0,+1 according to original strcmp
 */
_STRING32_LIB_OPTIMIZE_ATTR
size_t strcmp32(const char* pc_str1, const char* pc_str2)
{
#ifdef _STRING32_LIB_OPTIMIZE_NULL_CHECK
  if ((pc_str1 == NULL) || (pc_str2 == NULL)) {
    return 0UL;
  }
#endif

  return memcmp32(pc_str1, pc_str2, strlen32(pc_str1));
}

/* ================== Searching ====================== */
// TODO: fill averything here
// memchr
// strchr


/* ==================== Other ======================== */

/*
 * @brief Fill block of memory
 * @param *pv_dst - Pointer to the block of memory to fill
 * @param *ul_val - Pattern to be set
 * @param x_len - Number of bytes to be set to the Pattern
 * @retval destination buffer pointer
 */
_STRING32_LIB_OPTIMIZE_ATTR
void* memset32(void* pv_dst, uint32_t ul_val, size_t x_len)
{
#ifdef _STRING32_LIB_OPTIMIZE_NULL_CHECK
  if (pv_dst == NULL) {
    return NULL;
  }
#endif

  uint32_t* pul_dst = (uint32_t*) pv_dst;

  if (ul_val != 0) {
    ul_val |= ((ul_val << 8) & 0x0000FF00);
    ul_val |= ((ul_val << 16) & 0x00FF0000);
    ul_val |= ((ul_val << 24) & 0xFF000000);
  }

  while (x_len >= sizeof(uint32_t)) {
    *pul_dst = ul_val;

    ++pul_dst;
    x_len -= sizeof(uint32_t);
  }

  uint8_t* puc_dst = (uint8_t*) pul_dst;

  while (x_len--) {
    *puc_dst = (uint8_t) ul_val;
    ++puc_dst;
  }

  return pv_dst;
}

/*
 * @brief Get string length
 * @param *pv_src - C string
 * @retval the length of string
 */
_STRING32_LIB_OPTIMIZE_ATTR
size_t strlen32(void const* pv_src)
{
#ifdef _STRING32_LIB_OPTIMIZE_NULL_CHECK
  if (pv_src == NULL) {
    return 0UL;
  }
#endif

  uint32_t const *pul_src = (uint32_t const*) pv_src;
  uint32_t ul_word_chunk = *pul_src;
  uint32_t ul_offset = 0UL;

  // Idea based on concept when we trying to
  // discover '\0' at end of the sting splitted to word chunks
  do {
    if((ul_word_chunk & 0x000000FF) == 0UL) {
      ul_offset = 0UL;
      goto STRLEN32_DONE_END;
    } else if((ul_word_chunk & 0x0000FF00) == 0UL) {
      ul_offset = 1UL;
      goto STRLEN32_DONE_END;
    } else if((ul_word_chunk & 0x00FF0000) == 0UL) {
      ul_offset = 2UL;
      goto STRLEN32_DONE_END;
    } else if((ul_word_chunk & 0xFF000000) == 0UL) {
      ul_offset = 3UL;
      goto STRLEN32_DONE_END;
    }

    ++pul_src;
    ul_word_chunk = *pul_src;
  } while (1UL);

  STRLEN32_DONE_END:

  return (size_t)((const uint8_t *) pul_src - (const uint8_t *) pv_src + ul_offset);
}
