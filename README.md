# Bootloader_STM32F407

This project demonstrates a simple STM32F407 bootloader that validates and jumps to an application located at a different flash address.

---

## Overview
- **MCU:** STM32F407
- **Bootloader address:** `0x08000000`
- **Application address:** `0x08008000`
- **Function:** Bootloader checks if the application is valid by reading its vector table (MSP and Reset_Handler) and, if valid, jumps to it after disabling peripherals and interrupts.

---

## Memory Layout

### Bootloader Linker Script
```ld
MEMORY
{
  RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
  FLASH (rx): ORIGIN = 0x8000000, LENGTH = 1024K
}
```
### Application Linker Script
```ld

MEMORY
{
  RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
  FLASH (rx): ORIGIN = 0x8008000, LENGTH = 64K
}
```
