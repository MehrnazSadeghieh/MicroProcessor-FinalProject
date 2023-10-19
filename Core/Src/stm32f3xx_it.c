/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f3xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
extern int timer;
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
typedef unsigned char byte;
extern byte player[];
extern byte enemy[];
extern byte shot[];
extern int board[20][4];
extern int bullet[10][3];
extern int fire[5][3];
extern int shot_position_x[10];
extern int shot_position_y[10];
extern int stop_menu_music;


/////player position
extern int x;
extern int y;
extern int enemy_falling_timer;
int enemy_lowest_row = 17;
int enemy_falling_duration = 5000;
int enemyposition;
int end_shot[10] = {0,0,0,0,0,0,0,0,0,0};

RTC_TimeTypeDef mytime;
RTC_DateTypeDef mydate;
char timeStr[20];
char dateStr[20];
extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart1;


extern int playershot;
extern int started;
extern int menu;
extern int information;
extern int entername;
extern int uartflag;
extern int choose_mode;
extern int total_hit;
extern int health;
extern int game_score;
extern TIM_HandleTypeDef htim3;
extern volatile uint16_t volume;

int in_menu = 0;
int in_info = 0;
int in_name_entering = 0;
int in_game_mode = 0;
int in_game = 0;
int bullet_sound = 0;
int enemy_bullet_sound =0;
int fire_timer = 0;
int info_timer = 0;

int win = 0;
int lost = 0;
extern char name[10];

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void kick_enemy(int i,int x,int y);
void startOfGame();
void enemy_falling();
void bullet_handler();
void update_board_to_bullet(int x,int y);
void update_board_from_bullet(int x,int y);
void create_fire();
int random_gen();
void fire_handler();
void remove_fire(int i,int x,int y);
void update_board_to_fire(int x,int y);
void update_board_from_fire(int x,int y);
void finish();
void remove_fire_and_bullet(int ii, int x, int y);
void menu_func();
void information_func();
void entername_func();
void choose_mode_func();
/////////
void clean_LCD(){
	setCursor(0, 0);
	print("                        ");
	setCursor(0, 1);
	print("                        ");
	setCursor(0, 2);
	print("                        ");
	setCursor(0, 3);
	print("                        ");
	setCursor(0, 0);
}

////player is at start or is playing the game
int playing_game = 0;

void startOfGame(){
	for(int i = 17; i < 20; i++){
		board[i][0] = 2;
		board[i][1] = 2;
		board[i][2] = 2;
		board[i][3] = 2;
	}
	board[y][x] = 1;
}

void enemy_falling(){
	enemy_lowest_row --;
	for(int i = enemy_lowest_row; i < 19; i++){
		for(int j = 0; j < 4; j++){
			board[i][j] = board[i+1][j];
		}
	}
	board[20][0] = 2;
	board[20][1] = 2;
	board[20][2] = 2;
	board[20][3] = 2;
	enemy_falling_timer = HAL_GetTick();
}
void bullet_handler(){
	for(int i = 0 ; i < 10 ; i++){
		int old_x = bullet[i][1];
		int old_y = bullet[i][2];
		if (bullet [i][0] == 1){
			bullet[i][2]++;

			int new_x = bullet[i][1];
			int new_y = bullet[i][2];
			if (new_y > 19){
				bullet[i][0] = 0;
			}else{
				if (board[new_y][new_x] == 2){
					kick_enemy(i,new_x,new_y);
					update_board_from_bullet(old_x,old_y);
				}
				else if (board[new_y][new_x] == 0){
					update_board_to_bullet(new_x, new_y);
					update_board_from_bullet(old_x,old_y);
				}

			}
		}
	}
}
void kick_enemy(int i,int x,int y){
//	board[y+1][x] = 0;
	board[y][x] = 0;
//	board[y-1][x] = 0;
	total_hit --;
	bullet[i][0] = 0;
	bullet[i][1] = 0;
	bullet[i][2] = 0;
	bullet_sound = 1;
}
void update_board_to_bullet(int x,int y){
	board[y][x] = 3;
}
void update_board_from_bullet(int x,int y){
	if (y > 0){
		board[y][x] = 0;
	}
}
void create_fire(){
//	for (int i = 0 ; i < 5 ; i++){
//		if(fire[i][0] == 0){
//			fire[i][0] = 1;
//			fire[i][1] = random_gen();
//			fire[i][2] = enemy_lowest_row-1;
			board[enemy_lowest_row - 1][random_gen()] = 4;
//			break;
//		}
//	}
}
int random_gen(){
	int rnd;
//	time_t t;
//	srand((unsigned) time(&t));
	rnd = rand();
	rnd = rnd % 4;
	return rnd;
}

void enemy_fire(){
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < 4; j++){
			if(board[i][j] == 4){
				board[i][j] = 0;
				if(i-1 >= 0){
					if(board[i-1][j] == 3){
						board[i-1][j] = 0;
					}
					else if(board[i-1][j] == 1){
						health --;
					}
					else{
						board [i-1][j] = 4;
					}
				}
				else if(board[i][j] != 1){
					board[i][j] = 0;
				}
				else{
					health--;
					enemy_bullet_sound = 1;
				}
			}
		}
	}
}
//void fire_handler(){
//	for (int i = 0 ; i<5 ; i++){
////		unsigned char result_to_uart[30]= "";
////			  int n = sprintf(result_to_uart, "here i= %d\n" ,i);
////			  HAL_UART_Transmit(&huart1, result_to_uart, n, 1000);
//		int old_x = fire[i][1];
//		int old_y = fire[i][2];
//		if (fire[i][0] == 1){
//			fire[i][2]--;
//			int new_x = fire[i][1];
//			int new_y = fire[i][2];
////			unsigned char result_to_uart[30]= "";
////			int n = sprintf(result_to_uart, "here tar y= %d x = %d\n" ,new_y, new_x);
////			HAL_UART_Transmit(&huart1, result_to_uart, n, 1000);
//			if (board[new_y][new_x] == 3) {
//				remove_fire_and_bullet(i,new_x,new_y);
//			} else if (board[new_y][new_x] == 1) {
//				finish();
//			} else if (board[new_y][new_x] == 0){
//			    update_board_to_fire(new_x,new_y);
//			}else if (new_y == 0){
//				remove_fire(i,new_x,new_y);
//			}
//			update_board_from_fire(old_x,old_y);
//		}
//	}
//}
//void remove_fire(int i,int x,int y){
//		fire[i][0] = 0;
//		board[y][x] = 0;
//}
//void update_board_to_fire(int x,int y){
//	unsigned char result_to_uart[30]= "";
////		  int n = sprintf(result_to_uart, "update fire, y, %d , x, %d\n" , y,x);
////		  HAL_UART_Transmit(&huart1, result_to_uart, n, 1000);
//		board[y][x] = 4;
//
//}
//void update_board_from_fire(int x,int y){
//		board[y][x] = 0;
//}
//void finish(){
//	health --;
//	enemy_bullet_sound = 1;
//}
//void remove_fire_and_bullet(int ii, int x, int y) {
//	for (int i = 0; i < 10; i++) {
//		if (bullet[i][1] == x && bullet[i][2] == y) {
//			bullet[i][0] = 0;
////			unsigned char result_to_uart[30]= "";
////			int n = sprintf(result_to_uart, "i-x-y : %d %d %d\n" ,i,x,y);
////			HAL_UART_Transmit(&huart1, result_to_uart, n, 1000);
//		}
//		remove_fire(ii, x, y);
//	}
//}
void menu_func(){
	clean_LCD();
	setCursor(0, 0);
	print("s13 to start");
	setCursor(0, 1);
	print("s14 for more info");
	menu = 0;
	in_menu = 1;
}

void information_func(){
	clean_LCD();
	setCursor(0, 0);
	print("Sadeghieh");
	setCursor(0, 1);
	print("Etemadheravi");
	setCursor(10, 3);
	print("return(s15)");
	HAL_RTC_GetTime(&hrtc, &mytime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &mydate, RTC_FORMAT_BIN);
	setCursor(0, 2);
	sprintf(timeStr,"%2d:%2d:%2d",mytime.Hours,mytime.Minutes,mytime.Seconds);
	print(timeStr);

	setCursor(0, 3);
	sprintf(dateStr," %d \\ %d \\ %d",mydate.Year,mydate.Month,mydate.Date);
	print(dateStr);
	in_info = 1;
}

void entername_func(){
	clean_LCD();
	setCursor(0, 0);
	print("Enter your name");
	setCursor(0, 1);
	print("confirm:s16");
	entername = 0;
	uartflag = 1;
	in_name_entering = 1;
}

void choose_mode_func(){
	clean_LCD();
	setCursor(0, 0);
	print("easy s9");
	setCursor(0, 1);
	print("normal s10");
	setCursor(0, 2);
	print("hard s11");
	setCursor(0, 3);
	print("confirm s12");
	choose_mode = 0;
	in_game_mode = 1;
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  if(stop_menu_music == 0){
	  Update_Melody();
  }
  if(stop_menu_music == 1){
	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
  }
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */

  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */

  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  /* USER CODE BEGIN EXTI1_IRQn 1 */

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line2 and Touch Sense controller.
  */
void EXTI2_TSC_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_TSC_IRQn 0 */

  /* USER CODE END EXTI2_TSC_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  /* USER CODE BEGIN EXTI2_TSC_IRQn 1 */

  /* USER CODE END EXTI2_TSC_IRQn 1 */
}

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles ADC1 and ADC2 interrupts.
  */
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */

  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */
  int entry = HAL_ADC_GetValue(&hadc1);
  volume = entry * 100/4095;

  /* USER CODE END ADC1_2_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */
  HAL_ADC_Start_IT(&hadc1);
//  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_10);
  if(HAL_GetTick() >= timer + 250){

	  timer = HAL_GetTick();
	  unsigned char result_to_uart[30]= "";
//		int n = sprintf(result_to_uart, "hii");
//		HAL_UART_Transmit(&huart1, result_to_uart, n, 1000);
	  if(menu == 1){
		  menu_func();
		  info_timer = HAL_GetTick();
	  }
	  if(information == 1){
		  if(HAL_GetTick() >= info_timer + 1000){
			  information_func();
		  }
	  }
	  if(entername == 1){
		  entername_func();
	  }
	  if(choose_mode == 1){
		  choose_mode_func();
	  }
	  if(started == 1){
		  PWM_Change_Tone(0, 0);
		  if(bullet_sound == 0 && enemy_bullet_sound == 0){
			  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
		  }
		  fire_timer = HAL_GetTick();
		  for(int i = 0; i < 20; i++){
			  for(int j = 0; j < 4; j++){
				  setCursor(i, j);
				  ////player
				  if(board[i][j] == 1){
					  write(1);
				  }
				  ////enemy
				  else if(board[i][j] == 2){
					  write(2);
				  }
				  ////shot
				  else if(board[i][j] == 3){
//					  print(".");
					  write(3);
				  }
				  ////fire
				  else if(board[i][j] == 4){
//					  print(".");
					  write(3);
				  }
				  ////empty
				  else{
					  print(" ");
				  }
			  }
		  }
		  in_game = 1;
		  if(playing_game == 0){
			  startOfGame();
				if(enemy_falling_duration == 5000){
					int n = sprintf(result_to_uart, "Game started... mode: easy, goal:%d enemy_durtion:%d\n",total_hit,enemy_falling_duration);
					HAL_UART_Transmit(&huart1, result_to_uart, n, 1000);
				}
				else if(enemy_falling_duration == 3000){
					int n = sprintf(result_to_uart, "Game started... mode: normal, goal:%d enemy_durtion:%d\n",total_hit,enemy_falling_duration);
					HAL_UART_Transmit(&huart1, result_to_uart, n, 1000);
				}
				else{
					int n = sprintf(result_to_uart, "Game started... mode: hard, goal:%d enemy_durtion:%d\n",total_hit,enemy_falling_duration);
					HAL_UART_Transmit(&huart1, result_to_uart, n, 1000);
				}

			  playing_game = 1;
			  enemy_falling_timer = HAL_GetTick();
		  }
		  if(HAL_GetTick() >= enemy_falling_timer + enemy_falling_duration){
			  enemy_falling();
		  }
		  if(HAL_GetTick() >= fire_timer + 1000){
			  create_fire();
			  enemy_fire();
			  fire_timer = HAL_GetTick();
		  }
		  bullet_handler();
		  if(total_hit == 3){
				int n = sprintf(result_to_uart, "yoohoo just 3 more hit\n");
				HAL_UART_Transmit(&huart1, result_to_uart, n, 1000);
		  }
		  if(health == 1){
				int n = sprintf(result_to_uart, "Be careful its your last chance\n");
				HAL_UART_Transmit(&huart1, result_to_uart, n, 1000);
		  }
		  if(health == 0 || board[1][0] == 2 || board[1][1] == 2 || board[1][2] == 2 || board[1][3] == 2){
			  int n = sprintf(result_to_uart, "Game Over\n");
			  HAL_UART_Transmit(&huart1, result_to_uart, n, 1000);
			  lost = 1;
			  started = 0;

		  }
		  if(total_hit == 0){
			  int n = sprintf(result_to_uart, "Congratulations you did it \n");
			  HAL_UART_Transmit(&huart1, result_to_uart, n, 1000);
			  win = 1;
			  started = 0;
		  }
		  if(bullet_sound == 1){
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 10);
			  bullet_sound = 0;
		  }
		  if(enemy_bullet_sound == 1){
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 15);
			PWM_Change_Tone(500, volume);
		  }

	  }
	  if(win == 1){
		  clean_LCD();
		  setCursor(0, 0);
		  print("you won");
		  setCursor(0, 1);
		  int n = sprintf(result_to_uart,"name: %s",name);
		  print(result_to_uart);
		  setCursor(0, 2);
		  n = sprintf(result_to_uart,"score: %d",game_score);
		  print(result_to_uart);
		  win = 0;
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
	  }
	  if(lost == 1){
		  clean_LCD();
		  setCursor(0, 0);
		  print("you lost");
		  setCursor(0, 1);
		  int n = sprintf(result_to_uart,"name: %s",name);
		  print(result_to_uart);
		  game_score = game_score - total_hit;
		  n = sprintf(result_to_uart,"score: %d",game_score);
		  setCursor(0, 2);
		  print(result_to_uart);
		  lost = 0;
		  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
	  }
  }
  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
