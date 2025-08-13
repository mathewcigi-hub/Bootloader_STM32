# Bootloader_STM32

This bootloader example can jump to 2 different applications.

The controller use is STM32F407 
The FLASH memory  is splitted into multiple partitions.

Bootloader
```ld
MEMORY
{
  RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 20K
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 12K
}
```
Application 1
```ld

MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000, LENGTH = 20K
  FLASH  (rx)     : ORIGIN = 0x08008000, LENGTH = 16K
}
```
Application 2 
```ld
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000, LENGTH = 20K
  FLASH  (rx)     : ORIGIN = 0x08010000, LENGTH = 16K
}
```

