# string32.h
***

#### What's that for ?
While x86-64 get most advantages of maximum optimisations like AVX instructions,
world of Microcontrollers have nothing, but few bit tricks.

Even standart <string.h> library for ARM Cortex Microcontrollers can't provide great performance, as in most compilers it's implemented on bytes operations even for 32 bits Microcontrollers!

That's a huge lost of precious perfomance on Embedded systems!

This is why this library must nicely fit on all Microcontrollers from Cortex-M0 to Cortex-M7 and many others!


#### Tested on:
- STM32F1;
- STM32F4;
- ~RP2040 (Pi Pico)~ - tests in progress.


> ## :exclamation: ATTENTION! :exclamation:
>  * This project is still unstable and in develop! :beetle:
>  * Any changes are possible at any time! 
