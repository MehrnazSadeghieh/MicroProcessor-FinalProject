# MicroProcessor-FinalProject

## Overview

Welcome to the Microprocessor Space Invader Game project! This project is an exciting journey into the world of microprocessors, where we'll design and implement a classic Space Invader game using an STM32F3Discovery board. You'll harness the power of a microprocessor and various hardware components to create an immersive gaming experience.

## Hardware Components

To bring the Space Invader game to life, you'll need the following hardware components:

- **STM32F3Discovery Board**: The heart of your project, responsible for controlling the game.

- **LCD**: A display for showing the game screen, player's information, and messages.

- **Keypad**: Input device for navigating menus, controlling the game character, and shooting.

- **UART**: Used for communication with a computer, enabling the player to enter their name and receive game information.

- **Volume Control**: Adjust the volume of in-game melodies.

- **Buzzer**: Generate sound effects and melodies during gameplay.

- **LED**: Indicate game status and outcomes.

## Project Details

### Game Introduction

- **Menu and Overview**: The game starts with an introduction on the LCD screen. Press a keypad button to enter the main menu.

- **About Game**: Learn more about the game, including real-time date and time using RTC, author names, and return to the main menu.

### Player Setup

- **Enter Your Name**: Use UART to input your name from a computer to the STM32F3Discovery board.

- **Select Difficulty**: Choose a game difficulty level (Easy, Normal, or Hard) and confirm.

### Game On!

- **Game Screen**: The game screen is displayed vertically on the LCD (20x4). You control a spaceship at the bottom row.

- **Player Actions**: Move your spaceship left and right using keypad keys. Shoot arrows with another keypad key.

- **Enemies**: Alien enemies appear at the top rows and descend toward you. Shoot them to earn points.

- **Game Difficulty**: Difficulty settings impact enemy speed, player's health, and the number of enemies to destroy.

### Gameplay and Messages

- **UART Messages**: Send messages to the computer through UART:
  - Game start information (difficulty level and goal).
  - Warnings when the player is close to losing.
  - Encouraging messages when only a few enemies remain.
  - End game message (victory or defeat).

- **Sound Effects**: Play sounds using the buzzer for shooting arrows and enemy destruction.

- **Custom Characters**: I Designed custom characters for the game elements such as spaceship, arrows and enemies.

### Game Over

- **End Game**: The game ends when you are defeated or reach the goal based on the chosen difficulty.

- **Outcome Message**: Display a message on the LCD, including the player's name and earned points.

- **LED Indicator**: An external LED lights up to indicate the game's outcome.

## Usage

To enjoy the Space Invader game:

1. Connect the required hardware components to the STM32F3Discovery board.
2. Load the game software onto the board.
3. Follow the on-screen instructions and use the keypad to navigate menus and control the game character.
4. Play the game, shoot enemies, and aim for victory.

## Note

You can see my demo video to see the project overview visually.

## Contributors

This project is created and maintained by [Mehrnaz Sadeghieh]. Contributions and enhancements are welcome.

---

Dive into the world of microprocessors and gaming with the Space Invader game. Have fun defending Earth from alien invaders!
