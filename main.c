
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c ECE330L Lab 7
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2S_HandleTypeDef hi2s3;
SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim7;

uint16_t led_patterns[] = {0x000F, 0x00F0, 0x0F00, 0xF000};
uint16_t led_mask = 0x0001;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void delay_ms(uint32_t ms);

/* GPIO Initialization */
void GPIO_Init(void) {
    __HAL_RCC_GPIOD_CLK_ENABLE(); // Enable Port D (LEDs)
    __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable Port C (Switches)

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure Port D (PD0-PD15) as OUTPUT for LEDs
    GPIO_InitStruct.Pin = GPIO_PIN_All;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    // Configure Port C (PC0-PC3, PC10-PC11, PC12-PC15) as INPUT for switches
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                          GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |
                          GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/* Delay Function */
void delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}

/* Part 1: Grouped LEDs */
void LED_Part1(void) {
    int i = 0;
    while (1) {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_All, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, led_patterns[i], GPIO_PIN_SET);
        delay_ms(500);
        i = (i + 1) % 4;
    }
}

/* Part 2: Scanning LEDs */
void LED_Part2(void) {
    while (1) {
        for (int i = 0; i < 16; i++) {
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_All, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOD, led_mask << i, GPIO_PIN_SET);
            delay_ms(500);
        }
    }
}

/* Part 3: Read Switches and Display on LEDs */
void LED_Part3(void) {
    while (1) {
        uint16_t switch_states = GPIOC->IDR;
        switch_states = __RBIT(switch_states) >> 16;
        GPIOD->ODR = switch_states;
        delay_ms(100);
    }
}

/* Main */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();

    while (1) {
        // Uncomment one at a time to test each part
        LED_Part1();
        // LED_Part2();
        // LED_Part3();
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    __disable_irq();
    while (1) {}
}
