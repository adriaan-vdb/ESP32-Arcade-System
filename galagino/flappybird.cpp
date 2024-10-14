

#include "HardwareSerial.h"
#include "flappybird.h"
#include <TFT_eSPI.h>  // Using the TFT_eSPI library for display

#include <SPI.h>
#include <SD.h>

// External display object from the main program
extern TFT_eSPI tft;
extern TFT_eSprite sprite2;

#define SD_CS 10  // Chip Select for SD card
#define SD_SCK 12 // Clock
#define SD_MISO 43 // MISO // 13
#define SD_MOSI 44 // MOSI // 11

const char *filename = "/highscores.txt";
const int MAX_SCORES = 5;
File dataFile;

int playerCount = 1;  // Initialize player counter
int highscore_flag = false;

// High score struct
struct Score {
    char name[10];
    int score;
};

// const char* nameList[] = {"josh", "joshy", "yoshi", "joeline"};

Score highScores[MAX_SCORES]; // Array to store top 5 scores

void printHighScoresToSerial() {
    Serial.println("High Scores:");
    for (int i = 0; i < MAX_SCORES; i++) {
        Serial.print(i + 1);  // Print rank number
        Serial.print(". ");
        Serial.print("Name: ");
        Serial.print(highScores[i].name);  // Print player name
        Serial.print(" - Score: ");
        Serial.println(highScores[i].score);  // Print player score
    }
}


// Function to read the high scores from the SD card
void readHighScores() {
    File file = SD.open(filename, FILE_READ);

    if (!file) {
        Serial.println("Failed to open highscore file. Initializing default scores.");
        for (int i = 0; i < MAX_SCORES; i++) {
            strcpy(highScores[i].name, "p");
            highScores[i].score = 0;  // Initialize default score as 0
        }
        return;
    }

    // If the file exists, read the scores from the file
    int i = 0;
    while (file.available() && i < MAX_SCORES) {
        Serial.println("Reading from SD");
        file.readBytesUntil(',', highScores[i].name, sizeof(highScores[i].name));
        highScores[i].score = file.parseInt();
        file.read();  // Skip newline
        i++;
    }
    file.close();
}

// Function to write high scores to the SD card
void writeHighScores() {
    // Check if the SD card is available and can be written to
    if (!SD.begin(SD_CS)) {
        Serial.println("SD card initialization failed. Cannot write scores.");
        return;
    }

    // Attempt to open the file for writing, this will create the file if it doesn't exist
    File file = SD.open(filename, FILE_WRITE);

    if (!file) {
        Serial.println("Failed to open highscore file for writing.");
        return;
    }


    // Write high scores to the file
    for (int i = 0; i < MAX_SCORES; i++) {
        file.print(highScores[i].name);
        file.print(",");
        file.println(highScores[i].score);
    }

    file.close();  // Close the file after writing
    Serial.println("High scores written successfully.");
}



void addScore(int score) {
    bool playerExists = false;

    // Check if the player already exists in the high score list
    for (int i = 0; i < MAX_SCORES; i++) {
        if (strcmp(highScores[i].name, ("player " + String(playerCount)).c_str()) == 0) {
            playerExists = true;
            Serial.println("Player already exists in high scores.");
            break;
        }
    }

    // If the player does not exist, proceed with adding the score
    if (!playerExists) {
        for (int i = 0; i < MAX_SCORES; i++) {
            if (score > highScores[i].score) {
                // Shift lower scores down
                for (int j = MAX_SCORES - 1; j > i; j--) {
                    highScores[j] = highScores[j - 1];
                }
                // Create a new player name with the current playerCount
                String playerName = "player " + String(playerCount);
                strcpy(highScores[i].name, playerName.c_str());
                highScores[i].score = score;
                writeHighScores(); // Save to SD card

                // Increment the player count for the next new player
                playerCount++;
                Serial.println("New high score added.");
                break;
            }
        }
    }
}

// Display the high scores on the TTGO screen
void displayHighScores() {
    sprite2.setTextColor(TFT_WHITE);
    vga.setTextColor(73, 0);
    sprite2.setTextSize(2);
    sprite2.setCursor(10, 170);
    vga.setCursor(10, 170+30);
    sprite2.setTextColor(TFT_GREEN);
    vga.setTextColor(73, 1);
    sprite2.println("HIGHSCORES:");
    vga.println("HIGHSCORES:");
    
    sprite2.setTextColor(TFT_WHITE);
    vga.setTextColor(73, 0);
    sprite2.setTextSize(1);
    for (int i = 0; i < MAX_SCORES; i++) {
        sprite2.setCursor(10, 200 + i * 20);
        vga.setCursor(10, 200 + i * 20 + 30);
        sprite2.printf("%s: %d", highScores[i].name, highScores[i].score);
        String output = String(highScores[i].name) + ": " + String(highScores[i].score);
        vga.println(String(output).c_str());
    }
    
    sprite2.pushSprite(0, 0); // Push the sprite to the screen
}


// SD card initialization and high score setup
void setupSDandHighScores() {

    SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

    if (!SD.begin(SD_CS)) {
        Serial.println("Card Mount Failed");
        return;
    }
    readHighScores(); 
}




// Game variables
const int gravity = 1;
const int jumpStrength = -7;
int pipeSpeed = 3;
const int gapHeight = 200;
const int pipeWidth = 20;
const int groundHeight = 10;
const int MIN_GAP_HEIGHT = 90;  // Minimum gap height between pipes
const int player_hitpoint = 3;   // Hitbox radius for player bird
const int score_increment = 100;

// Player birds
Bird bird1 = {30, 120, 0, 0};  // Player 1
Bird bird2 = {60, 120, 0, 0};  // Player 2

// Pipe set (two pipes per game round)
Pipe pipe1 = {170, random(60, TFT_HEIGHT - MIN_GAP_HEIGHT - groundHeight)};
Pipe pipeBottom = {170, pipe1.height + MIN_GAP_HEIGHT};  // Bottom pipe always based on top

// Game state
bool gameOver = false;
bool flappyBirdStarted = false;

// Player death tracking
bool player1Dead = false;
bool player2Dead = false;

// Score tracking (obstacles cleared)
int running_score = 0;

// Initialize the Flappy Bird game

void initFlappyBird2P() {
    Serial.println(vgaOn);
    sprite2.setColorDepth(16);  // Set sprite to 16-bit color depth
    sprite2.createSprite(TFT_WIDTH, TFT_HEIGHT);  // Create a sprite matching the display size

    if (vgaOn) vga.setTextColor(73, 0);

    // Reset bird positions and velocities
    bird1 = {30, 120, 0, 0};  // Reset player 1's position and score
    bird2 = {60, 120, 0, 0};  // Reset player 2's position and score

    // Reset pipes
    pipe1 = {170, random(60, TFT_HEIGHT - MIN_GAP_HEIGHT - groundHeight)};
    pipeBottom = {170, pipe1.height + MIN_GAP_HEIGHT};  // Bottom pipe follows the top pipe

    // Reset game state
    gameOver = false;
    flappyBirdStarted = false;
    player1Dead = false;
    player2Dead = false;
    running_score = 0;  // Reset global score
}

// void initFlappyBird2P() {
//     tft.setTextColor(TFT_WHITE);
//     vga.setTextColor(73, 0);
//     tft.setTextSize(2);

//     // Reset bird positions and velocities
//     bird1 = {30, 120, 0, 0};  // Reset player 1's position and score
//     bird2 = {60, 120, 0, 0};  // Reset player 2's position and score

//     // Reset pipes
//     pipe1 = {170, random(60, TFT_HEIGHT - MIN_GAP_HEIGHT - groundHeight)};
//     pipeBottom = {170, pipe1.height + MIN_GAP_HEIGHT};  // Bottom pipe follows the top pipe

//     // Reset game state
//     gameOver = false;
//     flappyBirdStarted = false;
//     player1Dead = false;
//     player2Dead = false;
//     running_score = 0;  // Reset global score
// }

// Handle the bird's movement, gravity, and jump
void updateBird(Bird &bird, bool &playerDead) {
    if (!playerDead && !gameOver) {
        bird.velocity += gravity;
        bird.y += bird.velocity;

        // Collision with the ground
        if (bird.y + player_hitpoint >= TFT_HEIGHT - groundHeight) {
            bird.y = TFT_HEIGHT - groundHeight - player_hitpoint;
            playerDead = true;  // Mark this player as dead
        }
    }
}

// Render the bird on screen
void drawBird(Bird &bird, uint16_t color) {
    sprite2.fillCircle(bird.x, bird.y, 8, color);
    if (color == TFT_BLUE) {
      if (vgaOn) vga.fillCircle(bird.x, bird.y + 30, 8, 8);
    }
    else {
      if (vgaOn) vga.fillCircle(bird.x, bird.y + 30, 8, 64);
    }
}

// Handle pipe movement and rendering
void updatePipes() {

    pipe1.x -= pipeSpeed;
    pipeBottom.x -= pipeSpeed;

    // Reset pipes if they move off the screen
    if (pipe1.x + pipeWidth < 0) {
        pipe1.x = TFT_WIDTH;

        // Ensure the top pipe has a reasonable random height, and the gap is enforced
        pipe1.height = random(60, TFT_HEIGHT - MIN_GAP_HEIGHT - groundHeight);
        pipeBottom.height = pipe1.height + MIN_GAP_HEIGHT;  // Ensure minimum gap

        running_score += score_increment;  // Increment score for each pipe passed
        if (!player1Dead) bird1.score += score_increment;  // Increment player 1 score only if they're alive
        if (!player2Dead) bird2.score += score_increment;  // Increment player 2 score only if they're alive
    }
}

// Render pipes on screen
void drawPipes() {
    sprite2.fillRect(pipe1.x, 0, pipeWidth, pipe1.height, TFT_GREEN);  // Top pipe
    if (vgaOn) vga.fillRect(pipe1.x, 0 + 30, pipeWidth, pipe1.height, 1);
    sprite2.fillRect(pipe1.x, pipe1.height + MIN_GAP_HEIGHT, pipeWidth, TFT_HEIGHT - pipe1.height - MIN_GAP_HEIGHT - groundHeight, TFT_GREEN);  // Bottom pipe
    if (vgaOn) vga.fillRect(pipe1.x, pipe1.height + MIN_GAP_HEIGHT + 30, pipeWidth, TFT_HEIGHT - pipe1.height - MIN_GAP_HEIGHT - groundHeight, 1);
}

// Check for collision with pipes
bool checkCollision(Bird &bird) {
    // Check if bird is within the x-range of the pipe
    if (bird.x + player_hitpoint > pipe1.x && bird.x - player_hitpoint < pipe1.x + pipeWidth) {
        // Check if bird collides with the top pipe or the bottom pipe
        if (bird.y - player_hitpoint < pipe1.height || bird.y + player_hitpoint > pipe1.height + MIN_GAP_HEIGHT) {
            return true;  // Bird collided with the pipe
        }
    }
    return false;
}

// Main logic for the Flappy Bird game (to be called in the loop)
unsigned long lastFrameTime = 0;
unsigned long frameDelay = 1000 / 30;  // Target 30 FPS

void mainFlappyBird2P() {
    if (millis() - lastFrameTime < frameDelay) {
        return;  // Skip the frame if we're running too fast
    }
    lastFrameTime = millis();

    // Clear the sprite instead of the screen
    sprite2.fillSprite(TFT_BLACK);  // Use sprite for double buffering

    if (!flappyBirdStarted) {
        sprite2.setTextColor(TFT_WHITE);
        if (vgaOn) vga.setTextColor(73, 0);
        sprite2.setTextSize(2);
        sprite2.setCursor(10, 10);
        if (vgaOn) vga.setCursor(10, 10+30);
        sprite2.println("Press START");
        if (vgaOn) vga.println("Press START");

        sprite2.pushSprite(0, 0);  // Push the sprite to the screen
        return;
    }
    if (!gameOver) {
        // Update and draw pipes
        if (vgaOn) vga.fillRect(0, 0, 630, 390, 0);
        updatePipes();
        drawPipes();

        // Update and draw birds if they are not dead
        if (!player1Dead) {
            updateBird(bird1, player1Dead);
            drawBird(bird1, TFT_BLUE);
            if (checkCollision(bird1)) player1Dead = true;  // Check collision for player 1
        }
        if (!player2Dead) {
            updateBird(bird2, player2Dead);
            drawBird(bird2, TFT_RED);
            if (checkCollision(bird2)) player2Dead = true;  // Check collision for player 2
        }

        // If both players are dead, end the game
        if (player1Dead && player2Dead) {
            gameOver = true;
            highscore_flag = true;
        }

        // Draw the ground
        sprite2.fillRect(0, TFT_HEIGHT - groundHeight, TFT_WIDTH, groundHeight, TFT_BROWN);

        if(vgaOn) vga.fillRect(0, TFT_HEIGHT - groundHeight+30, TFT_WIDTH, groundHeight, 65);
        if(vgaOn) vga.setTextColor(65, 0);
        if(vgaOn) vga.setCursor(10, 10+30);
        if(vgaOn) vga.println(String(running_score).c_str());

        // Display live score
        sprite2.setTextSize(2);
        sprite2.setTextColor(TFT_YELLOW);
        sprite2.setCursor(150, 10);
        sprite2.setRotation(1);
        sprite2.println(running_score);
        sprite2.setRotation(0);

        sprite2.pushSprite(0, 0);  // Push the entire sprite to the screen
    } else if (highscore_flag) {
        vga.fillRect(0, 0, 630, 390, 0);

        setupSDandHighScores();

        // Display Game Over message
        sprite2.setCursor(5, 10);
        sprite2.setTextColor(TFT_WHITE);
        sprite2.setTextSize(3);
        sprite2.println("Game Over");

        if(vgaOn) vga.setCursor(5, 10+30);
        if(vgaOn) vga.setTextColor(73, 0);
        if(vgaOn) vga.println("Game Over");

        // Display final scores for both players

        sprite2.setTextSize(2);
        sprite2.setCursor(10, 50);
        sprite2.println("P1: " + String(bird1.score));
        sprite2.setCursor(10, 50 + 30);
        sprite2.println("P2: " + String(bird2.score));

        if(vgaOn) vga.setCursor(10, 50+30);
        if(vgaOn) vga.println(("P1: " + String(bird1.score)).c_str());
        if(vgaOn) vga.setCursor(10, 80+30);
        if(vgaOn) vga.println(("P2: " + String(bird2.score)).c_str());

        // DISPLAY HIGHSCORE SCREEN
        int maxScore = max(bird1.score, bird2.score);
        addScore(maxScore);  // Add the score to the leaderboard
        displayHighScores(); // Show updated high scores

        // Press start to restart
        sprite2.setTextSize(1);
        sprite2.setCursor(10, 120);
        sprite2.println("Joystick Up for Menu");

        if(vgaOn) vga.setCursor(10, 120+30);
        if(vgaOn) vga.println("Joystick Up for Menu");

        sprite2.pushSprite(0, 0);  // Push the entire sprite to the screen
        // sprite2.deleteSprite();

        highscore_flag = false;
    }
}


// void mainFlappyBird2P() {
//     // Frame timing
//     // if (running_score > 500) {
//     //     frameDelay = 1000 / 40;
//     // } else {
//     //   frameDelay = 1000 / 30;
//     // }

//     if (millis() - lastFrameTime < frameDelay) {
//         return;  // Skip the frame if we're running too fast
//     }
//     lastFrameTime = millis();

//     if (!flappyBirdStarted) {
//         tft.setTextColor(TFT_WHITE);
//         vga.setTextColor(73, 0);
//         tft.setTextSize(2);
//         tft.setCursor(10, 10);
//         vga.setCursor(10, 10+30);
//         tft.println("Press START");
//         vga.println("Press START");
//         return;
//     }

//     if (!gameOver) {
//         tft.fillScreen(TFT_BLACK);
//         vga.fillRect(0, 0, 630, 390, 0);

//         // Update and draw pipes
//         updatePipes();
//         drawPipes();

//         // Update and draw birds if they are not dead
//         if (!player1Dead) {
//             updateBird(bird1, player1Dead);
//             drawBird(bird1, TFT_BLUE);
//             if (checkCollision(bird1)) player1Dead = true;  // Check collision for player 1
//         }
//         if (!player2Dead) {
//             updateBird(bird2, player2Dead);
//             drawBird(bird2, TFT_RED);
//             if (checkCollision(bird2)) player2Dead = true;  // Check collision for player 2
//         }

//         // If both players are dead, end the game
//         if (player1Dead && player2Dead) {
//             gameOver = true;
//         }

//         // Draw the ground
//         tft.fillRect(0, TFT_HEIGHT - groundHeight, TFT_WIDTH, groundHeight, TFT_BROWN);
//         vga.fillRect(0, TFT_HEIGHT - groundHeight+30, TFT_WIDTH, groundHeight, 65);

//         // Display live score
//         tft.setTextSize(2);
//         tft.setTextColor(TFT_YELLOW);
//         vga.setTextColor(65, 0);
//         tft.setCursor(150, 10);
//         vga.setCursor(10, 10+30);
//         tft.setRotation(1);
//         tft.println(running_score);
//         vga.println(String(running_score).c_str());
//         tft.setRotation(0);

//     } else {
//         // Display Game Over message
//         tft.setCursor(5, 10);
//         vga.setCursor(5, 10+30);
//         tft.setTextColor(TFT_WHITE);
//         vga.setTextColor(73, 0);
//         tft.setTextSize(3);
//         tft.println("Game Over");
//         vga.println("Game Over");

//         // Display final scores for both players
//         tft.setTextSize(2);
//         tft.setCursor(10, 50);
//         vga.setCursor(10, 50+30);
//         tft.println("P1: " + String(bird1.score));
//         vga.println(("P1: " + String(bird1.score)).c_str());
//         tft.setCursor(10, 80);
//         vga.setCursor(10, 80+30);
//         tft.println("P2: " + String(bird2.score));
//         vga.println(("P2: " + String(bird2.score)).c_str());

//         // Press start to restart
//         tft.setTextSize(1);
//         tft.setCursor(10, 120);
//         vga.setCursor(10, 120+30);
//         tft.println("Joystick Up for Menu");
//         vga.println("Joystick Up for Menu");
//     }
// }

// Handle jump for the birds (to be called when a button is pressed)
void birdJump(Bird& bird) {
    if (!gameOver) {
        bird.velocity = jumpStrength;
    }
}

// Call when the game starts
void startGame() {
    if (!flappyBirdStarted) {
        sprite2.deleteSprite();
        flappyBirdStarted = true;
        sprite2.createSprite(TFT_WIDTH, TFT_HEIGHT);  // Create a sprite matching the display size
    }
}