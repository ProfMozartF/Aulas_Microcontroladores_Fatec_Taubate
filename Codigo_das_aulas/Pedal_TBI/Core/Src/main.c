#include "main.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

/* Private variables ---------------------------------------------------------*/
float Fator_Erro = 0.98862;
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart1;
uint16_t adc_buffer[4]; // Buffer para 4 canais (0-3)
uint16_t pedal1Raw;
uint16_t pedal2Raw;
uint16_t tps1Raw;
uint16_t tps2Raw;
uint16_t milivolt_pedal1;
uint16_t milivolt_pedal2;
uint16_t milivolt_tps1;
uint16_t milivolt_tps2;
float porcPedal1;
float porcPedal2;
float porcTPS1;
float porcTPS2;
uint16_t duty;
uint8_t contadorUART=0;
char msg[50];
typedef enum {
    None = 0,
    Pedal1,
	Pedal2,
    TPS1,
	TPS2
} SensorType_t;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);

uint16_t mediaADC(uint8_t adcPino, uint8_t numAmostras);
float valorPorcento (uint16_t mVoltSensor, SensorType_t sensorConv);
uint16_t validaVolt(uint16_t mVoltLido, uint16_t lowLim, uint16_t upperLim);

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	//HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

	// Tempo medido para essa interrupção +- 100us com o clock atual
	//1/FOSC(ADC)xN_ciclosxN_conv
}

int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  HAL_TIM_Base_Start_IT(&htim2);				// Inicializa Interrupção Timer2
  // Inicia a conversão com DMA
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, adc_buffer, 4); //Lembre de no IOC marcar como circular
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

  duty = 200;


  while (1)
  {
	  //Aquisita o valor RAW do ADC e tira média
	  pedal1Raw = mediaADC(0, 100);
	  pedal2Raw = mediaADC(1, 100);
	  tps1Raw = mediaADC(2, 100);
	  tps2Raw =  mediaADC(3, 100);

	  //Transforma ADC raw em milivolt
	  milivolt_pedal1 = (pedal1Raw*1.22099)*Fator_Erro;
	  milivolt_pedal2 = (pedal2Raw*1.22099)*Fator_Erro;
	  milivolt_tps1 = (tps1Raw*1.22099)*Fator_Erro;
	  milivolt_tps2 = (tps2Raw*1.22099)*Fator_Erro;

	  // Verifica out of range
	  milivolt_pedal1 = validaVolt(milivolt_pedal1, 500, 4500);


	  //Chama função para calcular porcentagem
	  porcPedal1 = valorPorcento (milivolt_pedal1, Pedal1);
	  porcPedal2 = valorPorcento (milivolt_pedal2, Pedal2);
	  porcTPS1 = valorPorcento (milivolt_tps1, TPS1);
	  porcTPS2 = valorPorcento (milivolt_tps2, TPS2);

	  //Atualiza duty
	  duty = porcPedal1*10.25;//calcula duty cicle com base no pedal 1
	  TIM2->CCR1 = duty;

	  //Envia dados na UART
	  sprintf(msg, "mVolt Pedal_1= %d\r\n", milivolt_pedal1);
	  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

	  sprintf(msg, "mVolt Pedal_2 = %d\r\n", milivolt_pedal2);
	  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

	  sprintf(msg, "mVolt TPS_1 = %d\r\n", milivolt_tps1);
	  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

	  sprintf(msg, "mVolt TPS_2 = %d\r\n", milivolt_tps2);
	  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

	  sprintf(msg, "Porc P1 = %.2f\r\n", porcPedal1);
	  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

	  sprintf(msg, "Porc P2 = %.2f\r\n", porcPedal2);
	  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

	  sprintf(msg, "Porc TPS1 = %.2f\r\n", porcTPS1);
	  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

	  sprintf(msg, "Porc TPS2 = %.2f\r\n", porcTPS2);
	  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

	  HAL_Delay(100);

  }//FIM do While True

}//FIM DO MAIN

//*************************************************************************************************************************************
uint16_t mediaADC(uint8_t adcPino, uint8_t numAmostras)
{
	uint32_t soma = 0;
	uint8_t contador = 0;
	uint16_t mediaADC = 0;
	while(contador<numAmostras)
	{
		soma+=adc_buffer[adcPino];
		contador++;

		uint16_t contDelay_us = 0;
		while(contDelay_us<14400) //cria um delay de 200 microsegundos entre cada nova amostra
		{
			contDelay_us++;
		}

	}
	mediaADC = soma/numAmostras;
	return(mediaADC);
}
//*************************************************************************************************************************************
float valorPorcento (uint16_t mVoltSensor, SensorType_t sensorConv)
{
	float porc_temp=0.0;
	switch (sensorConv)
	    {
	        case None:
	            break;

	        case Pedal1:
	        	porc_temp = (0.0311 * mVoltSensor) - 24.11;
	            break;

	        case Pedal2:
	        	porc_temp = (0.0617 * mVoltSensor) - 23.45;
	            break;

	        case TPS1:
	        	porc_temp = (-0.0240 * mVoltSensor) + 108.1731;
	            break;

	        case TPS2:
	        	porc_temp = (0.0240 * mVoltSensor) - 12.2596;
	            break;

	        default:
	            break;
	    }
	return (porc_temp);
}
//*************************************************************************************************************************************
uint16_t validaVolt(uint16_t mVoltLido, uint16_t lowLim, uint16_t upperLim)
{
	uint16_t mVoltValido=0;
	if (mVoltLido>upperLim){mVoltValido = upperLim;}
	else if (mVoltLido<lowLim){mVoltValido = lowLim;}
	else {mVoltValido = mVoltLido;}
	return(mVoltValido);
}
//*************************************************************************************************************************************

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 4;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 6;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1025;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
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
