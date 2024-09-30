#include "HardwareSerial.h"
#include "flappybird.h"
#include <TFT_eSPI.h>  // Using the TFT_eSPI library for display

// External display object from the main program
extern TFT_eSPI tft;

extern TFT_eSprite sprite2;

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
        sprite2.println(running_score);

        sprite2.pushSprite(0, 0);  // Push the entire sprite to the screen
    } else {
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
        sprite2.setCursor(10, 80);
        sprite2.println("P2: " + String(bird2.score));

        if(vgaOn) vga.setCursor(10, 50+30);
        if(vgaOn) vga.println(("P1: " + String(bird1.score)).c_str());
        if(vgaOn) vga.setCursor(10, 80+30);
        if(vgaOn) vga.println(("P2: " + String(bird2.score)).c_str());

        // Press start to restart
        sprite2.setTextSize(1);
        sprite2.setCursor(10, 120);
        sprite2.println("Joystick Up for Menu");

        if(vgaOn) vga.setCursor(10, 120+30);
        if(vgaOn) vga.println("Joystick Up for Menu");

        sprite2.pushSprite(0, 0);  // Push the entire sprite to the screen
        // sprite2.deleteSprite();
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