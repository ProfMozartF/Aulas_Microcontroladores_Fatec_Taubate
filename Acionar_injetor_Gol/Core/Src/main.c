/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "string.h"
#include <math.h>

// --- Parâmetros do Motor ---
const float bore = 6.71;        // Diâmetro do cilindro (cm)
const float stroke = 7.06;     // Curso (cm)
const int cylinders = 4;       // Número de cilindros
const float volumetric_efficiency = 0.85; // Eficiência volumétrica (adimensional)

// --- Combustível ---
const float AFR = 14.7;         // Relação ar/combustível estequiométrica (adimensional)
const float fuel_density = 0.75; // Gasolina (g/cm³ ou g/mL)

// --- Injetor ---
const float base_fuel_pressure_bar_diff = 3.0;  // Pressão DIFERENCIAL de calibração (bar)
const float base_flow_rate_cc_min = 250.0;     // Vazão @ base_fuel_pressure_bar_diff (cc/min)
const float injector_dead_time_ms = 0.8;      // Tempo morto do injetor (ms) - VALOR DE EXEMPLO, OBTENHA O VALOR REAL DO SEU INJETOR!

// --- Sensores (simulados para este exemplo) ---
float MAP = 101.3;     // Pressão absoluta coletor (kPa)
float IAT = 25.0;      // Temperatura ar admissão (°C)
float current_fuel_pressure = 4.0; // Pressão real da linha de combustível (bar)

// --- Constantes Físicas ---
const float M_air = 28.97;     // Massa molar do ar (g/mol)
const float R_gas = 8.314;     // Constante universal dos gases (J/mol·K)
const float PI_VAL = 3.1415926535; // Definindo PI para clareza

/* Private variables ---------------------------------------------------------*/
float Fator_Erro = 0.98862;
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
UART_HandleTypeDef huart1;
//Variaveis globais
typedef enum {
    None = 0,
    Pedal1,
	Pedal2,
    PressMAP,
	TempMAP,
	TempAgua
} SensorType_t;
uint32_t adcBuffer[5];//NESSE VETOR VAO SER ARMAZENADAS AS CONVERSOES AD
/*
 adcBuffer[0] --> Temp. Ar
 adcBuffer[1] --> Pressão Ar (MAP)
 adcBuffer[2] --> Temp. Agua
 adcBuffer[3] --> Pedal pista 1
 adcBuffer[4] --> Pedal pista 2
 */
uint16_t tempoInjetor;
float tempo_de_injeção_calc; //resultado do tempo de injeção calculado
uint8_t denteRodaFonica;
uint32_t contDenteFonica;
uint32_t contDenteFonica_anterior;
float RPM;
uint16_t pedal1Raw;
uint16_t pedal2Raw;
uint16_t PressMAP_RAW;
uint16_t TempMAP_RAW;
int16_t TempAgua_RAW;
uint16_t milivolt_pedal1;
uint16_t milivolt_pedal2;
uint16_t milivolt_PressMAP;
uint16_t milivolt_TempMAP;
uint16_t milivolt_TempAgua;
float porcPedal1;
float porcPedal2;
float PressMAP_KPA;
float Temp_MAP;
float Temp_Agua;
char msg[100];
uint16_t cont_TempInj =0;
  float displacement_volume_per_cylinder_cm3 = 0.0;
  float total_engine_displacement_liters = 0.0;
  float final_injection_pulse_ms = 0.0;
  float TInj_corrigido;

// Estrutura para tabela de correção
typedef struct {
    float temp_c;    // Temperatura da água em °C
    float fator;     // Fator de correção
} CorrTWI_t;

// Tabela de correção TI x Temperatura Água
const CorrTWI_t tabela_corr_TW[] = {
    {-20.0f, 1.80f},
    {-10.0f, 1.60f},
    {  0.0f, 1.40f},
    { 10.0f, 1.20f},
    { 20.0f, 1.10f},
    { 30.0f, 1.05f},
    { 40.0f, 1.02f},
    { 50.0f, 1.00f},
    { 70.0f, 1.00f},
    { 90.0f, 0.98f}
};

#define TAM_TABELA (sizeof(tabela_corr_TW)/sizeof(CorrTWI_t))

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
uint16_t mediaADC(uint8_t adcPino, uint8_t numAmostras);
float valorPorcento (uint16_t mVoltSensor, SensorType_t sensorConv);
uint16_t validaVolt(uint16_t mVoltLido, uint16_t lowLim, uint16_t upperLim);
float calcularTIcorrigido(float TI_base);
float getFatorCorrecao(float temp);

//***************************************************************************************************************************************
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM3) {
	 HAL_GPIO_WritePin(Injetor_GPIO_Port, Injetor_Pin,GPIO_PIN_RESET); //Desliga aqui o injetor
	  HAL_TIM_Base_Stop_IT(&htim3);
  }
}

//***************************************************************************************************************************************
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) //chamdada da interrupção externa dente da roda fonica
{
	contDenteFonica = __HAL_TIM_GET_COUNTER(&htim2); // Lê o valor atual do contador
	  __HAL_TIM_DISABLE(&htim2);          // Desabilita o timer
	  __HAL_TIM_SET_COUNTER(&htim2, 0);   // Reinicia contador
	  __HAL_TIM_ENABLE(&htim2);           // Reinicia o Timer


	 if((contDenteFonica)>(contDenteFonica_anterior*2))
	 {
		 denteRodaFonica = 1;
		 contDenteFonica_anterior=contDenteFonica;
	 }
	 else
	 {
		 //RPM = freq Hz
		 RPM = 1000000/contDenteFonica;
		 denteRodaFonica++;
		 contDenteFonica_anterior=contDenteFonica;

		 if(denteRodaFonica==5)
		 {
			 //tempo_de_injeção_calc = 0.004; // PRECISA COLOCAR FORMULA --> num de contadores = tempo_injecao_calculado(em seg) / 0.00001 seg
			 tempo_de_injeção_calc = TInj_corrigido/1000;
			 cont_TempInj = tempo_de_injeção_calc/ 0.00001;
			// HAL_GPIO_WritePin
			 HAL_GPIO_WritePin(Injetor_GPIO_Port, Injetor_Pin,GPIO_PIN_SET); //liga aqui o injetor
			 HAL_TIM_Base_Start_IT(&htim3);
			  __HAL_TIM_DISABLE(&htim3);          // Desabilita o timer
			  __HAL_TIM_SET_AUTORELOAD(&htim3, cont_TempInj - 1); // Novo período do tempo de injeção
			  __HAL_TIM_SET_COUNTER(&htim3, 0);   // Reinicia contador
			  __HAL_TIM_ENABLE(&htim3);           // Reabilita o timer

		 }
	 }


	 EXTI->PR = EXTI_PR_PR12;  // Escreve 1 no bit para limpar flag de interrupção
}
//******************************************************************


int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, adcBuffer, 5); //Lembre de no IOC marcar como circular
  __HAL_TIM_ENABLE(&htim2);
  __HAL_TIM_ENABLE(&htim3);
  HAL_TIM_Base_Start_IT(&htim3);//USAR O TIMER 3 PRA desligar o injetor

  while (1)
  {
	  	  //Aquisita o valor RAW do ADC e tira média
	  	  pedal1Raw = mediaADC(3, 10);
	  	  pedal2Raw = mediaADC(4, 10);
	  	  PressMAP_RAW = mediaADC(1, 10);
	  	  TempMAP_RAW =  mediaADC(0, 10);
	  	  TempAgua_RAW =  mediaADC(2, 10);

	  	//Transforma ADC raw em milivolt
	  	milivolt_pedal1 = (pedal1Raw*0.805860)*Fator_Erro;
	  	milivolt_pedal2 = (pedal2Raw*0.805860)*Fator_Erro;
		milivolt_PressMAP = (PressMAP_RAW*1.221)*Fator_Erro;
		milivolt_TempMAP = (TempMAP_RAW*1.221)*Fator_Erro;
		milivolt_TempAgua = (TempAgua_RAW*1.221)*Fator_Erro;

	  	  // Verifica out of range
	  	  milivolt_pedal1 = validaVolt(milivolt_pedal1, 500, 4500);
	  	  milivolt_pedal2 = validaVolt(milivolt_pedal2, 250, 2500);
		  milivolt_PressMAP = validaVolt(milivolt_PressMAP, 300, 4850);
		  milivolt_TempMAP = validaVolt(milivolt_TempMAP, 300, 4850);
		  milivolt_TempAgua = validaVolt(milivolt_TempAgua, 300, 4850);


	  	  //Chama função para calcular porcentagem
	  	  porcPedal1 = valorPorcento (milivolt_pedal1, Pedal1);
	  	  porcPedal2 = valorPorcento (milivolt_pedal2, Pedal2);
	  	  PressMAP_KPA = valorPorcento (milivolt_PressMAP, PressMAP);
	  	  Temp_MAP = valorPorcento (milivolt_TempMAP, TempMAP);
	  	  Temp_Agua = valorPorcento (milivolt_TempAgua, TempAgua);

	  	  IAT = Temp_MAP;
	  	  MAP = PressMAP_KPA,

	  	  // --- 1. Cálculo do Volume de Deslocamento ---
	  	  displacement_volume_per_cylinder_cm3 = PI_VAL * pow(bore / 2.0, 2) * stroke;
	  	  total_engine_displacement_liters = (displacement_volume_per_cylinder_cm3 * cylinders) / 1000.0;

	  	  // --- 2. Cálculo da Massa de Ar (Speed Density) ---
	  	  float T_Kelvin = IAT + 273.15; // Temperatura em Kelvin

	  	  // Densidade do ar usando a Lei dos Gases Ideais (MAP em kPa)
	  	  // air_density em g/cm³ - MAP em kPa * 1000 para Pa; R_gas em J/molK
	  	  // Para manter consistência com g/cm³, usamos M_air em g/mol e convertemos R_gas
	  	  // R_gas em kPa*cm³/mol*K = 8.314 J/molK * (1000 Pa / 1 J/m³) * (1 m³ / 10^6 cm³) = 0.008314 kPa*cm³/molK
	  	  // Ou simplesmente, o cálculo de air_density em kg/m^3 e depois converter.
	  	  // Vamos usar g/cm^3 para consistência com fuel_density
	  	  // (MAP_Pa * M_air_kg_mol) / (R_gas_J_molK * T_Kelvin) = kg/m^3
	  	  // Convertendo para g/cm^3: (kg/m^3) * (1000 g / 1 kg) * (1 m^3 / 10^6 cm^3) = g/cm^3 / 1000
	  	  // Simplificando, MAP_kPa * M_air_g_mol / (R_gas_J_molK * T_Kelvin) * (1/1000)
	  	  float air_density_g_cm3 = (MAP * M_air) / (R_gas * T_Kelvin * 1000.0); // Resulta em g/cm³ se R_gas for 8.314 J/molK e MAP em kPa

	  	  // Massa de ar por segundo (g/s)
	  	  // (litros * g/cm³ * 1000 cm³/litro * eficiencia * RPM) / (2 rev/ciclo * 60 s/min)
	  	  // volume de deslocamento por ciclo = total_engine_displacement_liters * volumetric_efficiency
	  	  // Massa de ar por ciclo = volume_deslocamento_ciclo * air_density_g_cm3 * 1000.0 (para cm3)
	  	  // Frequência de ciclos = RPM / (2 * 60)
	  	  float mass_air_flow_gs = (total_engine_displacement_liters * volumetric_efficiency * RPM * air_density_g_cm3 * 1000.0) / (2.0 * 60.0);

	  	  // --- 3. Cálculo da Massa de Combustível ---
	  	  float mass_fuel_flow_gs = mass_air_flow_gs / AFR; // Massa de combustível por segundo (g/s)

	  	  // --- 4. Cálculo da Massa de Combustível por Injeção (por cilindro) ---
	  	  // Número de injeções por segundo para o motor completo
	  	  float injections_per_second_total = (RPM / 60.0) * (cylinders / 2.0);
	  	  float mass_fuel_per_injection_g = mass_fuel_flow_gs / injections_per_second_total;

	  	  // --- 5. Cálculo da Vazão Efetiva do Injetor ---
	  	  float delta_P_base_kPa = base_fuel_pressure_bar_diff * 100.0; // Pressão diferencial de calibração em kPa
	  	  float delta_P_real_kPa = (current_fuel_pressure * 100.0) - MAP; // Pressão diferencial real em kPa

	  	  // Prevenção de divisão por zero ou raiz de número negativo
	  	  if (delta_P_real_kPa <= 0 || delta_P_base_kPa <= 0) {

		  	  sprintf(msg, "Erro: Pressao diferencial invalida para calculo de vazao\r\n");
			  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);
	  	      }

	  	  // Vazão ajustada do injetor em cc/min
	  	  float actual_flow_rate_cc_min = base_flow_rate_cc_min * sqrt(delta_P_real_kPa / delta_P_base_kPa);

	  	  // --- 6. Conversão da Vazão do Injetor para g/ms ---
	  	  float actual_flow_rate_gms = (actual_flow_rate_cc_min * fuel_density) / 60000.0; // (cc/min * g/cc) / (60s/min * 1000ms/s)

	  	  // --- 7. Cálculo do Tempo de Injeção Final ---
	  	  // Prevenção de divisão por zero
	  	  if (actual_flow_rate_gms <= 0) {
		  	  sprintf(msg, "Erro: Vazao do injetor invalida para calculo de tempo de injeçao\r\n");
			  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);
	  	      }

	  	  float calculated_pulse_width_ms = mass_fuel_per_injection_g / actual_flow_rate_gms;

	  	  // Adição do tempo morto do injetor
	  	  final_injection_pulse_ms = calculated_pulse_width_ms + injector_dead_time_ms;

	  	  //CORRECAO TEMPO INJ POR TEMP AGUA
	  	  TInj_corrigido = calcularTIcorrigido(final_injection_pulse_ms);

	  	  // --- Exibir resultados ---
		  //Envia dados na UART
		  /*
	  	  sprintf(msg, "mVolt Pedal_1= %d\r\n", milivolt_pedal1);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		  sprintf(msg, "mVolt Pedal_2 = %d\r\n", milivolt_pedal2);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		  sprintf(msg, "mVolt PRES MAP = %d\r\n", milivolt_PressMAP);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		  sprintf(msg, "mVolt TEMP AR = %d\r\n", milivolt_TempMAP);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		  sprintf(msg, "mVolt TEMP AGUA = %d\r\n", milivolt_TempAgua);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		*/
		  sprintf(msg, "Porc P1 = %.2f\r\n", porcPedal1);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		  sprintf(msg, "Porc P2 = %.2f\r\n", porcPedal2);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		  sprintf(msg, "PRES MAP = %.2f\r\n", PressMAP_KPA);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		  sprintf(msg, "TEMP AR = %.2f\r\n", Temp_MAP);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		  sprintf(msg, "TEMP AGUA = %.2f\r\n", Temp_Agua);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		  sprintf(msg, "RPM = %.2f\r\n", RPM);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		  sprintf(msg, "Massa de Ar = %.2f", mass_air_flow_gs);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);
		  sprintf(msg, " g/s\r\n");
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		  sprintf(msg, "Massa Combustivel Total = %.2f", mass_fuel_flow_gs);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);
		  sprintf(msg, " g/s\r\n");
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		  sprintf(msg, "Massa Combustivel por Injeçao = %.2f", mass_fuel_per_injection_g);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);
		  sprintf(msg, " g\r\n");
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		  sprintf(msg, "Vazao Efetiva Injetor = %.2f", actual_flow_rate_cc_min);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);
		  sprintf(msg, " ms\r\n");
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		  sprintf(msg, "Tempo Inj. Calculado (sem dead time) = %.2f", calculated_pulse_width_ms);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);
		  sprintf(msg, " g/s\r\n");
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);

		  sprintf(msg, "Tempo Inj. FINAL (c/ dead time) = %.2f", final_injection_pulse_ms);
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);
		  sprintf(msg, " ms\r\n");
		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 50);


HAL_Delay(100);

  }

}//FIM DO MAIN
//*************************************************************************************************************************************
uint16_t mediaADC(uint8_t adcPino, uint8_t numAmostras)
{
	uint32_t soma = 0;
	uint8_t contador = 0;
	uint16_t mediaADC = 0;
	while(contador<numAmostras)
	{
		soma+=adcBuffer[adcPino];
		contador++;

		uint16_t contDelay_us = 0;
		while(contDelay_us<4000)
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
	        	//porc_temp = (0.1075 * mVoltSensor) - 236.5591;
	            break;

	        case Pedal2:
	        	porc_temp = (0.0617 * mVoltSensor) - 23.45;
	        	//porc_temp = (0.0943 * mVoltSensor) - 36.7925;
	            break;

	        case PressMAP:
	        	porc_temp = (0.0247 * mVoltSensor) + 1.1211;
	            break;

	        case TempAgua:
	        	porc_temp = (0.00001*(mVoltSensor*mVoltSensor))-(0.0617*mVoltSensor)+116.49;
	            break;

	        case TempMAP:
	        	porc_temp = (0.0248 * mVoltSensor) - 77.282;
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
// Função para buscar o fator de correção com interpolação linear
float getFatorCorrecao(float temp) {
    // Limites inferior e superior
    if (temp <= tabela_corr_TW[0].temp_c) {
        return tabela_corr_TW[0].fator;
    }
    if (temp >= tabela_corr_TW[TAM_TABELA - 1].temp_c) {
        return tabela_corr_TW[TAM_TABELA - 1].fator;
    }

    // Busca na tabela
    for (uint8_t i = 0; i < TAM_TABELA - 1; i++) {
        if (temp >= tabela_corr_TW[i].temp_c && temp < tabela_corr_TW[i + 1].temp_c) {
            // Interpolação linear
            float t1 = tabela_corr_TW[i].temp_c;
            float t2 = tabela_corr_TW[i + 1].temp_c;
            float f1 = tabela_corr_TW[i].fator;
            float f2 = tabela_corr_TW[i + 1].fator;

            return f1 + ((temp - t1) * (f2 - f1) / (t2 - t1));
        }
    }
    return 1.0f; // Valor padrão (não deveria chegar aqui)
}


// Função principal de cálculo de TI corrigido
float calcularTIcorrigido(float TI_base) {
    float fator = getFatorCorrecao(Temp_Agua);
    return TI_base * fator;
}
//*************************************************************************************************************************************
/**
  * @brief System Clock Configuration
  * @retval None
  */
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
  hadc1.Init.NbrOfConversion = 5;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_5;
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

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;//cada contador tera 1us
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
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
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 719;//tempo de cada contador = 10us
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&htim3, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Injetor_GPIO_Port, Injetor_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Injetor_Pin */
  GPIO_InitStruct.Pin = Injetor_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Injetor_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Sensor_Rot_Pin */
  GPIO_InitStruct.Pin = Sensor_Rot_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Sensor_Rot_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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
