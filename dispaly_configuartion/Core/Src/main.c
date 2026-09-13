#include "main.h"
#include <string.h>

I2C_HandleTypeDef hi2c1;

#define OLED_ADDR       (0x3C << 1)

/* Function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

void OLED_WriteCommand(uint8_t command);
void OLED_WriteData(uint8_t *data, uint16_t size);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_SetCursor(uint8_t page, uint8_t column);
void OLED_DrawChar(char c);
void OLED_DrawString(char *str);
void Error_Handler(void);


/* =========================================================
   OLED COMMAND
   ========================================================= */

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


/* =========================================================
   OLED DATA
   ========================================================= */

void OLED_WriteData(uint8_t *data, uint16_t size)
{
    uint8_t buffer[129];

    if (size > 128)
        size = 128;

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


/* =========================================================
   OLED INITIALIZATION
   ========================================================= */

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

    HAL_Delay(100);
}


/* =========================================================
   OLED CLEAR
   ========================================================= */

void OLED_Clear(void)
{
    uint8_t zero[128];

    for (uint16_t i = 0; i < 128; i++)
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


/* =========================================================
   OLED CURSOR
   ========================================================= */

void OLED_SetCursor(uint8_t page, uint8_t column)
{
    OLED_WriteCommand(0xB0 + page);

    OLED_WriteCommand(
        0x00 | (column & 0x0F)
    );

    OLED_WriteCommand(
        0x10 | ((column >> 4) & 0x0F)
    );
}


/* =========================================================
   5x7 FONT
   ========================================================= */

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
        uint8_t index = (uint8_t)(c - 'A' + 1);

        OLED_WriteData(
            (uint8_t *)font[index],
            5
        );

        uint8_t space = 0x00;

        OLED_WriteData(
            &space,
            1
        );
    }
    else if (c == ' ')
    {
        uint8_t space[6] =
        {
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00
        };

        OLED_WriteData(space, 6);
    }
}


/* =========================================================
   OLED STRING
   ========================================================= */

void OLED_DrawString(char *str)
{
    while (*str)
    {
        OLED_DrawChar(*str);
        str++;
    }
}


/* =========================================================
   MAIN
   ========================================================= */

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();

    MX_I2C1_Init();

    HAL_Delay(100);

    OLED_Init();

    OLED_Clear();


    /* OLED DISPLAY */

    OLED_SetCursor(0, 15);
    OLED_DrawString("WELCOME");

    OLED_SetCursor(2, 15);
    OLED_DrawString("TO JAPAN");

    OLED_SetCursor(4, 5);
    OLED_DrawString("STM32F446RE");

    OLED_SetCursor(6, 20);
    OLED_DrawString("ARIGATO");


    /* MAIN LOOP */

    while (1)
    {
        /* PC8 LED ON */

        HAL_GPIO_WritePin(
            GPIOC,
            GPIO_PIN_8,
            GPIO_PIN_SET
        );

        HAL_Delay(500);


        /* PC8 LED OFF */

        HAL_GPIO_WritePin(
            GPIOC,
            GPIO_PIN_8,
            GPIO_PIN_RESET
        );

        HAL_Delay(500);
    }
}


/* =========================================================
   SYSTEM CLOCK
   ========================================================= */

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(
        PWR_REGULATOR_VOLTAGE_SCALE3
    );


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
            FLASH_LATENCY_0
        ) != HAL_OK)
    {
        Error_Handler();
    }
}


/* =========================================================
   I2C1 INITIALIZATION
   ========================================================= */

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


/* =========================================================
   GPIO INITIALIZATION
   ========================================================= */

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};


    /* Enable GPIOC clock */

    __HAL_RCC_GPIOC_CLK_ENABLE();


    /* PC8 LOW initially */

    HAL_GPIO_WritePin(
        GPIOC,
        GPIO_PIN_8,
        GPIO_PIN_RESET
    );


    /* PC8 OUTPUT */

    GPIO_InitStruct.Pin =
        GPIO_PIN_8;

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


/* =========================================================
   ERROR HANDLER
   ========================================================= */

void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}


#ifdef USE_FULL_ASSERT

void assert_failed(
    uint8_t *file,
    uint32_t line
)
{
}

#endif
