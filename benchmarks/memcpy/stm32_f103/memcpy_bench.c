/*
 * Description:
 *  Test benchmark for memcpy/memcpy32
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
uint32_t ul_benchmark_32(void* pv_data1, void* pv_data2, size_t x_size)
{
  volatile uint32_t ul_res = 0UL;
  uint32_t ul_res_total = 0UL;

  DWT->DWT_CYCCNT = 0UL;
  memcpy32(pv_data1, pv_data2, x_size);

  ul_res = DWT->DWT_CYCCNT;
  ul_res_total += ul_res;
  DWT->DWT_CYCCNT = 0UL;

  return ul_res_total;
}

__attribute__ ((optimize("O0")))
uint32_t ul_benchmark_std(void* pv_data1, void* pv_data2, size_t x_size)
{
  volatile uint32_t ul_res = 0UL;
  uint32_t ul_res_total = 0UL;

  DWT->DWT_CYCCNT = 0UL;
  memcpy(pv_data1, pv_data2, x_size);

  ul_res = DWT->DWT_CYCCNT;
  ul_res_total += ul_res;
  DWT->DWT_CYCCNT = 0UL;

  return ul_res_total;
}

//--------------------------------------------//
uint8_t uc_buff_test[128];
uint32_t ul_buff_test[32];

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

  // memcpy:
  //  ---------------------------------------------
  // |         |  128b  |  127b  |  126b  |  125b  |
  // |---------|--------|--------|--------|--------|
  // |         |  1435  |  1424  |  1413  |  1402  |
  // |---------|-----------------------------------|
  // | Total:  |       5674  clocks                |
  //  ---------------------------------------------
  ul_total_clocks += ul_benchmark_std(&uc_buff_test[0], &ul_buff_test[0], sizeof(uc_buff_test));
  ul_total_clocks += ul_benchmark_std(&uc_buff_test[0], &ul_buff_test[0], sizeof(uc_buff_test)-1);
  ul_total_clocks += ul_benchmark_std(&uc_buff_test[0], &ul_buff_test[0], sizeof(uc_buff_test)-2);
  ul_total_clocks += ul_benchmark_std(&uc_buff_test[0], &ul_buff_test[0], sizeof(uc_buff_test)-3);


  // memcpy32:
  //  -----------------------------------------
  // |         | 128b  | 127b  | 126b  | 125b  |
  // |---------|-------|-------|-------|-------|
  // |         |  396  |  409  |  398  |  387  |
  // |---------|-------------------------------|
  // | Total:  |        1590  clocks           |
  //  -----------------------------------------
  ul_total_clocks = 0UL;
  ul_total_clocks += ul_benchmark_32(&uc_buff_test[0], &ul_buff_test[0], sizeof(uc_buff_test));
  ul_total_clocks += ul_benchmark_32(&uc_buff_test[0], &ul_buff_test[0], sizeof(uc_buff_test)-1);
  ul_total_clocks += ul_benchmark_32(&uc_buff_test[0], &ul_buff_test[0], sizeof(uc_buff_test)-2);
  ul_total_clocks += ul_benchmark_32(&uc_buff_test[0], &ul_buff_test[0], sizeof(uc_buff_test)-3);

  for (;;) {
    __WFI();
  }

  return 0;
}
