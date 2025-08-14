#include "main.h"
#include <stdio.h>

typedef void (*func_ptr)(void);

#define APPLICATION_ADDRESS   (0x08008000UL)
#define SRAM_START            (0x20000000UL)
#define SRAM_END              (0x20030000UL) // F407: 192KB SRAM
#define EMPTY_WORD            (0xFFFFFFFFUL)


void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // PD12..PD15 as push-pull outputs (Discovery LEDs)
  GPIO_InitStruct.Pin   = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
}




static void delay_ms(uint32_t ms) {
    // crude blocking delay; adjust for your SysClk if needed
    for (uint32_t i = 0; i < (ms * 12000U); ++i) { __NOP(); }
}

static void boot_led_toggle(void)  { HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14); } // red
static void app_led_toggle(void)   { HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12); } // green

// optional: turn off all used LEDs before jump
static void leds_off(void) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
}

static uint8_t application_is_valid(uint32_t app_base) {
    uint32_t sp = *((uint32_t*)app_base);
    uint32_t reset = *(((uint32_t*)app_base) + 1U);

    if (sp == EMPTY_WORD || reset == EMPTY_WORD) return 0;
    if (sp < SRAM_START || sp > (SRAM_END - 1U)) return 0; // SP must point into SRAM
    if ((reset & 0xFF000000U) != 0x08000000U) return 0;    // reset handler typically in flash
    return 1;
}

static void jump_to_application(uint32_t app_base) {
    uint32_t app_sp    = *((uint32_t*)app_base);
    uint32_t app_entry = *(((uint32_t*)app_base) + 1U);
    func_ptr app_reset = (func_ptr)app_entry;

    __disable_irq();

    // stop SysTick
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    // disable all NVIC interrupts
    for (uint32_t i = 0; i < 8; ++i) {
        NVIC->ICER[i] = 0xFFFFFFFFUL;
        NVIC->ICPR[i] = 0xFFFFFFFFUL;
    }

    HAL_RCC_DeInit();
    HAL_DeInit();

    leds_off();

    // relocate vector table to the application's vector table
    SCB->VTOR = app_base;

    // set MSP to application's stack pointer
    __set_MSP(app_sp);

    __enable_irq();

    app_reset(); // never returns
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();     // make sure GPIOD clock + pins are configured

    // bootloader indication: slow blink on PD14
    for (int i = 0; i < 6; ++i) {
        boot_led_toggle();
        delay_ms(250);
    }

    if (application_is_valid(APPLICATION_ADDRESS)) {
        // quick “handoff” indication (blink PD12 twice)
        for (int i = 0; i < 2; ++i) { app_led_toggle(); delay_ms(80); }
        jump_to_application(APPLICATION_ADDRESS);
    }

    // if no valid app, stay in bootloader with slow blink
    while (1) {
        boot_led_toggle(); // PD14
        delay_ms(500);
        // … put your comms/USB/UART update handler here if needed
    }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
