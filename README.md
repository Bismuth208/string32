# string32.h
***

#### What's that for ?
While x86-64 get most advantages of maximum optimisations like AVX instructions,
world of Microcontrollers have nothing, but few bit tricks.

Even standart <string.h> library for ARM Cortex Microcontrollers can't provide great performance, as in most compilers it's implemented on bytes operations even for 32bit Microcontrollers!

That's a huge lost of precious perfomance on Embedded systems!

This is why this library must nicely fit on all Microcontrollers from Cortex-M0 to Cortex-M7 and many others!


## Benchmarks
#### Tested on:
- STM32F1;
- STM32F4;
- ~RP2040 (Pi Pico)~ - tests in progress.

All avaliable benchamarks located in bencmarks folder.
Clocks counted using DWT register in debug mode.
Next numbers shows only results from STM32F1, but for others MCU results must be pretty much the same with 6-8 clocks difference.

***
### memset/memset32
Memset is tested for 2 different data types:
 - large buffer for 128 bytes;
 - small buffer for 16 bytes;

First case performed for best data type to achive maximum speed.
Second one, most common and worst case possible, when data type to be set is small.

Tested on 4 different patterns:
 - 0x00 is regular most spread uint8_t reset value;
 - 0xFF yet another common uint8_t reset value;
 - 0x1FF is rare uint16_t value;
 - 0x1FFFF is typical uint32_t value;

Also, to test different data length and type branching prediction, each pattern tested with next sizes:
 - 128/16 bytes for 4 bytes align (best case);
 - 127/15 bytes for 3 bytes non-align (worst case);
 - 126/14 bytes for 2 bytes non-align (meh case);
 - 124/13 bytes for 1 bytes non-align (ok case);

#### Clocks for 128 bytes buffer
memset (string.h):
  | pattern |  128b  |  127b  |  126b  |  125b  |
  | --- | ---  | ---  | ---  | ---  |
  | 0x00    |  1294  |  1284  |  1274  |  1264  |
  | 0xFF    |  1294  |  1284  |  1274  |  1264  |
  | 0x1FF   |  1294  |  1284  |  1274  |  1264  |
  | 0x1FFFF |  1298  |  1288  |  1278  |  1268  |

Total: 30776 clocks

memset32 (string32.h):
  | pattern |  128b  |  127b  |  126b  |  125b  |
  | --- | ---  | ---  | ---  | ---  |
  | 0x00    |  222  |  231  |  224  |  217  |
  | 0xFF    |  243  |  252  |  245  |  238  |
  | 0x1FF   |  236  |  245  |  238  |  231  |
  | 0x1FFFF |  240  |  249  |  242  |  235  |

Total: 3936 clocks

#### Clocks for 16 bytes buffer
memset (string.h):
  | pattern |  16b  |  15b  |  14b  |  13b  |
  | --- | ---  | ---  | ---  | ---  |
  | 0x00    |   180  |   170  |   160  |   150  |
  | 0xFF    |   180  |   170  |   160  |   150  |
  | 0x1FF   |   180  |   170  |   160  |   150  |
  | 0x1FFFF |   184  |   174  |   164  |   154  |

Total: 2656 clocks

memset32 (string32.h):
  | pattern |  16b  |  15b  |  14b  |  13b  |
  | --- | ---  | ---  | ---  | ---  |
  | 0x00    |   60  |   69  |   62  |   55  |
  | 0xFF    |   81  |   90  |   83  |   76  |
  | 0x1FF   |   74  |   83  |   76  |   69  |
  | 0x1FFFF |   78  |   87  |   80  |   73  |

Total: 1196 clocks

***
> ## :exclamation: ATTENTION! :exclamation:
>  * This project is still unstable and in develop! :beetle:
>  * Any changes are possible at any time! 
