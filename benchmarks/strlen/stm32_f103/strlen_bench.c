/*
 * Description:
 *  Test benchmark for strlen/strlen32
 *
 * ATTENTION!
 * Result values in clocks may differ up to +/-6 clocks !
 *
 * Author: 
 *  Alexandr Antonov (@Bismuth208)
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

#include <stdint.h>
#include <string.h>
#include <string32.h>

//--------------------------------------------//
typedef struct {
  uint32_t DWT_LAR;        // Lock Access Register  | 0xE0000FB0
  uint32_t DWT_LSR;        // Lock Status Register  | 0xE0000FB4

  uint32_t DWT_UNKNOWN[18];

  uint32_t DWT_CTRL;       // Control Register      | 0xE0001000
  uint32_t DWT_CYCCNT;     // Cycle Count Register  | 0xE0001004
  uint32_t DWT_CPICNT;     // CPI Count Register
  uint32_t DWT_EXCCNT;     // Exception Overhead Count Register
  uint32_t DWT_SLEEPCNT;   // Sleep Count Register
  uint32_t DWT_LSUCNT;     // LSU Count Register
  uint32_t DWT_FOLDCNT;    // Folded-instruction Count Register
  uint32_t DWT_PCSR;       // Program Counter Sample Register
  uint32_t DWT_COMP0;      // Comparator Register 0
  uint32_t DWT_MASK0;      // Mask Register 0
  uint32_t DWT_FUNCTION0;  // Function Register 0
  uint32_t DWT_COMP1;      // Comparator Register 1
  uint32_t DWT_MASK1;      // Mask Register 1
  uint32_t DWT_FUNCTION1;  // Function Register 1
  uint32_t DWT_COMP2;      // Comparator Register 2
  uint32_t DWT_MASK2;      // Mask Register 2
  uint32_t DWT_FUNCTION2;  // Function Register 2
  uint32_t DWT_COMP3;      // Comparator Register 3
  uint32_t DWT_MASK3;      // Mask Register 3
  uint32_t DWT_FUNCTION3;  // Function Register 3
  // Some of the registers is not added here
  // From 0xE0001FD0 -> 0xE0001FFC
} DWT_TypeDef;  // 0xE0000FB0 -> 0xE0000FB4

/*
 * ITM registers to perform clocks count
 * For STM32 this registers are the same (mostly ?).
 */
#define SCB_DEMCR   *(volatile uint32_t* )0xE000EDFC // CoreDebug
#define DWT         ((DWT_TypeDef*) ((volatile uint32_t) 0xE0000FB0))

const uint32_t DWT_LAR_MAGIC = 0xC5ACCE55;
//--------------------------------------------//

__attribute__ ((optimize("O0")))
uint32_t ul_benchmark_32(const void* pv_data)
{
  volatile uint32_t ul_res = 0UL;
  uint32_t ul_res_total = 0UL;

  DWT->DWT_CYCCNT = 0UL;
  size_t x_len = strlen32(pv_data);

  ul_res = DWT->DWT_CYCCNT;
  ul_res_total += ul_res;
  DWT->DWT_CYCCNT = 0UL;

  return ul_res_total;
}

__attribute__ ((optimize("O0")))
uint32_t ul_benchmark_std(const void* pv_data)
{
  volatile uint32_t ul_res = 0UL;
  uint32_t ul_res_total = 0UL;

  DWT->DWT_CYCCNT = 0UL;
  size_t x_len = strlen(pv_data);

  ul_res = DWT->DWT_CYCCNT;
  ul_res_total += ul_res;
  DWT->DWT_CYCCNT = 0UL;

  return ul_res_total;
}

//--------------------------------------------//
const uint8_t uc_buff_test_128b[129] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis dictum, risus id congue malesuada, nibh urna feugiat velit ligula.\0";
const uint8_t uc_buff_test_16b[17] = "Lorem ipsum leo.\0";
const uint8_t uc_buff_test_15b[16] = "Lorem ipsum ac.\0";
const uint8_t uc_buff_test_14b[15] = "Lorem aliquam.\0";
const uint8_t uc_buff_test_13b[14] = "Lorem lectus.\0";

/*
 * @brief Enables debug counter to mesure wasted clocks
 * @retval none
 */
void init_dwt(void)
{
  DWT->DWT_LAR = DWT_LAR_MAGIC;  // unlock access to DWT (ITM, etc.)registers
  SCB_DEMCR |= 0x01000000;       // enable trace

  DWT->DWT_CTRL |= 1;         // enable the counter
  DWT->DWT_CYCCNT = 0;        // reset the counter
}

int main(void)
{
  __disable_irq(); // __ASM volatile ("cpsid i");

  init_dwt();

  uint32_t ul_total_clocks = 0UL;

  // strlen:
  //  ---------------------------------------------------
  // |         |  128b  |  16b   |  15b  |  14b  |  13b  |
  // |---------|--------|--------|-------|-------|-------|
  // |         |  1433  |  201   |  190  |  179  |  168  |
  // |---------|-----------------------------------------|
  // | Total:  |        2171  clocks                     |
  //  ---------------------------------------------------
  ul_total_clocks += ul_benchmark_std(&uc_buff_test_128b[0]);
  ul_total_clocks += ul_benchmark_std(&uc_buff_test_16b[0]);
  ul_total_clocks += ul_benchmark_std(&uc_buff_test_15b[0]);
  ul_total_clocks += ul_benchmark_std(&uc_buff_test_14b[0]);
  ul_total_clocks += ul_benchmark_std(&uc_buff_test_13b[0]);


  // strlen32:
  //  -------------------------------------------------
  // |         | 128b  | 16b   | 15b   | 14b   | 13b   |
  // |---------|-------|-------|-------|-------|-------|
  // |         |  865  |  137  | 130   | 123   | 120   |
  // |---------|---------------------------------------|
  // | Total:  |       1375  clocks                    |
  //  -------------------------------------------------
  ul_total_clocks = 0UL;
  ul_total_clocks += ul_benchmark_32(&uc_buff_test_128b[0]);
  ul_total_clocks += ul_benchmark_32(&uc_buff_test_16b[0]);
  ul_total_clocks += ul_benchmark_32(&uc_buff_test_15b[0]);
  ul_total_clocks += ul_benchmark_32(&uc_buff_test_14b[0]);
  ul_total_clocks += ul_benchmark_32(&uc_buff_test_13b[0]);

  for (;;) {
    __WFI();
  }

  return 0;
}
