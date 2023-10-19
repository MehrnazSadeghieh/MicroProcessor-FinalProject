/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef unsigned char byte;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
byte player[] = { 0x10, 0x08, 0x0C, 0x1F, 0x1F, 0x0C, 0x08, 0x10 };
byte enemy[] = { 0x00, 0x00, 0x04, 0x1F, 0x0E, 0x04, 0x00, 0x00 };
byte shot[] = { 0x00, 0x00, 0x0E, 0x0E, 0x0E, 0x0E, 0x00, 0x00 };
int board[20][4];
int x = 1;
int y = 0;
int playershot = 0;
int timer = 0;
int enemy_falling_timer;
extern int enemy_falling_duration;
int total_hit = 1;
int health = 7;
int started = 0;
int menu = 0;
int information = 0;
int entername = 0;
int uartflag = 0;
int choose_mode = 0;
int bullet[10][3];
int fire[5][3];
char name[10] = "";
int game_score = 15;
int stop_menu_music = 0;

extern int in_menu;
extern int in_info;
extern int in_name_entering;
extern int in_game;
extern int in_game_mode;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#include "LiquidCrystal.h"
#include "string.h"
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */

typedef struct {
	uint16_t frequency;
	uint16_t duration;
} Tone;

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0
#define Q2 1000
#define Q4 500
#define Q8 250
#define Q16 125
#define QM1 3000
#define QM2 1250
#define QM4 750
#define QM8 375

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

TIM_HandleTypeDef *pwm_timer = &htim3; // Point to PWM Timer configured in CubeMX
uint32_t pwm_channel = TIM_CHANNEL_2;   // Select configured PWM channel number

const Tone *volatile melody_ptr;
volatile uint16_t melody_tone_count;
volatile uint16_t current_tone_number;
volatile uint32_t current_tone_end;
volatile uint16_t volume = 50;          // (0 - 1000)
volatile uint32_t last_button_press;

const Tone GOT[] = {

{ NOTE_G4, Q8 }, { NOTE_C4, Q8 }, { NOTE_DS4, Q16 }, { NOTE_F4, Q16 }, {
NOTE_G4, Q8 }, { NOTE_C4, Q8 }, { NOTE_DS4, Q16 },
		{ NOTE_F4, Q16 }, //1
		{ NOTE_G4, Q8 }, { NOTE_C4, Q8 }, { NOTE_DS4, Q16 }, { NOTE_F4, Q16 }, {
		NOTE_G4, Q8 }, { NOTE_C4, Q8 }, { NOTE_DS4, Q16 }, { NOTE_F4,
		Q16 }, { NOTE_G4, Q8 }, { NOTE_C4, Q8 }, { NOTE_E4, Q16 }, {
		NOTE_F4, Q16 }, { NOTE_G4, Q8 }, { NOTE_C4, Q8 }, { NOTE_E4, Q16 }, {
		NOTE_F4, Q16 }, { NOTE_G4, Q8 }, { NOTE_C4, Q8 }, {
		NOTE_E4, Q16 }, { NOTE_F4, Q16 }, { NOTE_G4, Q8 }, { NOTE_C4, Q8 }, {
		NOTE_E4, Q16 }, { NOTE_F4, Q16 }, { NOTE_G4, QM4 }, {
		NOTE_C4, QM4 }, { NOTE_DS4, Q16 }, { NOTE_F4, Q16 }, { NOTE_G4,
		Q4 }, { NOTE_C4, Q4 }, { NOTE_DS4, Q16 }, { NOTE_F4, Q16 }, {
		NOTE_D4, QM1 }, { NOTE_F4, QM4 }, { NOTE_AS3, QM4 }, { NOTE_DS4,
		Q16 }, { NOTE_D4, Q16 }, { NOTE_F4, Q4 }, { NOTE_AS3, QM4 }, {
		NOTE_DS4, Q16 }, { NOTE_D4, Q16 },
		{ NOTE_C4, QM1 }, //11 and 12
		{ NOTE_G4, QM4 },
		{ NOTE_C4, QM4 }, //5
		{ NOTE_DS4, Q16 }, { NOTE_F4, Q16 }, { NOTE_G4, Q4 }, { NOTE_C4, Q4 }, {
		NOTE_DS4, Q16 },
		{ NOTE_F4, Q16 }, //6
		{ NOTE_D4, QM1 }, //7 and 8
		{ NOTE_F4, QM4 }, { NOTE_AS3, QM4 }, { NOTE_DS4, Q16 },
		{ NOTE_D4, Q16 }, { NOTE_F4, Q4 }, { NOTE_AS3, QM4 }, { NOTE_DS4, Q16 },
		{ NOTE_D4, Q16 },
		{ NOTE_C4, QM1 }, //11 and 12
		{ NOTE_G4, QM4 }, { NOTE_C4, QM4 }, { NOTE_DS4, Q16 }, { NOTE_F4, Q16 },
		{ NOTE_G4, Q4 }, { NOTE_C4, Q4 }, { NOTE_DS4, Q16 }, { NOTE_F4, Q16 },
		{ NOTE_D4, QM2 }, //15
		{ NOTE_F4, QM4 }, { NOTE_AS3, QM4 }, { NOTE_D4, QM8 },
		{ NOTE_DS4, QM8 }, { NOTE_D4, QM8 }, { NOTE_AS3, QM8 },
		{ NOTE_C4, QM1 }, { NOTE_C5, QM2 }, { NOTE_AS4, QM2 }, { NOTE_C4, QM2 },
		{ NOTE_G4, QM2 }, { NOTE_DS4, QM4 }, { NOTE_DS4, QM4 },
		{ NOTE_F4, QM4 }, { NOTE_G4, QM1 }, { NOTE_C5, QM2 }, { NOTE_AS4, QM2 },
		{ NOTE_C4, QM2 }, { NOTE_G4, QM2 }, { NOTE_DS4, QM2 },
		{ NOTE_DS4, QM4 }, { NOTE_D4, QM4 }, { NOTE_C5, Q8 }, { NOTE_G4, Q8 }, {
		NOTE_GS4, Q16 }, { NOTE_AS4, Q16 }, { NOTE_C5, Q8 }, { NOTE_G4,
		Q8 }, { NOTE_GS4, Q16 }, { NOTE_AS4, Q16 }, { NOTE_C5, Q8 }, {
		NOTE_G4, Q8 }, { NOTE_GS4, Q16 }, { NOTE_AS4, Q16 }, { NOTE_C5,
		Q8 }, { NOTE_G4, Q8 }, { NOTE_GS4, Q16 }, { NOTE_AS4, Q16 }, {
		REST, Q4 }, { NOTE_GS5, Q16 }, { NOTE_AS5, Q16 }, { NOTE_C6, Q8 }, {
		NOTE_G5, Q8 }, { NOTE_GS5, Q16 }, { NOTE_AS5, Q16 }, { NOTE_C6,
		Q8 }, { NOTE_G5, Q16 }, { NOTE_GS5, Q16 }, { NOTE_AS5, Q16 }, {
		NOTE_C6, Q8 }, { NOTE_G5, Q8 }, { NOTE_GS5, Q16 }, { NOTE_AS5,
		Q16 }, { 0, 0 } };

void PWM_Start() {
	HAL_TIM_PWM_Start(pwm_timer, pwm_channel);
}

void PWM_Change_Tone(uint16_t pwm_freq, uint16_t volume) // pwm_freq (1 - 20000), volume (0 - 1000)
{
	if (pwm_freq == 0 || pwm_freq > 20000) {
		__HAL_TIM_SET_COMPARE(pwm_timer, pwm_channel, 0);
	} else {
		const uint32_t internal_clock_freq = HAL_RCC_GetSysClockFreq();
		const uint16_t prescaler = 1 + internal_clock_freq / pwm_freq / 60000;
		const uint32_t timer_clock = internal_clock_freq / prescaler;
		const uint32_t period_cycles = timer_clock / pwm_freq;
		const uint32_t pulse_width = volume * period_cycles / 1000 / 2;

		pwm_timer->Instance->PSC = prescaler - 1;
		pwm_timer->Instance->ARR = period_cycles - 1;
		pwm_timer->Instance->EGR = TIM_EGR_UG;
		__HAL_TIM_SET_COMPARE(pwm_timer, pwm_channel, pulse_width); // pwm_timer->Instance->CCR2 = pulse_width;
	}
}

void Change_Melody(const Tone *melody, uint16_t tone_count) {
	melody_ptr = melody;
	melody_tone_count = tone_count;
	current_tone_number = 0;
}

void Update_Melody() {
	if ((HAL_GetTick() > current_tone_end)
			&& (current_tone_number < melody_tone_count)) {
		const Tone active_tone = *(melody_ptr + current_tone_number);
		PWM_Change_Tone(active_tone.frequency, volume);
		current_tone_end = HAL_GetTick() + active_tone.duration;
		current_tone_number++;
	}
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int key_pad_number = 0;
int board_variable = 0;
// Input pull down rising edge trigger interrupt pins:
// Row1 PD3, Row2 PD5, Row3 PD7, Row4 PB4
void emptyboard() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 4; j++) {
			board[i][j] = 0;
		}
	}
}

void emptyshot() {
	for (int i = 0; i < 10; i++) {
		bullet[i][0] = 0;
		bullet[i][1] = 0;
		bullet[i][2] = 0;
		if (i < 5) {
			fire[i][0] = 0;
			fire[i][1] = 0;
			fire[i][2] = 0;
		}
	}
}

void fill_shot(int x, int y) {
	for (int i = 0; i < 10; i++) {
		if (bullet[i][0] == 0) {
			bullet[i][0] = 1;
			bullet[i][1] = x;
			bullet[i][2] = y;
			break;
		}
	}
}

GPIO_TypeDef *const Row_ports[] = { GPIOD, GPIOD, GPIOD, GPIOD };
const uint16_t Row_pins[] = { GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3 };
// Output pins: Column1 PD4, Column2 PD6, Column3 PB3, Column4 PB5
GPIO_TypeDef *const Column_ports[] = { GPIOD, GPIOD, GPIOD, GPIOD };
const uint16_t Column_pins[] =
		{ GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7 };
volatile uint32_t last_gpio_exti;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (last_gpio_exti + 300 > HAL_GetTick()) // Simple button debouncing
		return;

	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_9);

	int8_t row_number = -1;
	int8_t column_number = -1;

	if (GPIO_Pin == GPIO_PIN_0) {
		// blue_button_pressed = 1;
		// return;
	}

	for (uint8_t row = 0; row < 4; row++) // Loop through Rows
			{
		if (GPIO_Pin == Row_pins[row]) {
			row_number = row;
		}
	}

	HAL_GPIO_WritePin(Column_ports[0], Column_pins[0], 0);
	HAL_GPIO_WritePin(Column_ports[1], Column_pins[1], 0);
	HAL_GPIO_WritePin(Column_ports[2], Column_pins[2], 0);
	HAL_GPIO_WritePin(Column_ports[3], Column_pins[3], 0);

	for (uint8_t col = 0; col < 4; col++) // Loop through Columns
			{
		HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 1);
		if (HAL_GPIO_ReadPin(Row_ports[row_number], Row_pins[row_number])) {
			column_number = col;
		}
		HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 0);
	}

	HAL_GPIO_WritePin(Column_ports[0], Column_pins[0], 1);
	HAL_GPIO_WritePin(Column_ports[1], Column_pins[1], 1);
	HAL_GPIO_WritePin(Column_ports[2], Column_pins[2], 1);
	HAL_GPIO_WritePin(Column_ports[3], Column_pins[3], 1);

	if (row_number == -1 || column_number == -1) {
		return; // Reject invalid scan
	}
	//   C0   C1   C2   C3
	// +----+----+----+----+
	// | 1  | 2  | 3  | 4  |  R0
	// +----+----+----+----+
	// | 5  | 6  | 7  | 8  |  R1
	// +----+----+----+----+
	// | 9  | 10 | 11 | 12 |  R2
	// +----+----+----+----+
	// | 13 | 14 | 15 | 16 |  R3
	// +----+----+----+----+
	const uint8_t button_number = row_number * 4 + column_number + 1;
	switch (button_number) {
	case 1:
		/* code */
		//S13
		//go left
		if (in_game == 1) {
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
			y = 0;
			board[y][x] = 0;
			if (x > 0) {
				x--;
			} else {
				x = 3;
			}
			board[y][x] = 1;
		}

		break;
	case 2:
		/* code */
		//S14
		//go right
		if (in_game == 1) {
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_9);
			y = 0;
			board[y][x] = 0;
			x = (x + 1) % 4;
			board[y][x] = 1;
		}
		break;
	case 3:
		/* code */
		//S15
		if (in_game == 1) {
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_10);
			fill_shot(x, 1);
			playershot = 1;
		}

		break;
	case 4:
		/* code */
		//s16
		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
		menu = 1;
		Change_Melody(GOT, ARRAY_LENGTH(GOT));
		break;
	case 5:
		/* code */
//	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
		break;
	case 6:
		/* code */
//	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
		break;
	case 7:
		/* code */
//	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
		break;
	case 8:
		/* code */
//	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
		break;
	case 9:
		/* code */
		//S5
		if (in_game_mode == 1) {
			enemy_falling_duration = 5000;
			health = 7;
			total_hit = 15;
			game_score = 15;
			unsigned char test[20] = "this is s5\n";
			HAL_UART_Transmit(&huart1, test, sizeof(test), 1000);
		}
//	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
		break;
	case 10:
		/* code */
		//S6
		if (in_game_mode == 1) {
			enemy_falling_duration = 3000;
			health = 5;
			total_hit = 25;
			game_score = 25;
			unsigned char test[20] = "this is s6\n";
			HAL_UART_Transmit(&huart1, test, sizeof(test), 1000);
		}
		//	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
		break;
	case 11:
		/* code */
		//S7
		if (in_game_mode == 1) {
			enemy_falling_duration = 1000;
			health = 3;
			total_hit = 35;
			game_score = 35;
			unsigned char test[20] = "this is s7\n";
			HAL_UART_Transmit(&huart1, test, sizeof(test), 1000);
		}
//	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
		break;
	case 12:
		/* code */
		//S8
//	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
		if (in_game_mode == 1) {
			started = 1;
			in_game_mode = 0;
			unsigned char test[20] = "this is s8\n";
			HAL_UART_Transmit(&huart1, test, sizeof(test), 1000);
//			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
			stop_menu_music = 1;

		}

		break;
	case 13:
		/* code */
		//s1
//	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
//	  started = 1;
		if (in_menu == 1) {
			entername = 1;
			in_menu = 0;
		}
		break;
	case 14:
		/* code */
		//s2
		if (in_menu == 1) {
			information = 1;
			in_menu = 0;
		}
//	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
		break;
	case 15:
		/* code */
		//S3
		if (in_info == 1) {
			information = 0;
			menu = 1;
			in_info = 0;
		}
//	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
		break;
	case 16:
		/* code */
		//S4
		if (in_name_entering == 1) {
			choose_mode = 1;
			in_name_entering = 0;
		}
//	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
		break;

	default:
		break;
	}
	last_gpio_exti = HAL_GetTick();
}

void emptyBuffer(unsigned char b[]) {
	for (int i = 0; i < 30; i++) {
		b[i] = "";
	}
}

int position = 0;
unsigned char data[1];
unsigned char buffer[30] = "";

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		if (data[0] == '/') {
			if (uartflag == 1) {
				unsigned char test[20] = "name recieved\n";
				HAL_UART_Transmit(&huart1, test, sizeof(test), 1000);
				uartflag = 0;
//				name = buffer;
			}
			emptyBuffer(buffer);
			position = 0;
		} else {
			buffer[position] = data[0];
//			buffer[position++] = '\0';
			name[position] = data[0];
			position++;
		}
		HAL_UART_Receive_IT(&huart1, data, sizeof(data));
	}

}

void keypadInit() {
	HAL_GPIO_WritePin(Column_ports[0], Column_pins[0], 1);
	HAL_GPIO_WritePin(Column_ports[1], Column_pins[1], 1);
	HAL_GPIO_WritePin(Column_ports[2], Column_pins[2], 1);
	HAL_GPIO_WritePin(Column_ports[3], Column_pins[3], 1);

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	timer = HAL_GetTick();
//	enemy_falling_timer = HAL_GetTick();
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
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USB_PCD_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
	keypadInit();
	emptyboard();
	emptyshot();
	LiquidCrystal(GPIOD, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11,
	GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14);
	begin(20, 4);
	createChar(1, player);
	createChar(2, enemy);
	createChar(3, shot);
	setCursor(3, 0);
	print("space invaders");
	setCursor(0, 2);
	print("menu(s4)");
	setCursor(0, 3);
//  board[y][x] = 1;
//  setCursor(0, 0);
//  print("hi");
//  setCursor(0, 2);
//  write(1);
//  unsigned char test[10] = "Hi";
//  HAL_UART_Transmit(&huart1, test, sizeof(test), 1000);

	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_UART_Receive_IT(&huart1, data, sizeof(data));

	PWM_Start();
//	Change_Melody(GOT, ARRAY_LENGTH(GOT));
	  HAL_ADC_Start_IT(&hadc1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 15);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC
                              |RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
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

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 39;
  hrtc.Init.SynchPrediv = 999;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

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
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
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

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 4799;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
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
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
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
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

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
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin
                           LD7_Pin LD9_Pin LD10_Pin LD8_Pin
                           LD6_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PC0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD2 PD3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD4 PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_TSC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_TSC_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

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
	while (1) {
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
