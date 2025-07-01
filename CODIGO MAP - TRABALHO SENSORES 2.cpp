#include "stm32f1xx_hal.h"
#include <stdio.h>

// Defina os valores da sua calibração:
#define v1 100
#define v2 900
#define v3 0
#define v4 100

ADC_HandleTypeDef hadc1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);

float mapValue(int x, int in_min, int in_max, int out_min, int out_max) {
    return (float)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Função para ler o ADC (canal 0, por exemplo)
int analogRead(void) {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    int val = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return val;
}

// Redireciona printf para UART (necessário implementar para ver no serial)
int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

// UART handle para printf (configure conforme seu projeto)
UART_HandleTypeDef huart1;

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();

    // Inicialize UART1 para printf, implemente MX_USART1_UART_Init() conforme seu projeto
    MX_USART1_UART_Init();

    while (1) {
        int leitura = analogRead();
        float tensao = leitura * (3.3f / 4095.0f); // Para STM32 ADC 12 bits, Vref=3.3V

        float pressao_kPa = mapValue(leitura, v1, v2, v3, v4);

        printf("Tensão: %.2f V | Pressão aproximada: %.2f kPa\r\n", tensao, pressao_kPa);

        HAL_Delay(1000);
    }
}

// -- Funções de inicialização (geradas pelo STM32CubeMX normalmente) --

void SystemClock_Config(void)
{
    // ... configuração gerada pelo CubeMX ...
}

static void MX_GPIO_Init(void)
{
    // ... configuração gerada pelo CubeMX ...
}

static void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    __HAL_RCC_ADC1_CLK_ENABLE();
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    HAL_ADC_Init(&hadc1);

    sConfig.Channel = ADC_CHANNEL_0; // Use o canal desejado
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

// Implemente a função de inicialização da UART conforme seu CubeMX
void MX_USART1_UART_Init(void)
{
