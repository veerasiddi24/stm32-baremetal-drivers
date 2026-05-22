/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "string.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define SCB_CPACR   (*(volatile uint32_t*)0xE000ED88)
#define FPU_FPCCR   (*(volatile uint32_t*)0xE000EF34)
#define MAX_LOGS           1024
#define LOG_SIZE           4
#define OBJECT_THRESHOLD   20.0f
#define OBJECT_ENTER 1
#define OBJECT_EXIT  2
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t tx[4] = {0x11, 0x22, 0x33, 0x44};
uint8_t rx[4];
uint32_t ft = 12;
uint32_t r_ft;
uint32_t flash_id;
float Retrived_Frm_Flash;
static uint32_t rising_time = 0;
static uint32_t falling_time = 0;
static uint8_t capture_state = 0;
static float distance_cm = 0;
static uint8_t measurement_done =0;

uint8_t Read_Buf[4];
uint8_t Write_Buf[4];


volatile uint8_t Exti_Flag = 0;


typedef struct
{
    uint8_t event;
    float distance;
    uint32_t timestamp;
}ObjectLog_t;


ObjectLog_t logData;

uint32_t address = 0;

uint32_t logCounter = 0;

uint8_t object_present = 0;

uint32_t system_time = 0;

//void delay_us(uint32_t us)
//{
//    TIM2->CNT = 0;              // reset counter
//
//    while(TIM2->CNT < us);
//
//}

void delay_us(uint32_t time) {
	for (uint32_t i = 0; i < time * 10; i++)
		;
}

void Ultrasonic_Trigger(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, RESET);
	delay_us(2);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, SET);
    delay_us(10);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, RESET);
}

//void Flash_LogDistance(float distance,
//                       uint32_t *address,
//                       uint32_t *logCounter)
//{
//    memcpy(Write_Buf, &distance, sizeof(float));
//
//    if((*address % 4096) == 0)
//    {
//        w25q_erase_sector((*address) / 4096);
//
//        while(w25q_read_status() & 0x01);
//    }
//
//    w25q_write(*address,
//               Write_Buf,
//               sizeof(float));
//
//    while(w25q_read_status() & 0x01);
//
//    *address += sizeof(float);
//
//    (*logCounter)++;
//
//    if(*logCounter >= MAX_LOGS)
//    {
//        *logCounter = 0;
//        *address = 0;
//    }
//}
//
//void Flash_PrintLastLogs(uint32_t *logCounter)
//{
//    HAL_Delay(200);
//
//    Exti_Flag = 0;
//
//    uint32_t count;
//
//    if(*logCounter < 10)
//        count = *logCounter;
//    else
//        count = 10;
//
//    printf("\r\nLast %ld Logs:\r\n", count);
//
//    for(uint32_t i = 0; i < count; i++)
//    {
//        uint32_t index = *logCounter - count + i;
//
//        uint32_t read_addr = index * sizeof(float);
//
//        w25q_read(read_addr,
//                  Read_Buf,
//                  sizeof(float));
//
//        memcpy(&Retrived_Frm_Flash,
//               Read_Buf,
//               sizeof(float));
//
//        printf("[%ld] Distance = %.2f cm\r\n",
//               index,
//               Retrived_Frm_Flash);
//    }
//}

void Store_Log(uint8_t event,float distance)
{
    ObjectLog_t log;

    log.event = event;

    log.distance = distance;

    log.timestamp = system_time;

    /* Erase sector when new sector starts */
    if((address % 4096) == 0)
    {
        w25q_erase_sector(address / 4096);

        while(w25q_read_status() & 0x01);
    }

    /* Write structure */
    w25q_write(address,
               (uint8_t*)&log,
               sizeof(ObjectLog_t));

    while(w25q_read_status() & 0x01);

    address += sizeof(ObjectLog_t);

    logCounter++;

    /* Circular logging */
    if(logCounter >= MAX_LOGS)
    {
        logCounter = 0;
        address = 0;
    }
}


void Read_All_Logs(void)
{
    ObjectLog_t readLog;

    printf("\r\n========= FLASH LOGS =========\r\n");

    uint32_t count;

    if(logCounter < MAX_LOGS)
        count = logCounter;
    else
        count = MAX_LOGS;

    for(uint32_t i = 0; i < count; i++)
    {
        uint32_t read_address;

        read_address = i * sizeof(ObjectLog_t);

        w25q_read(read_address,
                  (uint8_t*)&readLog,
                  sizeof(ObjectLog_t));

        printf("Log %ld\r\n",i);

        if(readLog.event == OBJECT_ENTER)
        {
            printf("EVENT    : OBJECT ENTER\r\n");
        }
        else if(readLog.event == OBJECT_EXIT)
        {
            printf("EVENT    : OBJECT EXIT\r\n");
        }

        printf("DISTANCE : %.2f cm\r\n",
               readLog.distance);

        printf("TIME     : %ld\r\n",
               readLog.timestamp);

        printf("-----------------------------\r\n");
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	// Enable CP10 and CP11 full access
		SCB_CPACR |= (0xF << 20);
	// Disable lazy stacking
	    FPU_FPCCR &= ~((1 << 31) | (1 << 30)); // ASPEN=bit31, LSPEN=bit30
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
  w25q_reset();

  flash_id = w25q_readID();

//  w25q_erase_sector(0);
//
//  w25q_write(0x000000, (uint8_t*)&ft, 4);
//
//  HAL_Delay(10);
//
//  w25q_read(0x000000, (uint8_t*)&r_ft, 4);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


//  	while(1)
//  	{
//  	    measurement_done = 0;
//
//  	    Ultrasonic_Trigger();
//
//  	    uint32_t timeout = 500000;
//
//  	    while((measurement_done == 0) && timeout--);
//
//  	    float distance = distance_cm;
//
//  	  Flash_LogDistance(distance,
//  	                    &address,
//  	                    &logCounter);
//
//  	    if(Exti_Flag)
//  	    {
//
//  	    	Flash_PrintLastLogs(&logCounter);
//  	    }
//
//  	  HAL_Delay(100);
//  	}

        while(1)
  	    {
  	        system_time++;

  	        measurement_done = 0;

  	        Ultrasonic_Trigger();

  	        uint32_t timeout = 500000;

  	        while((measurement_done == 0) && timeout--);

  	        float distance = distance_cm;

  //	        printf("Distance : %.2f cm\r\n",distance);

  	        /****************************************
  	         * OBJECT ENTER DETECTION
  	         ****************************************/
  	        if((distance < OBJECT_THRESHOLD) &&
  	           (object_present == 0))
  	        {
  	            object_present = 1;

  	            printf("OBJECT ENTERED\r\n");

  	            Store_Log(OBJECT_ENTER,
  	                      distance);
  	        }

  	        /****************************************
  	         * OBJECT EXIT DETECTION
  	         ****************************************/
  	        else if((distance > OBJECT_THRESHOLD) &&
  	                (object_present == 1))
  	        {
  	            object_present = 0;

  	            printf("OBJECT EXITED\r\n");

  	            Store_Log(OBJECT_EXIT,
  	                      distance);
  	        }

  	        /****************************************
  	         * BUTTON PRESSED
  	         ****************************************/
  	        if(Exti_Flag)
  	        {
//
  	        	HAL_Delay(100);

  	            Exti_Flag = 0;

  	            Read_All_Logs();
  	        }

  	         HAL_Delay(100);
  	    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* EXTI15_10_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  /* TIM2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 4-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//	if(GPIO_Pin == 13)
	{
		Exti_Flag = 1;
	}
}



//
//	void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
//		if(htim->Instance == TIM2) {
//	        uint32_t capture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
//
//	        if(capture_state == 0) {
//	            rising_time = capture;
////	            __HAL_TIM_DISABLE(htim);
//	            TIM_CCxChannelCmd(htim->Instance,
//	                              TIM_CHANNEL_1,
//	                              TIM_CCx_DISABLE);
//
//	            __HAL_TIM_SET_CAPTUREPOLARITY(htim,
//	                                          TIM_CHANNEL_1,
//	                                          TIM_INPUTCHANNELPOLARITY_FALLING);
//
//	            TIM_CCxChannelCmd(htim->Instance,
//	                              TIM_CHANNEL_1,
//	                              TIM_CCx_ENABLE);
////	            __HAL_TIM_ENABLE(htim);
//	            capture_state = 1;
//	        } else {
//	            falling_time = capture;
//	            uint32_t pulse = (falling_time >= rising_time) ?
//	                              (falling_time - rising_time) :
//	                              ((0xFFFF - rising_time + 1) + falling_time);
//
//	            distance_cm = pulse / 58.0f;
//	            capture_state = 0;
////	            __HAL_TIM_DISABLE(htim);
//	            TIM_CCxChannelCmd(htim->Instance,
//	                              TIM_CHANNEL_1,
//	                              TIM_CCx_DISABLE);
//
//	            __HAL_TIM_SET_CAPTUREPOLARITY(htim,
//	                                          TIM_CHANNEL_1,
//	                                          TIM_INPUTCHANNELPOLARITY_FALLING);
//
//	            TIM_CCxChannelCmd(htim->Instance,
//	                              TIM_CHANNEL_1,
//	                              TIM_CCx_ENABLE);
//	            //	            __HAL_TIM_ENABLE(htim);
//	            measurement_done = 1;
//	        }
//	    }
//	}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        uint32_t capture =
            HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

        if(capture_state == 0)
        {
            rising_time = capture;

            TIM_CCxChannelCmd(htim->Instance,
                              TIM_CHANNEL_1,
                              TIM_CCx_DISABLE);

            __HAL_TIM_SET_CAPTUREPOLARITY(htim,
                                          TIM_CHANNEL_1,
                                          TIM_INPUTCHANNELPOLARITY_FALLING);

            TIM_CCxChannelCmd(htim->Instance,
                              TIM_CHANNEL_1,
                              TIM_CCx_ENABLE);

            capture_state = 1;
        }
        else
        {
            falling_time = capture;

            uint32_t pulse;

            if(falling_time >= rising_time)
            {
                pulse = falling_time - rising_time;
            }
            else
            {
                pulse =
                    (0xFFFFFFFF - rising_time + 1)
                    + falling_time;
            }

            distance_cm = pulse / 58.0f;

            capture_state = 0;

            TIM_CCxChannelCmd(htim->Instance,
                              TIM_CHANNEL_1,
                              TIM_CCx_DISABLE);

            __HAL_TIM_SET_CAPTUREPOLARITY(htim,
                                          TIM_CHANNEL_1,
                                          TIM_INPUTCHANNELPOLARITY_RISING);

            TIM_CCxChannelCmd(htim->Instance,
                              TIM_CHANNEL_1,
                              TIM_CCx_ENABLE);

            measurement_done = 1;
        }
    }
}
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
