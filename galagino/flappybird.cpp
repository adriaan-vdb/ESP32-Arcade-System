#include "flappybird.h"
#include <TFT_eSPI.h>  // Using the TFT_eSPI library for display

// External display object from the main program
extern TFT_eSPI tft;

// Game variables
const int gravity = 1;
const int jumpStrength = -7;
int pipeSpeed = 2;
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
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);

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
    tft.fillCircle(bird.x, bird.y, 8, color);
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
    tft.fillRect(pipe1.x, 0, pipeWidth, pipe1.height, TFT_GREEN);  // Top pipe
    tft.fillRect(pipe1.x, pipe1.height + MIN_GAP_HEIGHT, pipeWidth, TFT_HEIGHT - pipe1.height - MIN_GAP_HEIGHT - groundHeight, TFT_GREEN);  // Bottom pipe
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
    // Frame timing
    if (running_score > 500) {
        frameDelay = 1000 / 40;
    } else {
      frameDelay = 1000 / 30;
    }

    if (millis() - lastFrameTime < frameDelay) {
        return;  // Skip the frame if we're running too fast
    }
    lastFrameTime = millis();

    if (!flappyBirdStarted) {
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(2);
        tft.setCursor(10, 10);
        tft.println("Press START");
        return;
    }

    if (!gameOver) {
        tft.fillScreen(TFT_BLACK);

        // Update and draw pipes
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
        tft.fillRect(0, TFT_HEIGHT - groundHeight, TFT_WIDTH, groundHeight, TFT_BROWN);

        // Display live score
        tft.setTextSize(2);
        tft.setTextColor(TFT_YELLOW);
        tft.setCursor(150, 10);
        tft.setRotation(1);
        tft.println(running_score);
        tft.setRotation(0);

    } else {
        // Display Game Over message
        tft.setCursor(5, 10);
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(3);
        tft.println("Game Over");

        // Display final scores for both players
        tft.setTextSize(2);
        tft.setCursor(10, 50);
        tft.println("P1: " + String(bird1.score));
        tft.setCursor(10, 80);
        tft.println("P2: " + String(bird2.score));

        // Press start to restart
        tft.setTextSize(1);
        tft.setCursor(10, 120);
        tft.println("Press Start to Restart");
    }
}

// Handle jump for the birds (to be called when a button is pressed)
void birdJump(Bird& bird) {
    if (!gameOver) {
        bird.velocity = jumpStrength;
    }
}

// Call when the game starts
void startGame() {
    if (!flappyBirdStarted) {
        flappyBirdStarted = true;
    }
}