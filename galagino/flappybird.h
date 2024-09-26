#ifndef FLAPPYBIRD_H
#define FLAPPYBIRD_H

#include <TFT_eSPI.h>

#include "VGA.h"
#include <FONT_9x16.h>

extern VGA vga;

#include "flappybird_logo.h"

extern const unsigned short flappybird_logo[];

// Define pins for inputs (modify based on your setup)
#define BTN_START_PIN 14  // Start button pin
#define BTN_JUMP_P1_PIN 12  // Player 1 jump button pin
#define BTN_JUMP_P2_PIN 13  // Player 2 jump button pin

// Game constants
extern const int gravity;
extern const int jumpStrength;
extern int pipeSpeed;
extern const int gapHeight;
extern const int pipeWidth;
extern const int groundHeight;


// Structure for the bird properties
struct Bird {
    int x, y;
    int velocity;
    int score;
};

// Structure for pipe properties
struct Pipe {
    int x, height;
};

extern Bird bird1;  // Declare bird1 as extern
extern Bird bird2;  // Declare bird2 as extern

extern bool gameOver;
extern bool flappyBirdStarted;

// Function declarations
void initFlappyBird2P();
void updateBird(Bird &bird);
void drawBird(Bird &bird, uint16_t color);
void updatePipes();
void drawPipes();
bool checkCollision(Bird &bird, Pipe &pipe);
void mainFlappyBird2P();
void birdJump(Bird &bird);
void startGame();

#endif




// flappybird.h
#ifndef FLAPPYBIRD_H
#define FLAPPYBIRD_H

#include "flappybird_logo.h"

// Function declarations for Flappy Bird 2P game

// Function to initialize the game (to be called in setup)
void initFlappyBird2P();

// Function to run the main game logic (to be called in loop)
void mainFlappyBird2P();

// Any other game-related functions or variables can be declared here

#endif // FLAPPYBIRD_H




// #ifndef FLAPPYBIRD_H
// #define FLAPPYBIRD_H

// #include "flappybird_logo.h"

// // Declare global variables as extern to avoid multiple definitions
// extern int fb_bird1;
// extern int fb_bird2;
// extern int fb_pipes;
// extern int fb_gameRunning;
// extern int fb_gameOver;
// extern int fb_score;

// extern unsigned short *frame_buffer;

// #ifdef __cplusplus
// extern "C" {
// #endif

// void flappybird_reset(void);        // Declare reset function
// // void flappybird_run_frame(void);    // Declare run frame function
// static inline void flappybird_run_frame(void) {
// }
// void render_flappybird(void);       // Declare render function

// #ifdef __cplusplus
// }
// #endif

// #endif // FLAPPYBIRD_H








// /* flappybird.h */

// #ifndef FLAPPYBIRD_H
// #define FLAPPYBIRD_H

// #ifdef IO_EMULATION

// #include <stdlib.h>  // For rand()
// #include <string.h>  // For memset()

// // Game parameters
// #define SCREEN_WIDTH 224
// #define SCREEN_HEIGHT 288

// #define BIRD_WIDTH  8
// #define BIRD_HEIGHT 8

// #define GRAVITY 1
// #define JUMP_VELOCITY -6

// #define PIPE_WIDTH 16
// #define PIPE_GAP   50
// #define PIPE_SPEED 2
// #define PIPE_SPACING 80

// // Game variables
// static int bird_x = SCREEN_WIDTH / 4;
// static int bird_y = SCREEN_HEIGHT / 2;
// static int bird_velocity = 0;

// static unsigned char game_over = 0;

// struct Pipe {
//     int x;
//     int gap_y;
// };

// #define MAX_PIPES 5
// static struct Pipe pipes[MAX_PIPES];

// // Initialize the game
// static inline void flappybird_init(void) {
//     bird_x = SCREEN_WIDTH / 4;
//     bird_y = SCREEN_HEIGHT / 2;
//     bird_velocity = 0;
//     game_over = 0;

//     // Initialize pipes
//     for (int i = 0; i < MAX_PIPES; i++) {
//         pipes[i].x = SCREEN_WIDTH + i * PIPE_SPACING;
//         pipes[i].gap_y = (rand() % (SCREEN_HEIGHT - PIPE_GAP * 2)) + PIPE_GAP;
//     }
// }

// // Update game state
// static inline void flappybird_run_frame(void) {
//     if (game_over) {
//         // Reset the game if the player presses the fire button
//         unsigned char buttons = buttons_get();
//         if (buttons & BUTTON_FIRE) {
//             flappybird_init();
//         }
//         return;
//     }

//     // Get input
//     unsigned char buttons = buttons_get();
//     if (buttons & BUTTON_FIRE) {
//         bird_velocity = JUMP_VELOCITY;
//     }

//     // Update bird position
//     bird_velocity += GRAVITY;
//     bird_y += bird_velocity;

//     // Check for collision with ground or ceiling
//     if (bird_y < 0 || bird_y + BIRD_HEIGHT > SCREEN_HEIGHT) {
//         game_over = 1;
//         return;
//     }

//     // Update pipes
//     for (int i = 0; i < MAX_PIPES; i++) {
//         pipes[i].x -= PIPE_SPEED;
//         if (pipes[i].x < -PIPE_WIDTH) {
//             pipes[i].x = SCREEN_WIDTH;
//             pipes[i].gap_y = (rand() % (SCREEN_HEIGHT - PIPE_GAP * 2)) + PIPE_GAP;
//         }
//     }

//     // Check for collisions with pipes
//     for (int i = 0; i < MAX_PIPES; i++) {
//         if (bird_x + BIRD_WIDTH > pipes[i].x && bird_x < pipes[i].x + PIPE_WIDTH) {
//             if (bird_y < pipes[i].gap_y - PIPE_GAP / 2 || bird_y + BIRD_HEIGHT > pipes[i].gap_y + PIPE_GAP / 2) {
//                 // Collision detected, game over
//                 game_over = 1;
//                 break;
//             }
//         }
//     }
// }

// // Prepare frame data if necessary
// static inline void flappybird_prepare_frame(void) {
//     // Nothing to prepare in this simple implementation
// }

// // Render a row
// static inline void flappybird_render_row(short row) {
//     memset(frame_buffer, 0, 2 * SCREEN_WIDTH * 8); // Clear the frame buffer

//     int y_start = row * 8;
//     int y_end = y_start + 8;

//     // Render bird if in this row
//     if (bird_y + BIRD_HEIGHT > y_start && bird_y < y_end) {
//         int bird_row_start = (bird_y > y_start) ? bird_y : y_start;
//         int bird_row_end = (bird_y + BIRD_HEIGHT < y_end) ? bird_y + BIRD_HEIGHT : y_end;

//         for (int y = bird_row_start; y < bird_row_end; y++) {
//             int fb_y = y - y_start;
//             for (int x = bird_x; x < bird_x + BIRD_WIDTH; x++) {
//                 if (x >= 0 && x < SCREEN_WIDTH) {
//                     frame_buffer[fb_y * SCREEN_WIDTH + x] = 0xFFFF; // White color for bird
//                 }
//             }
//         }
//     }

//     // Render pipes
//     for (int i = 0; i < MAX_PIPES; i++) {
//         // Upper pipe
//         int pipe_top_end = pipes[i].gap_y - PIPE_GAP / 2;
//         if (y_start < pipe_top_end && y_end > 0) {
//             int pipe_row_start = (y_start > 0) ? y_start : 0;
//             int pipe_row_end = (y_end < pipe_top_end) ? y_end : pipe_top_end;
//             for (int y = pipe_row_start; y < pipe_row_end; y++) {
//                 int fb_y = y - y_start;
//                 for (int x = pipes[i].x; x < pipes[i].x + PIPE_WIDTH; x++) {
//                     if (x >= 0 && x < SCREEN_WIDTH) {
//                         frame_buffer[fb_y * SCREEN_WIDTH + x] = 0x07E0; // Green color for pipe
//                     }
//                 }
//             }
//         }
//         // Lower pipe
//         int pipe_bottom_start = pipes[i].gap_y + PIPE_GAP / 2;
//         if (y_end > pipe_bottom_start && y_start < SCREEN_HEIGHT) {
//             int pipe_row_start = (y_start > pipe_bottom_start) ? y_start : pipe_bottom_start;
//             int pipe_row_end = (y_end < SCREEN_HEIGHT) ? y_end : SCREEN_HEIGHT;
//             for (int y = pipe_row_start; y < pipe_row_end; y++) {
//                 int fb_y = y - y_start;
//                 for (int x = pipes[i].x; x < pipes[i].x + PIPE_WIDTH; x++) {
//                     if (x >= 0 && x < SCREEN_WIDTH) {
//                         frame_buffer[fb_y * SCREEN_WIDTH + x] = 0x07E0; // Green color for pipe
//                     }
//                 }
//             }
//         }
//     }

//     // Optionally, render "Game Over" message
//     if (game_over) {
//         // Simple horizontal line in the middle of the screen
//         if (y_start == SCREEN_HEIGHT / 2 - 4) {
//             for (int x = SCREEN_WIDTH / 2 - 40; x < SCREEN_WIDTH / 2 + 40; x++) {
//                 if (x >= 0 && x < SCREEN_WIDTH) {
//                     frame_buffer[(4) * SCREEN_WIDTH + x] = 0xF800; // Red color
//                 }
//             }
//         }
//     }
// }

// #endif // IO_EMULATION

// #endif // FLAPPYBIRD_H

