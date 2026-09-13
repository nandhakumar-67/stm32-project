/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : STM32F446RE I2C OLED Test
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* Private defines -----------------------------------------------------------*/
#define OLED_ADDR       (0x3C << 1)

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

/* OLED functions */
void OLED_WriteCommand(uint8_t command);
void OLED_WriteData(uint8_t *data, uint16_t size);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_SetCursor(uint8_t page, uint8_t column);
void OLED_DrawChar(char c);
void OLED_DrawString(char *str);

/* USER CODE BEGIN 0 */

/* Send command to OLED */
void OLED_WriteCommand(uint8_t command)
{
    uint8_t data[2];

    data[0] = 0x00;
    data[1] = command;

    HAL_I2C_Master_Transmit(
        &hi2c1,
        OLED_ADDR,
        data,
        2,
        HAL_MAX_DELAY
    );
}


/* Send data to OLED */
void OLED_WriteData(uint8_t *data, uint16_t size)
{
    uint8_t buffer[129];

    buffer[0] = 0x40;

    for (uint16_t i = 0; i < size; i++)
    {
        buffer[i + 1] = data[i];
    }

    HAL_I2C_Master_Transmit(
        &hi2c1,
        OLED_ADDR,
        buffer,
        size + 1,
        HAL_MAX_DELAY
    );
}


/* OLED initialization */
void OLED_Init(void)
{
    HAL_Delay(100);

    OLED_WriteCommand(0xAE); // Display OFF

    OLED_WriteCommand(0xD5);
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xA8);
    OLED_WriteCommand(0x3F);

    OLED_WriteCommand(0xD3);
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x40);

    OLED_WriteCommand(0x8D);
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0x20);
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0xA1);

    OLED_WriteCommand(0xC8);

    OLED_WriteCommand(0xDA);
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81);
    OLED_WriteCommand(0x7F);

    OLED_WriteCommand(0xD9);
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB);
    OLED_WriteCommand(0x40);

    OLED_WriteCommand(0xA4);

    OLED_WriteCommand(0xA6);

    OLED_WriteCommand(0xAF); // Display ON
}


/* Clear OLED */
void OLED_Clear(void)
{
    uint8_t zero[128];

    for (uint8_t i = 0; i < 128; i++)
    {
        zero[i] = 0x00;
    }

    for (uint8_t page = 0; page < 8; page++)
    {
        OLED_WriteCommand(0xB0 + page);
        OLED_WriteCommand(0x00);
        OLED_WriteCommand(0x10);

        OLED_WriteData(zero, 128);
    }
}


/* Set OLED cursor */
void OLED_SetCursor(uint8_t page, uint8_t column)
{
    OLED_WriteCommand(0xB0 + page);

    OLED_WriteCommand(0x00 | (column & 0x0F));

    OLED_WriteCommand(0x10 | ((column >> 4) & 0x0F));
}


/* Simple 5x7 character drawing */
void OLED_DrawChar(char c)
{
    static const uint8_t font[][5] =
    {
        /* SPACE */
        {0x00,0x00,0x00,0x00,0x00},

        /* A */
        {0x7E,0x11,0x11,0x11,0x7E},

        /* B */
        {0x7F,0x49,0x49,0x49,0x36},

        /* C */
        {0x3E,0x41,0x41,0x41,0x22},

        /* D */
        {0x7F,0x41,0x41,0x22,0x1C},

        /* E */
        {0x7F,0x49,0x49,0x49,0x41},

        /* F */
        {0x7F,0x09,0x09,0x09,0x01},

        /* G */
        {0x3E,0x41,0x49,0x49,0x7A},

        /* H */
        {0x7F,0x08,0x08,0x08,0x7F},

        /* I */
        {0x00,0x41,0x7F,0x41,0x00},

        /* J */
        {0x20,0x40,0x41,0x3F,0x01},

        /* K */
        {0x7F,0x08,0x14,0x22,0x41},

        /* L */
        {0x7F,0x40,0x40,0x40,0x40},

        /* M */
        {0x7F,0x02,0x0C,0x02,0x7F},

        /* N */
        {0x7F,0x04,0x08,0x10,0x7F},

        /* O */
        {0x3E,0x41,0x41,0x41,0x3E},

        /* P */
        {0x7F,0x09,0x09,0x09,0x06},

        /* Q */
        {0x3E,0x41,0x51,0x21,0x5E},

        /* R */
        {0x7F,0x09,0x19,0x29,0x46},

        /* S */
        {0x46,0x49,0x49,0x49,0x31},

        /* T */
        {0x01,0x01,0x7F,0x01,0x01},

        /* U */
        {0x3F,0x40,0x40,0x40,0x3F},

        /* V */
        {0x1F,0x20,0x40,0x20,0x1F},

        /* W */
        {0x7F,0x20,0x18,0x20,0x7F},

        /* X */
        {0x63,0x14,0x08,0x14,0x63},

        /* Y */
        {0x03,0x04,0x78,0x04,0x03},

        /* Z */
        {0x61,0x51,0x49,0x45,0x43}
    };

    if (c >= 'A' && c <= 'Z')
    {
        uint8_t index = c - 'A' + 1;

        OLED_WriteData((uint8_t *)font[index], 5);

        uint8_t space = 0x00;
        OLED_WriteData(&space, 1);
    }
    else
    {
        uint8_t space = 0x00;
        OLED_WriteData(&space, 1);
    }
}


/* Draw string */
void OLED_DrawString(char *str)
{
    while (*str)
    {
        OLED_DrawChar(*str);
        str++;
    }
}

/* USER CODE END 0 */


/**
  * @brief  The application entry point.
  */
int main(void)
{
    /* Reset peripherals and initialize Flash and Systick */
    HAL_Init();

    /* Configure system clock */
    SystemClock_Config();

    /* Initialize GPIO */
    MX_GPIO_Init();

    /* Initialize I2C */
    MX_I2C1_Init();

    /* Initialize OLED */
    OLED_Init();

    /* Clear OLED */
    OLED_Clear();
    /* Clear OLED */
    OLED_Clear();

    /* Display message */
    OLED_SetCursor(0, 15);
    OLED_DrawString("WELCOME");

    OLED_SetCursor(2, 15);
    OLED_DrawString("TO JAPAN");

    OLED_SetCursor(4, 5);
    OLED_DrawString("STM32F446RE");

    OLED_SetCursor(6, 20);
    OLED_DrawString("ARIGATO");

    /* Display first line */


    while (1)
    {
        /* LED ON */
        HAL_GPIO_WritePin(
            GPIOC,
            GPIO_PIN_8,
            GPIO_PIN_SET
        );

        HAL_Delay(500);

        /* LED OFF */
        HAL_GPIO_WritePin(
            GPIOC,
            GPIO_PIN_8,
            GPIO_PIN_RESET
        );

        HAL_Delay(500);
    }
}


/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Power clock enable */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Voltage scaling */
    __HAL_PWR_VOLTAGESCALING_CONFIG(
        PWR_REGULATOR_VOLTAGE_SCALE3
    );

    /* HSI oscillator */
    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_HSI;

    RCC_OscInitStruct.HSIState =
        RCC_HSI_ON;

    RCC_OscInitStruct.HSICalibrationValue =
        RCC_HSICALIBRATION_DEFAULT;

    RCC_OscInitStruct.PLL.PLLState =
        RCC_PLL_NONE;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Clock configuration */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource =
        RCC_SYSCLKSOURCE_HSI;

    RCC_ClkInitStruct.AHBCLKDivider =
        RCC_SYSCLK_DIV1;

    RCC_ClkInitStruct.APB1CLKDivider =
        RCC_HCLK_DIV1;

    RCC_ClkInitStruct.APB2CLKDivider =
        RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(
            &RCC_ClkInitStruct,
            FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}


/**
  * @brief I2C1 Initialization Function
  */
static void MX_I2C1_Init(void)
{
    hi2c1.Instance = I2C1;

    hi2c1.Init.ClockSpeed = 100000;

    hi2c1.Init.DutyCycle =
        I2C_DUTYCYCLE_2;

    hi2c1.Init.OwnAddress1 = 0;

    hi2c1.Init.AddressingMode =
        I2C_ADDRESSINGMODE_7BIT;

    hi2c1.Init.DualAddressMode =
        I2C_DUALADDRESS_DISABLE;

    hi2c1.Init.OwnAddress2 = 0;

    hi2c1.Init.GeneralCallMode =
        I2C_GENERALCALL_DISABLE;

    hi2c1.Init.NoStretchMode =
        I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
}


/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO clocks */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* PC8 initial LOW */
    HAL_GPIO_WritePin(
        GPIOC,
        GPIO_PIN_8,
        GPIO_PIN_RESET
    );

    /* PC8 output */
    GPIO_InitStruct.Pin = GPIO_PIN_8;

    GPIO_InitStruct.Mode =
        GPIO_MODE_OUTPUT_PP;

    GPIO_InitStruct.Pull =
        GPIO_NOPULL;

    GPIO_InitStruct.Speed =
        GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(
        GPIOC,
        &GPIO_InitStruct
    );
}


/**
  * @brief Error Handler
  */
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
    /* User can add implementation here */
}

#endif
