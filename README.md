# Bootloader_STM32

This bootloader example can jump to 2 different applications.

The controller use is STM32F407 
The FLASH memory  is splitted into multiple partitions.

The first partition (12KB) is for the bootloader.
<img width="656" height="154" alt="image" src="https://github.com/user-attachments/assets/597cd3fc-fc60-4295-8f62-236983ae9295" />

The second partition is for the Application1 (16KB).
<img width="642" height="123" alt="image" src="https://github.com/user-attachments/assets/03015577-5550-479d-9fab-aa2be320a7cd" />

The third partion is for the Application2 (16KB)
![Uploading image.png…]()
