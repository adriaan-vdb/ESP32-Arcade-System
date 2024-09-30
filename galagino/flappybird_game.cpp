// #include "config.h"
// #include "flappybird.h"
// #include <TFT_eSPI.h>

// // TFT display instance
// TFT_eSPI tft = TFT_eSPI(TFT_WIDTH, TFT_HEIGHT);

// // Global variables
// int currentProgram = 0;  // 0 for other programs, 1 for Flappy Bird
// bool flappyBirdStarted = false;

// // Button debounce variables for each player
// unsigned long lastDebounceTimeP1 = 0;
// unsigned long lastDebounceTimeP2 = 0;
// unsigned long lastDebounceTimeStart = 0;
// unsigned long debounceDelay = 50;

// int lastButtonStateP1 = HIGH;
// int lastButtonStateP2 = HIGH;
// int lastButtonStateStart = HIGH;

// // Forward declarations for the functions used in the main program
// void handleInput();
// bool isButtonPressed(int pin, unsigned long &lastDebounceTime, int &lastButtonState);

// // setup function
// void setup() {
//   // Initialize the display
//   tft.init();
//   tft.fillScreen(TFT_BLACK);

//   // Initialize buttons as input
//   pinMode(BTN_LEFT_PIN, INPUT_PULLUP);  // Player 1 jump
//   pinMode(BTN_RIGHT_PIN, INPUT_PULLUP); // Player 2 jump
//   pinMode(BTN_START_PIN, INPUT_PULLUP); // Start button

//   // Initialize the current program
//   if (currentProgram == 1) {
//     setupFlappyBird2P();  // Initialize Flappy Bird 2P game
//   } else {
//     // Other programs' setup can be added here
//   }
// }

// // loop function
// void loop() {
//   // Check if Flappy Bird is the current program
//   if (currentProgram == 1) {
//     // Handle button inputs and game logic
//     handleInput();   // Process button inputs
//     mainFlappyBird2P();  // Run the main Flappy Bird game loop
//   } else {
//     // Code for other programs can go here
//   }
// }

// // Handle input for both players and the start button
// void handleInput() {
//   // Player 1 jump (left button)
//   if (isButtonPressed(BTN_LEFT_PIN, lastDebounceTimeP1, lastButtonStateP1)) {
//     birdJump(bird1);  // Player 1's bird jumps
//   }

//   // Player 2 jump (right button)
//   if (isButtonPressed(BTN_RIGHT_PIN, lastDebounceTimeP2, lastButtonStateP2)) {
//     birdJump(bird2);  // Player 2's bird jumps
//   }

//   // Start button press to start the game or restart after game over
//   if (isButtonPressed(BTN_START_PIN, lastDebounceTimeStart, lastButtonStateStart)) {
//     if (!flappyBirdStarted) {
//       startGame();  // Start the game if it hasn't started yet
//       flappyBirdStarted = true;
//     } else if (gameOver) {
//       initFlappyBird2P();  // Restart the game if game over
//     }
//   }
// }

// // Function to check if a button was pressed, with debounce logic
// bool isButtonPressed(int pin, unsigned long &lastDebounceTime, int &lastButtonState) {
//   int reading = digitalRead(pin);

//   if (reading != lastButtonState) {
//     lastDebounceTime = millis();
//   }

//   if ((millis() - lastDebounceTime) > debounceDelay) {
//     if (reading == LOW) {  // Button is pressed (active-low)
//       lastButtonState = reading;
//       return true;
//     }
//   }

//   lastButtonState = reading;
//   return false;
// }
