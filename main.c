/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();

  while (1)
  {
    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_0);

    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_0);
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_1);
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_1);
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_2);
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_2);
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_3);
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_3);
    HAL_Delay(500);


  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK |
                                RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA,
                    GPIO_PIN_0 |
                    GPIO_PIN_1 |
                    GPIO_PIN_2 |
                    GPIO_PIN_3,
                    GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_0 |
                        GPIO_PIN_1 |
                        GPIO_PIN_2 |
                        GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();

  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
