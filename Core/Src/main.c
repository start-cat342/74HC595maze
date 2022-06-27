/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include<memory.h>
#include<string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define queue_max 300 //?��?�� ?���? 매모�? 초과?�� �?경할�?
#define M 4 //배열 ?���?(?��?���?)
#define N 4 //배열 ?���?(?��?���?)

int date[10][10] = {{1,0,0,0},
				  {-1,0,-1,0},
					{0,0,0,0},
					{0,-1,0,0}};
int xdate[100];//xy ???��?��
int ydate[100];//xy ???��?��
const int xifdate[4] = { 1,-1,0,0 };
const int yifdate[4] = { 0,0,1,-1 };


typedef struct __queue
{
    int queue[queue_max];
    int top; //
}queue;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void queue_into(queue* date)
{
    memset(date->queue, 0, sizeof(int) * queue_max);
    date->top = -1;
}
void queue_push(queue* date, int indate)
{
    date->top++;
    date->queue[date->top] = indate;

}
void queue_pop(queue* date, int* outdate)
{

    if (date->top >= 0)
    {
        *outdate = date->queue[0];
        for (int i = 0; i < (date->top); i++)
        {
            date->queue[i] = date->queue[i + 1];
        }
        date->top--;
    }
    else
    {
        *outdate = -1;
    }
}
int queue_size(queue* date)
{
    return date->top + 1;
}


queue qx;
queue qy;

void BFS(int inx, int iny)
{
	  queue_into(&qx);
	  queue_into(&qy);

	  queue_push(&qx,inx);
	  queue_push(&qy,iny);

	  date[inx][iny] = 1;
    while (queue_size(&qx))
    {
        int x;
        int y;
        queue_pop(&qx,&x);
        queue_pop(&qy,&y);

        for (int i = 0; 4 > i; i++)
        {
            if ( xifdate[i] + x >= 0 && yifdate[i] + y >= 0) //?�� ?��리상 ?��?��처리 �??��
            {
            	if(date[xifdate[i] + x][yifdate[i] + y] == 0)
            	{
            		date[xifdate[i] + x][yifdate[i] + y] = date[x][y] + 1;
            		queue_push(&qx, xifdate[i] + x);
            		queue_push(&qy, yifdate[i] + y);
            	}

            }
        }
    }



}
void line(int x, int y)
{
    int cot = 1;//배열
    xdate[0] = x; // ?��?�� 좌표 ?��?��
    ydate[0] = y;
    while (1)
    {
        for (int i = 0; 4 > i; i++)
        {

        	if (xifdate[i] + x >= 0 && yifdate[i] + y >= 0 && xifdate[i] + x <= 3 && yifdate[i] + y <= 3 && (date[xifdate[i] + x][yifdate[i] + y] + 1) == date[x][y])
        	{

        			if (date[xifdate[i] + x][yifdate[i] + y] == 1)
        			{
        				xdate[cot] = xifdate[i] + x;
        				ydate[cot] = yifdate[i] + y;
        				cot++;
        				goto out;//while �? 빠�? ?���??�� ?��?��
        			}
        			xdate[cot] = xifdate[i] + x;
        			ydate[cot] = yifdate[i] + y;
        			x = xifdate[i] + x;
                	y = yifdate[i] + y;
                	cot++;



        	}

        }
    }
    out:

    //출력
    for (int i = cot - 1; i >= 0; i--)
    {
        for (int j = 3; j >= 0; j--)
        {
            for (int h = 3; h >= 0; h--)
            {
                if (xdate[i] == j && ydate[i] == h)
                {
                	set595(1);
                }
                else if (date[j][h] == -1)
                {
                	set595(1);
                }
                else
                {
                	set595(0);
                }
            }

        }
        out595();
        HAL_Delay(1000);
    }
}
void out595()
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2, 1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, 1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2, 0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, 0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4, 0);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4, 1);
}
void set595(int x)
{
	//L1,L2 ->12 2,5 래치
	//SDI -> 14  1 데이터
	//CLK -> 11  3  시프트
	//CLR -> 10  4  리셋
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4, 1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1, !x);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3, 1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3, 0);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  /* USER CODE BEGIN 2 */
  //3
  int cot = 0;
  const int test_case[5][4][4]={{{0,-1,0,0},
		  	  	  	  	  	 {0,0,0,0},
							 {-1,0,-1,0},
							 {0,0,0,0}},

							 {{0,0,0,0},
							 {-1,-1,0,0},
							 {0,0,0,0},
							 {0,-1,0,0}},

							 {{0,0,0,0},
							 {0,0,-1,0},
							 {-1,-1,-1,0},
							 {0,0,0,0}},

							 {{0,0,0,0},
							 {0,0,0,0},
							 {0,0,0,0},
							 {0,0,0,0}},

							 {{0,-1,0,0},
							 {0,0,-1,0},
							 {-1,0,-1,0},
							 {0,0,0,-1}},

							 {{0,0,0,0},
							 {0,0,0,0},
							 {-1,-1,-1,0},
							 {0,0,0,0}}};
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //cot = 3;
	  if(!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)))
	  {
		  for (int i = 0; 3 >= i; i++)
		  {
		          for (int j = 0; 3 >= j; j++)
		          {
		             date[i][j]=test_case[cot][i][j];
		          }
		  }
		  BFS(0,0);
		  line(3,0);
	  }
	cot++;
	if(cot==5){cot=0;}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SDI_Pin|L1_Pin|CLK_Pin|CLR_Pin
                          |L2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SDI_Pin L1_Pin CLK_Pin CLR_Pin
                           L2_Pin */
  GPIO_InitStruct.Pin = SDI_Pin|L1_Pin|CLK_Pin|CLR_Pin
                          |L2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SW1_Pin */
  GPIO_InitStruct.Pin = SW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);

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
