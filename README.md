# Bootloader_STM32

This bootloader example can jump to 2 different applications.

The controller use is STM32F407 
The FLASH memory  is splitted into multiple partitions.

The first partition (12KB) is for the bootloader.
/* Memories definition */
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 20K
  FLASH    (rx)    : ORIGIN = 0x08000000 ,   LENGTH = 12K /*20K*/
}

The second partition is for the Application1 (16KB).
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 20K
  FLASH    (rx)    : ORIGIN = 0x08008000,   LENGTH = 16K /*22K*/
}

The third partion is for the Application2 (16KB)
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 20K
  FLASH    (rx)    : ORIGIN = 0x08010000,   LENGTH = 16K /*22K*/
}


/* Bootloader */
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000, LENGTH = 20K
  FLASH  (rx)     : ORIGIN = 0x08000000, LENGTH = 12K
}

/* Application 1 */
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000, LENGTH = 20K
  FLASH  (rx)     : ORIGIN = 0x08008000, LENGTH = 16K
}

/* Application 2 */
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000, LENGTH = 20K
  FLASH  (rx)     : ORIGIN = 0x08010000, LENGTH = 16K
}

