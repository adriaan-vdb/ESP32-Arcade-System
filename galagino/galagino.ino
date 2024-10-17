/*
 * Galagino.ino - Galaga arcade for ESP32 and Arduino IDE
 *
 * (c) 2023 Till Harbaum <till@harbaum.org>
 * 
 * Published under GPLv3
 *
 */

// AV
// https://github.com/harbaum/galagino
// commented out leds in this file and commented out entire leds.h file.


// AV add our own program to switch between that and galagino
#define SWITCH_BUTTON_PIN 14
#define NUM_PROGRAMS 3
int tonePin = 18;
int currentProgram = 0;  // 0 for Galagino, 1 for 2P Mode, 2 for the Menu System
// Galagino is the fancy arcade emulator
// 2P Mode is our own flappy bird game
// The Menu system will be used to control:
    // Sound On/Off
    // Display Scaling?
    // Controls Explanation

#define FLAPPY_BIRD_SWITCH_BUTTON_PIN 14
bool inMenu = true;  // Start in the menu
#include "flappybird.h"
#define BTN_START_PIN 0  // Example Start button pin (can map to same as P1/P2 - depending if our start button is right or left)
#define BTN_JUMP_P1_PIN 0  // Example Player 1 Jump button pin
#define BTN_JUMP_P2_PIN 0  // Example Player 2 Jump button pin


bool soundOn = true;  // Initial state of sound (true = ON, false = OFF)
#define SOUND_TOGGLE_BUTTON_PIN 0  // Pin for the button to toggle sound



// JT VGA STUFF

#include "VGA.h"
#include <FONT_9x16.h>

// AV SD CARD READING
#include <SPI.h>
#include <SD.h>

#include <esp_now.h>
#include <WiFi.h>

// AUDIO THINGS

/* 
  The legend of Zelda theme 
  Connect a piezo buzzer or speaker to pin 11 or select a new pin.
  More songs available at https://github.com/robsoncouto/arduino-songs                                            
                                              
                                              Robson Couto, 2019
*/

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
#define REST 0

int tempo=250; 

// change this to whichever pin you want to use
int buzzer = 18;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  //Based on the arrangement at https://www.flutetunes.com/tunes.php?id=192
  
  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,

  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,
  

  NOTE_E5,2,  NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,2,   NOTE_A4,2,
  NOTE_GS4,2,  NOTE_B4,4,  REST,8, 
  NOTE_E5,2,   NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,4,   NOTE_E5,4,  NOTE_A5,2,
  NOTE_GS5,2,

};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes=sizeof(melody)/sizeof(melody[0])/2; 

// this calculates the duration of a whole note in ms (60s/tempo)*4 beats
int wholenote = (60000 * 4) / tempo;

// int divider = 0, noteDuration = 0;

unsigned long previousMillis = 0;  // Stores the last time we updated the note
int thisNote = 0;                  // Tracks the current note being played
unsigned long noteDuration = 0;     // Stores the duration of the current note
int divider = 0;                    // Divider for note duration calculation
bool isPlayingNote = false;         // Tracks whether a note is currently being played

typedef struct struct_message {
    int joystick_x_value = 0;
    int joystick_y_value = 0;
    int joystick_sw_value = 1;
    int button_1_value = 1;
    int button_2_value = 1;
} struct_message;

int second_controller_button = 1;

typedef struct struct_message_raw {
    int joystick_x_value = 0;
    int joystick_y_value = 0;
    int joystick_sw_value = 1;
    int button_1_value = 1;
    int button_2_value = 1;
    int main_controller; // 1 if main controller; 0 if not
} struct_message_raw;

// Create a struct_message called myData
struct_message myData;
struct_message_raw myData_raw;

VGA vga;

int scale = 2;

// Pin numbers for joystick x, y and switch; originally, 1 2 3
const int joystick_x = 18;
const int joystick_y = 17;
const int joystick_sw = 16;

// Pin numbers for buttons; originally 11 10
const int button_1 = 13;
const int button_2 = 12;

// END JT VGA STUFF


#include "config.h"
#include <TFT_eSPI.h>

#include "driver/i2s.h"
#include "video.h"
// #include "leds.h"

#include "emulation.h"

#include "tileaddr.h"

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 4, 5)
// See https://github.com/espressif/arduino-esp32/issues/8467
#define WORKAROUND_I2S_APLL_PROBLEM
// as a workaround we run dkong audio also at 24khz (instead of 11765)
// and fill the buffer at half the rate resulting in 12khz effective
// sample rate
unsigned char dkong_obuf_toggle = 0;
#endif

#define IO_EMULATION

TFT_eSPI tft = TFT_eSPI(170, 320); // Init screen size
TFT_eSprite sprite2 = TFT_eSprite(&tft);  // Define sprite here

// AV to scale image - I think it won't work unless multiple of 2 (8)
float scaleFactor = min(170.0 / 224.0, 320.0 / 288.0);  


// the hardware supports 64 sprites
unsigned char active_sprites = 0;
struct sprite_S *sprite;

// AV
struct sprite_S {
  unsigned char code, color, flags;
  short x, y; 
};

// buffer space for one row of 28 characters
unsigned short *frame_buffer;

#ifdef NUNCHUCK_INPUT
#include "Nunchuck.h"
#endif

// AV Flappy bird 2P
#ifdef ENABLE_FLAPPYBIRD
#include "flappybird.h"
#endif

// include converted rom data
#ifdef ENABLE_PACMAN
#include "pacman.h"
#endif

#ifdef ENABLE_GALAGA
#include "galaga.h"
#endif

#ifdef ENABLE_DKONG
#include "dkong.h"
#endif

#ifdef ENABLE_FROGGER
#include "frogger.h"
#endif

#ifdef ENABLE_DIGDUG
#include "digdug.h"
#endif

#ifdef ENABLE_1942
#include "1942.h"
#endif

#ifndef SINGLE_MACHINE
signed char machine = MCH_MENU;   // start with menu
#endif

// AV uncommented to switch to the TFT_eSPI library for the LilyGo T-Display S3
// instance of main tft driver
// Video tft = Video();

TaskHandle_t emulationtask;

#ifdef ENABLE_GALAGA
// the ship explosion sound is stored as a digi sample.
// All other sounds are generated on the fly via the
// original wave tables
unsigned short snd_boom_cnt = 0;
const signed char *snd_boom_ptr = NULL;
#endif

#ifdef ENABLE_DKONG
unsigned short dkong_sample_cnt[3] = { 0,0,0 };
const signed char *dkong_sample_ptr[3];
#endif

#if defined(ENABLE_FROGGER) || defined(ENABLE_1942)
int ay_period[2][4] = {{0,0,0,0}, {0,0,0,0}};
int ay_volume[2][3] = {{0,0,0}, {0,0,0}};
int ay_enable[2][3] = {{0,0,0}, {0,0,0}};
int audio_cnt[2][4], audio_toggle[2][4] = {{1,1,1,1},{1,1,1,1}};
unsigned long ay_noise_rng[2] = { 1, 1 };
extern unsigned char soundregs[];
#endif

// one method to return to the main menu is to reset
// the entire machine. The main disadvantage is a 
// short noise from the speaker during reset
extern "C" void hw_reset(void);
void hw_reset(void) {
  ESP.restart();
}

#ifndef SINGLE_MACHINE
// convert rgb565 big endian color to greyscale
unsigned short greyscale(unsigned short in) {
  unsigned short r = (in>>3) & 31;
  unsigned short g = ((in<<3) & 0x38) | ((in>>13)&0x07);
  unsigned short b = (in>>8)& 31;
  unsigned short avg = (2*r + g + 2*b)/4;
  
  return (((avg << 13) & 0xe000) |   // g2-g0
          ((avg <<  7) & 0x1f00) |   // b5-b0
          ((avg <<  2) & 0x00f8) |   // r5-r0
          ((avg >>  3) & 0x0007));   // g5-g3
}

// JT VGA STUFF

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  // Serial.println("RECEIVED");
  memcpy(&myData_raw, incomingData, sizeof(myData_raw));

  if (myData_raw.main_controller) {
    myData.joystick_x_value = myData_raw.joystick_x_value;
    myData.joystick_y_value = myData_raw.joystick_y_value;
    myData.joystick_sw_value = myData_raw.joystick_sw_value;
    myData.button_1_value = myData_raw.button_1_value;
    myData.button_2_value = myData_raw.button_2_value;
  }
  else {
    second_controller_button = myData_raw.button_1_value;
  }
  // Serial.print("Data received: ");
  // Serial.println(myData.joystick_x_value);
  // Serial.println(myData.joystick_y_value);
  // Serial.println(myData.joystick_sw_value);
  // Serial.println(myData.button_1_value);
  // Serial.println(myData.button_2_value);
  // Serial.println();
}

int getHue(int red, int green, int blue) {
    float min = std::min(std::min(red, green), blue);
    float max = std::max(std::max(red, green), blue);

    if (min == max) {
        return 0;  // Grayscale, no hue
    }

    float hue = 0.0f;

    if (max == red) {
        hue = (green - blue) / (max - min);
    } else if (max == green) {
        hue = 2.0f + (blue - red) / (max - min);
    } else {  // max == blue
        hue = 4.0f + (red - green) / (max - min);
    }

    hue = hue * 60.0f;
    if (hue < 0) hue += 360.0f;

    return static_cast<int>(round(hue));
}

// Example function to convert 24-bit RGB to 8-bit color
uint8_t convertTo8BitColor(uint8_t r, uint8_t g, uint8_t b) {
    // This is a simple example. You might need a more sophisticated mapping.
    // For example, you could use a predefined palette.
    return (r & 0xE0) | ((g & 0xE0) >> 3) | (b >> 6);
}

void processAudio() {
  // Get the current time
  unsigned long currentMillis = millis();
  
  if (!isPlayingNote) {
    // If we're not currently playing a note, it's time to calculate the duration and play it
    if (thisNote < notes * 2) {
      // Calculates the duration of each note
      divider = melody[thisNote + 1];
      if (divider > 0) {
        // Regular note, just proceed
        noteDuration = (wholenote) / divider;
      } else if (divider < 0) {
        // Dotted notes are represented with negative durations
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5; // Increases the duration by half for dotted notes
      }
      noTone(buzzer);
      // Play the note for 90% of the duration
      tone(buzzer, melody[thisNote], noteDuration * 0.9);

      // Store the time when the note started playing
      previousMillis = currentMillis;

      // Indicate that a note is now playing
      isPlayingNote = true;
    }
  } else {
    // If we're in the middle of playing a note, check if enough time has passed
    if (currentMillis - previousMillis >= noteDuration) {
      // Stop the waveform generation before the next note
      // noTone(buzzer);

      // Move to the next note
      thisNote += 2;

      if (thisNote >= notes * 2) {
        thisNote = 0;  // Reset to start the melody again
      }

      // Reset the flag to indicate the note has finished playing
      isPlayingNote = false;
    }
  }
}

void VGAWrite(uint16_t* frame_buffer, uint32_t width, int y_offset) {
    // Iterate over each pixel in the line
    for (uint32_t i = 0; i < width; i++) {
        uint16_t color = frame_buffer[i];

        // Extract RGB components from 16-bit color
        uint8_t r = (color >> 11) & 0x1F;
        uint8_t g = (color >> 5) & 0x3F;
        uint8_t b = color & 0x1F;
        
        // Scale the RGB components to 8-bit values
        r = (r * 255) / 31;
        g = (g * 255) / 63;
        b = (b * 255) / 31;

        r = (r > 127) ? 1 : 0;
        g = (g > 127) ? 1 : 0;
        b = (b > 127) ? 1 : 0;
        
        

        
        // Convert to 8-bit color (this step depends on your palette)
        // Here, we just return a simple combination for demonstration purposes
        // return ;
// }
        
//         // Extract and scale RGB components from 16-bit color
//         uint8_t r = ((color >> 11) & 0x1F) * 255 / 31; // Extract red component
//         uint8_t g = ((color >> 5) & 0x3F) * 255 / 63;  // Extract green component
//         uint8_t b = (color & 0x1F) * 255 / 31;         // Extract blue component

        // Calculate the x and y positions
        int x = i % 224;  // Assuming the screen width is 224 pixels
        int y = i / 224 + y_offset + 30;

        // Draw the pixel using vga.dot() with the 8-bit color
        vga.dot(x, y, r * 1 + g * 8 + b * 64);
    }
}

uint8_t convertTo3BitColor(uint8_t r, uint8_t g, uint8_t b) {
    // Convert 8-bit RGB values to 1-bit values
    uint8_t r_bit = (r > 127) ? 1 : 0;
    uint8_t g_bit = (g > 127) ? 1 : 0;
    uint8_t b_bit = (b > 127) ? 1 : 0;

    // Combine the bits into a single 3-bit value
    return (r_bit << 2) | (g_bit << 1) | b_bit;
}

// render one of three the menu logos. Only the active one is colorful
// render logo into current buffer starting with line "row" of the logo
void render_logo(short row, const unsigned short *logo, char active) {
  unsigned short marker = logo[0];
  const unsigned short *data = logo+1;

  // current pixel to be drawn
  unsigned short ipix = 0;
    
  // less than 8 rows in image left?
  unsigned short pix2draw = ((row <= 96-8)?(224*8):((96-row)*224));
  
  if(row >= 0) {
    // skip ahead to row
    unsigned short col = 0;
    unsigned short pix = 0;
    while(pix < 224*row) {
      if(data[0] != marker) {
        pix++;
        data++;
      } else {
        pix += data[1]+1;
        col = data[2];
        data += 3;
      }
    }
    
    // draw pixels remaining from previous run
    if(!active) col = greyscale(col);
    while(ipix < ((pix - 224*row < pix2draw)?(pix - 224*row):pix2draw))
      frame_buffer[ipix++] = col;
  } else
    // if row is negative, then skip target pixel
    ipix -= row * 224;
    
  while(ipix < pix2draw) {
    if(data[0] != marker)
      frame_buffer[ipix++] = active?*data++:greyscale(*data++);
    else {
      unsigned short color = data[2];
      if(!active) color = greyscale(color);
      for(unsigned short j=0;j<data[1]+1 && ipix < pix2draw;j++)
        frame_buffer[ipix++] = color;

      data += 3;
    }
  }  
}
#endif

#ifndef SINGLE_MACHINE
// menu for more than three machines
const unsigned short *logos[] = {
#ifdef ENABLE_PACMAN    
  pacman_logo,
#endif
#ifdef ENABLE_GALAGA
  galaga_logo,
#endif
#ifdef ENABLE_DKONG    
  dkong_logo,
#endif
#ifdef ENABLE_FROGGER    
  frogger_logo,
#endif
#ifdef ENABLE_DIGDUG    
  digdug_logo,
#endif
#ifdef ENABLE_1942
  _1942_logo,
#endif
};
#endif

// render one of 36 tile rows (8 x 224 pixel lines)
void render_line(short row) {
  // the upper screen half of frogger has a blue background
  // using 8 in fact adds a tiny fraction of red as well. But that does not hurt
  memset(frame_buffer, 
#ifdef ENABLE_FROGGER 
    (MACHINE_IS_FROGGER && row <= 17)?8:
#endif
    0, 2*224*8);

#ifndef SINGLE_MACHINE
  if(machine == MCH_MENU) {

    if(MACHINES <= 3) {    
      // non-scrolling menu for 2 or 3 machines
      for(char i=0;i<sizeof(logos)/sizeof(unsigned short*);i++) {
      	char offset = i*12;
	      if(sizeof(logos)/sizeof(unsigned short*) == 2) offset += 6;
	
	      if(row >= offset && row < offset+12)  
	        render_logo(8*(row-offset), logos[i], menu_sel == i+1);
      }
    } else {
      // scrolling menu for more than 3 machines
    
      // valid offset values range from 0 to MACHINE*96-1
      static int offset = 0;

      // check which logo would show up in this row. Actually
      // two may show up in the same character row when scrolling
      int logo_idx = ((row + offset/8) / 12)%MACHINES;
      if(logo_idx < 0) logo_idx += MACHINES;
      
      int logo_y = (row * 8 + offset)%96;  // logo line in this row
      
      // check if logo at logo_y shows up in current row
      render_logo(logo_y, logos[logo_idx], (menu_sel-1) == logo_idx);
      
      // check if a second logo may show up here
      if(logo_y > (96-8)) {
        logo_idx = (logo_idx + 1)%MACHINES;
        logo_y -= 96;
        render_logo(logo_y, logos[logo_idx], (menu_sel-1) == logo_idx);
      }
      
      if(row == 35) {
      	// finally offset is bound to game, something like 96*game:    
	      int new_offset = 96*((unsigned)(menu_sel-2)%MACHINES);
	      if(menu_sel == 1) new_offset = (MACHINES-1)*96;
	
	      // check if we need to scroll
	      if(new_offset != offset) {
	        int diff = (new_offset - offset) % (MACHINES*96);
	        if(diff < 0) diff += MACHINES*96;
	  
	        if(diff < MACHINES*96/2) offset = (offset+8)%(MACHINES*96);
	        else                     offset = (offset-8)%(MACHINES*96);
	        if(offset < 0) offset += MACHINES*96;
      	}
      }
    }
  } else
#endif  

#ifdef ENABLE_PACMAN
PACMAN_BEGIN
  pacman_render_row(row);
PACMAN_END
#endif
  
#ifdef ENABLE_GALAGA
GALAGA_BEGIN
  galaga_render_row(row);
GALAGA_END
#endif

#ifdef ENABLE_DKONG
DKONG_BEGIN
  dkong_render_row(row);
DKONG_END
#endif

#ifdef ENABLE_FROGGER
FROGGER_BEGIN
  frogger_render_row(row);
FROGGER_END
#endif
  
#ifdef ENABLE_DIGDUG
DIGDUG_BEGIN
  digdug_render_row(row);
DIGDUG_END
#endif

#ifdef ENABLE_1942
_1942_BEGIN
  _1942_render_row(row);
_1942_END
#endif
}
  
#ifdef ENABLE_GALAGA
void galaga_trigger_sound_explosion(void) {
  if(game_started) {
    snd_boom_cnt = 2*sizeof(galaga_sample_boom);
    snd_boom_ptr = (const signed char*)galaga_sample_boom;
  }
}
#endif

#ifdef USE_NAMCO_WAVETABLE
static unsigned long snd_cnt[3] = { 0,0,0 };
static unsigned long snd_freq[3];
static const signed char *snd_wave[3];
static unsigned char snd_volume[3];
#endif

#ifdef SND_DIFF
static unsigned short snd_buffer[128]; // buffer space for two channels
#else
static unsigned short snd_buffer[64];  // buffer space for a single channel
#endif

void snd_render_buffer(void) {
#if defined(ENABLE_FROGGER) || defined(ENABLE_1942)
  #ifndef ENABLE_1942        // only frogger
    #define AY        1      // frogger has one AY
    #define AY_INC    9      // and it runs at 1.78 MHz -> 223718/24000 = 9,32
    #define AY_VOL   11      // min/max = -/+ 3*15*11 = -/+ 495
  #else
    #ifndef ENABLE_FROGGER   // only 1942  
      #define AY      2      // 1942 has two AYs
      #define AY_INC  8      // and they runs at 1.5 MHz -> 187500/24000 = 7,81
      #define AY_VOL 10      // min/max = -/+ 6*15*11 = -/+ 990
    #else
      // both enabled
      #define AY ((machine == MCH_FROGGER)?1:2)
      #define AY_INC ((machine == MCH_FROGGER)?9:8)
      #define AY_VOL ((machine == MCH_FROGGER)?11:10)
    #endif
  #endif
  
  if(
#ifdef ENABLE_FROGGER
     MACHINE_IS_FROGGER ||
#endif
#ifdef ENABLE_1942
     MACHINE_IS_1942 ||
#endif
     0) {

    // up to two AY's
    for(char ay=0;ay<AY;ay++) {
      int ay_off = 16*ay;

      // three tone channels
      for(char c=0;c<3;c++) {
	ay_period[ay][c] = soundregs[ay_off+2*c] + 256 * (soundregs[ay_off+2*c+1] & 15);
	ay_enable[ay][c] = (((soundregs[ay_off+7] >> c)&1) | ((soundregs[ay_off+7] >> (c+2))&2))^3;
	ay_volume[ay][c] = soundregs[ay_off+8+c] & 0x0f;
      }
      // noise channel
      ay_period[ay][3] = soundregs[ay_off+6] & 0x1f;
    }
  }
#endif

  // render first buffer contents
  for(int i=0;i<64;i++) {
    short v = 0;

#if defined(ENABLE_PACMAN) || defined(ENABLE_GALAGA)
  #ifndef SINGLE_MACHINE
    if(0
    #ifdef ENABLE_PACMAN
        || (machine == MCH_PACMAN)
    #endif
    #ifdef ENABLE_GALAGA
        || (machine == MCH_GALAGA)
    #endif
    #ifdef ENABLE_DIGDUG
        || (machine == MCH_DIGDUG)
    #endif
    ) 
  #endif
    {
      // add up to three wave signals
      if(snd_volume[0]) v += snd_volume[0] * snd_wave[0][(snd_cnt[0]>>13) & 0x1f];
      if(snd_volume[1]) v += snd_volume[1] * snd_wave[1][(snd_cnt[1]>>13) & 0x1f];
      if(snd_volume[2]) v += snd_volume[2] * snd_wave[2][(snd_cnt[2]>>13) & 0x1f];

  #ifdef ENABLE_GALAGA
      if(snd_boom_cnt) {
        v += *snd_boom_ptr;
        if(snd_boom_cnt & 1) snd_boom_ptr++;
        snd_boom_cnt--;
      }
  #endif
    }
#endif    

#ifdef ENABLE_DKONG
DKONG_BEGIN
    {
      v = 0;  // silence

      // no buffer available
      if(dkong_audio_rptr != dkong_audio_wptr)
        // copy data from dkong buffer into tx buffer
        // 8048 sounds gets 50% of the available volume range
#ifdef WORKAROUND_I2S_APLL_PROBLEM
        v = dkong_audio_transfer_buffer[dkong_audio_rptr][(dkong_obuf_toggle?32:0)+(i/2)];
#else
        v = dkong_audio_transfer_buffer[dkong_audio_rptr][i];
#endif
      // include sample sounds
      // walk is 6.25% volume, jump is at 12.5% volume and, stomp is at 25%
      for(char j=0;j<3;j++) {
        if(dkong_sample_cnt[j]) {
#ifdef WORKAROUND_I2S_APLL_PROBLEM
          v += *dkong_sample_ptr[j] >> (2-j); 
          if(i & 1) { // advance read pointer every second sample
            dkong_sample_ptr[j]++;
            dkong_sample_cnt[j]--;
          }
#else
          v += *dkong_sample_ptr[j]++ >> (2-j); 
          dkong_sample_cnt[j]--;
#endif
        }
      }
    }
DKONG_END
#endif

#if defined(ENABLE_FROGGER) || defined(ENABLE_1942)
    if(
#ifdef ENABLE_FROGGER
     MACHINE_IS_FROGGER ||
#endif
#ifdef ENABLE_1942
     MACHINE_IS_1942 ||
#endif
     0) {
    v = 0;  // silence
    
    for(char ay=0;ay<AY;ay++) {

      // frogger can acually skip the noise generator as
      // it doesn't use it      
      if(ay_period[ay][3]) {
	      // process noise generator
	      audio_cnt[ay][3] += AY_INC; // for 24 khz
	      if(audio_cnt[ay][3] > ay_period[ay][3]) {
	        audio_cnt[ay][3] -=  ay_period[ay][3];
	        // progress rng
	        ay_noise_rng[ay] ^= (((ay_noise_rng[ay] & 1) ^ ((ay_noise_rng[ay] >> 3) & 1)) << 17);
	        ay_noise_rng[ay] >>= 1;
	      }
      }
	
      for(char c=0;c<3;c++) {
	      // a channel is on if period != 0, vol != 0 and tone bit == 0
	      if(ay_period[ay][c] && ay_volume[ay][c] && ay_enable[ay][c]) {
	        short bit = 1;
	        if(ay_enable[ay][c] & 1) bit &= (audio_toggle[ay][c]>0)?1:0;  // tone
	        if(ay_enable[ay][c] & 2) bit &= (ay_noise_rng[ay]&1)?1:0;     // noise
	  
	        if(bit == 0) bit = -1;
	        v += AY_VOL * bit * ay_volume[ay][c];
	  
	        audio_cnt[ay][c] += AY_INC; // for 24 khz
	        if(audio_cnt[ay][c] > ay_period[ay][c]) {
	          audio_cnt[ay][c] -= ay_period[ay][c];
	          audio_toggle[ay][c] = -audio_toggle[ay][c];
	        }
	      }
      }
    }
    }
#endif
    // v is now in the range of +/- 512, so expand to +/- 15 bit
    v = v*64;

#ifdef SND_DIFF
    // generate differential output
    snd_buffer[2*i]   = 0x8000 + v;    // positive signal on GPIO26
    snd_buffer[2*i+1] = 0x8000 - v;    // negatve signal on GPIO25 
#else
    // work-around weird byte order bug, see 
    // https://github.com/espressif/arduino-esp32/issues/8467#issuecomment-1656616015
    snd_buffer[i^1]   = 0x8000 + v;
#endif
      
#if defined(ENABLE_PACMAN) || defined(ENABLE_GALAGA)
    snd_cnt[0] += snd_freq[0];
    snd_cnt[1] += snd_freq[1];
    snd_cnt[2] += snd_freq[2];
#endif
  }
  
#ifdef ENABLE_DKONG
  #ifndef SINGLE_MACHINE
  if(machine == MCH_DKONG)
  #endif
  {
#ifdef WORKAROUND_I2S_APLL_PROBLEM
    if(dkong_obuf_toggle)
#endif
      // advance write pointer. The buffer is a ring
      dkong_audio_rptr = (dkong_audio_rptr+1)&DKONG_AUDIO_QUEUE_MASK;
#ifdef WORKAROUND_I2S_APLL_PROBLEM
    dkong_obuf_toggle = !dkong_obuf_toggle;
#endif
  }
#endif
}

#ifdef USE_NAMCO_WAVETABLE
void audio_namco_waveregs_parse(void) {
#ifndef SINGLE_MACHINE
  if(
#ifdef ENABLE_PACMAN
    MACHINE_IS_PACMAN ||
#endif    
#ifdef ENABLE_GALAGA    
    MACHINE_IS_GALAGA ||
#endif
#ifdef ENABLE_DIGDUG    
    MACHINE_IS_DIGDUG ||
#endif
  0) 
#endif  
  {
    // parse all three wsg channels
    for(char ch=0;ch<3;ch++) {  
      // channel volume
      snd_volume[ch] = soundregs[ch * 5 + 0x15];
    
      if(snd_volume[ch]) {
        // frequency
        snd_freq[ch] = (ch == 0) ? soundregs[0x10] : 0;
        snd_freq[ch] += soundregs[ch * 5 + 0x11] << 4;
        snd_freq[ch] += soundregs[ch * 5 + 0x12] << 8;
        snd_freq[ch] += soundregs[ch * 5 + 0x13] << 12;
        snd_freq[ch] += soundregs[ch * 5 + 0x14] << 16;
      
        // wavetable entry
#ifdef ENABLE_PACMAN
  #if defined(ENABLE_GALAGA) || defined(ENABLE_DIGDUG)  // there's at least a second machine
        if(machine == MCH_PACMAN)
  #endif
          snd_wave[ch] = pacman_wavetable[soundregs[ch * 5 + 0x05] & 0x0f];
  #if defined(ENABLE_GALAGA) || defined(ENABLE_DIGDUG)
        else
  #endif      
#endif      
#ifdef ENABLE_GALAGA
  #ifdef ENABLE_DIGDUG
        if(machine == MCH_GALAGA)
  #endif
          snd_wave[ch] = galaga_wavetable[soundregs[ch * 5 + 0x05] & 0x07];
#endif      
#ifdef ENABLE_DIGDUG
          snd_wave[ch] = digdug_wavetable[soundregs[ch * 5 + 0x05] & 0x0f];
#endif      
      }
    }
  }
}
#endif  // MCH_PACMAN || MCH_GALAGA

#ifdef ENABLE_DKONG
void dkong_trigger_sound(char snd) {
  static const struct {
    const signed char *data;
    const unsigned short length; 
  } samples[] = {
    { (const signed char *)dkong_sample_walk0, sizeof(dkong_sample_walk0) },
    { (const signed char *)dkong_sample_walk1, sizeof(dkong_sample_walk1) },
    { (const signed char *)dkong_sample_walk2, sizeof(dkong_sample_walk2) },
    { (const signed char *)dkong_sample_jump,  sizeof(dkong_sample_jump)  },
    { (const signed char *)dkong_sample_stomp, sizeof(dkong_sample_stomp) }
  };

  // samples 0 = walk, 1 = jump, 2 = stomp

  if(!snd) {
    // walk0, walk1 and walk2 are variants
    char rnd = random() % 3;
    dkong_sample_cnt[0] = samples[rnd].length;
    dkong_sample_ptr[0] = samples[rnd].data;
  } else {
    dkong_sample_cnt[snd] = samples[snd+2].length;
    dkong_sample_ptr[snd] = samples[snd+2].data;
  }
}
#endif

void snd_transmit() {
  // (try to) transmit as much audio data as possible. Since we
  // write data in exact the size of the DMA buffers we can be sure
  // that either all or nothing is actually being written
  
  size_t bytesOut = 0;
  do {
    // copy data in i2s dma buffer if possible
    i2s_write(I2S_NUM_0, snd_buffer, sizeof(snd_buffer), &bytesOut, 0);

    // render the next audio chunk if data has actually been sent
    if(bytesOut) {
#if defined(ENABLE_PACMAN) || defined(ENABLE_GALAGA)
      audio_namco_waveregs_parse();
#endif
      snd_render_buffer();
    }
  } while(bytesOut);
}

void audio_dkong_bitrate(char is_dkong) {
  // The audio CPU of donkey kong runs at 6Mhz. A full bus
  // cycle needs 15 clocks which results in 400k cycles
  // per second. The sound CPU typically needs 34 instruction
  // cycles to write an updated audio value to the external
  // DAC connected to port 0.
  
  // The effective sample rate thus is 6M/15/34 = 11764.7 Hz
#ifndef WORKAROUND_I2S_APLL_PROBLEM
  i2s_set_sample_rates(I2S_NUM_0, is_dkong?11765:24000);
#endif
}

void audio_init(void) {
    // Setup the I2S configuration
    static const i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),  // Use master and TX mode
        .sample_rate = 24000,                                 // Sample rate 24kHz
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,         // 16-bit samples
#ifdef SND_DIFF
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,         // Dual channel
#elif defined(SND_LEFT_CHANNEL)
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,          // Left channel only
#else
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,         // Right channel only
#endif
        .intr_alloc_flags = 0,                                // Default interrupt allocation
        .dma_buf_count = 4,                                   // Number of DMA buffers
        .dma_buf_len = 64,                                    // Buffer length
        .use_apll = false                                     // Disable APLL
    };

    // Install I2S driver using the above configuration
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);

    // Set the I2S pins for the internal DAC
    i2s_set_pin(I2S_NUM_0, NULL);  // Use the internal DAC
}

/*
void audio_init(void) {  
  // init audio
#if defined(ENABLE_PACMAN) || defined(ENABLE_GALAGA)
  audio_namco_waveregs_parse();
#endif
  snd_render_buffer();

  // 24 kHz @ 16 bit = 48000 bytes/sec = 800 bytes per 60hz game frame =
  // 1600 bytes per 30hz screen update = ~177 bytes every four tile rows
  static const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
    .sample_rate = 24000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
#ifdef SND_DIFF
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
#elif defined(SND_LEFT_CHANNEL) // For devices using the left channel (e.g. CYD)
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
#else
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
#endif
    .intr_alloc_flags = 0,
    .dma_buf_count = 4,
    .dma_buf_len = 64,   // 64 samples
#ifndef WORKAROUND_I2S_APLL_PROBLEM
    .use_apll = true
#else
    // APLL usage is broken in ESP-IDF 4.4.5
    .use_apll = false
#endif
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);

#if defined(SINGLE_MACHINE) && defined(ENABLE_DKONG)
  // only dkong installed? Then setup rate immediately
  audio_dkong_bitrate(true);
#endif

// AV
i2s_set_pin(I2S_NUM_0, NULL); // Use internal DAC without external pins

// #ifdef SND_DIFF
//   i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
// #elif defined(SND_LEFT_CHANNEL) // For devices using the left channel (e.g. CYD)
//   i2s_set_dac_mode(I2S_DAC_CHANNEL_LEFT_EN); 
// #else
//   i2s_set_dac_mode(I2S_DAC_CHANNEL_RIGHT_EN);
// #endif
}
*/

void update_screen(void) {
  uint32_t t0 = micros();

#ifdef ENABLE_PACMAN
PACMAN_BEGIN
  pacman_prepare_frame();    
PACMAN_END
#endif

#ifdef ENABLE_GALAGA
GALAGA_BEGIN
  galaga_prepare_frame();
GALAGA_END
#endif  
  
#ifdef ENABLE_DKONG
DKONG_BEGIN
  dkong_prepare_frame();
DKONG_END
#endif

#ifdef ENABLE_FROGGER
FROGGER_BEGIN
  frogger_prepare_frame();
FROGGER_END
#endif

#ifdef ENABLE_DIGDUG
DIGDUG_BEGIN
  digdug_prepare_frame();
DIGDUG_END
#endif

#ifdef ENABLE_1942
_1942_BEGIN
  _1942_prepare_frame();
_1942_END
#endif

  // max possible video rate:
  // 8*224 pixels = 8*224*16 = 28672 bits
  // 2790 char rows per sec at 40Mhz = max 38 fps
#if TFT_SPICLK < 80000000
#define VIDEO_HALF_RATE
#endif

#ifdef VIDEO_HALF_RATE
  // render and transmit screen in two halfs as the display
  // running at 40Mhz can only update every second 60 hz game frame
  for(int half=0;half<2;half++) {
    // Serial.print("Rendering half: ");
    // Serial.println(half);

    // vga.clear(0);

    for(int c=18*half;c<18*(half+1);c+=3) {
      // AV changed write to pushImage

      // JT VGA STuff
      

      // Calculate the vertical offset based on the half being rendered
      // int vertical_offset = (half == 0) ? 0 : (0 / 2);  // Offset for the second half

      render_line(c + 0);
      VGAWrite(frame_buffer, 224 * 8, (c + 0) * 8);  // Adjust vertical position
      
      render_line(c + 1);
      VGAWrite(frame_buffer, 224 * 8, (c + 1) * 8);
      
      render_line(c + 2);
      VGAWrite(frame_buffer, 224 * 8, (c + 2) * 8);
      
      // render_line(c + 0); tft.pushImage(0, c * 8, 224, 8, frame_buffer);
      // render_line(c + 1); tft.pushImage(0, (c + 1) * 8, 224, 8, frame_buffer);
      // render_line(c + 2); tft.pushImage(0, (c + 2) * 8, 224, 8, frame_buffer);

// AV IMPORTANT: shifting screen (scaling isn't working)
      int shiftLeft = 20; // Define the shift amount
      int shiftDown = 20;

      render_line(c + 0); tft.pushImage(-shiftLeft, (c + 0) * 8 + shiftDown, 224, 8, frame_buffer);
      render_line(c + 1); tft.pushImage(-shiftLeft, (c + 1) * 8 + shiftDown, 224, 8, frame_buffer);
      render_line(c + 2); tft.pushImage(-shiftLeft, (c + 2) * 8 + shiftDown, 224, 8, frame_buffer);

      // audio is refilled 6 times per screen update. The screen is updated
      // every second frame. So audio is refilled 12 times per 30 Hz frame.
      // Audio registers are udated by CPU3 two times per 30hz frame.
        // AV commented audio
        //snd_transmit();
    } 


 
    // one screen at 60 Hz is 16.6ms
    unsigned long t1 = (micros()-t0)/1000;  // calculate time in milliseconds
    // printf("uspf %d\n", t1);
    if(t1<(half?33:16)) vTaskDelay((half?33:16)-t1);
    else if(half)       vTaskDelay(1);    // at least 1 ms delay to prevent watchdog timeout

    // physical refresh is 30Hz. So send vblank trigger twice a frame
    // to the emulation. This will make the game run with 60hz speed
    xTaskNotifyGive(emulationtask);
  }
#else
  #warning FULL SPEED
  
  // render and transmit screen at once as the display
  // running at 80Mhz can update at full 60 hz game frame
  for(int c=0;c<36;c+=6) {
    render_line(c + 0);
    VGAWrite(frame_buffer, 224 * 8, (c + 0) * 8);  // Adjust vertical position
    
    render_line(c + 1);
    VGAWrite(frame_buffer, 224 * 8, (c + 1) * 8);
    
    render_line(c + 2);
    VGAWrite(frame_buffer, 224 * 8, (c + 2) * 8);
    // AV
    // render_line(c+0); tft.write(frame_buffer, 224*8);
    // render_line(c+1); tft.write(frame_buffer, 224*8);
    // render_line(c+2); tft.write(frame_buffer, 224*8);
    // render_line(c+3); tft.write(frame_buffer, 224*8);
    // render_line(c+4); tft.write(frame_buffer, 224*8);
    // render_line(c+5); tft.write(frame_buffer, 224*8);
    render_line(c + 0); tft.pushImage(0, (c + 0) * 8, 224, 8, frame_buffer);
    render_line(c + 1); tft.pushImage(0, (c + 1) * 8, 224, 8, frame_buffer);
    render_line(c + 2); tft.pushImage(0, (c + 2) * 8, 224, 8, frame_buffer);
    render_line(c + 3); tft.pushImage(0, (c + 3) * 8, 224, 8, frame_buffer);
    render_line(c + 4); tft.pushImage(0, (c + 4) * 8, 224, 8, frame_buffer);
    render_line(c + 5); tft.pushImage(0, (c + 5) * 8, 224, 8, frame_buffer);

    // audio is updated 6 times per 60 Hz frame
    snd_transmit();
  } 
 
  // one screen at 60 Hz is 16.6ms
  unsigned long t1 = (micros()-t0)/1000;  // calculate time in milliseconds
  if(t1<16) vTaskDelay(16-t1);
  else      vTaskDelay(1);    // at least 1 ms delay to prevent watchdog timeout

  // physical refresh is 60Hz. So send vblank trigger once a frame
  xTaskNotifyGive(emulationtask);
#endif
   
#ifdef ENABLE_GALAGA
  /* the screen is only updated every second frame, scroll speed is thus doubled */
  static const signed char speeds[8] = { -1, -2, -3, 0, 3, 2, 1, 0 };
  stars_scroll_y += 2*speeds[starcontrol & 7];
#endif
}

void emulation_task(void *p) {
  prepare_emulation();

  while(1)
    emulate_frame();
}

void setup() {
  // tone(tonePin, 1000);
  Serial.begin(115200);

  // JT VGA STUFF
  pinMode(joystick_sw, INPUT_PULLUP);
  // digitalWrite(joystick_sw, HIGH); // Random line that must be included
  pinMode(button_1, INPUT_PULLUP);
  pinMode(button_2, INPUT_PULLUP);
  // Initialize button pin
  pinMode(SWITCH_BUTTON_PIN, INPUT_PULLUP);

  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  int x = 0;
  int y = 0;
  int sw = 0;
  int b1 = 0;
  int b2 = 0;
  
  // AV
  tft.init();  
  tft.fillScreen(TFT_BLACK);
  tft.begin();

  const PinConfig pins(-1,-1,1,-1,-1,  -1,-1,-1,2,-1,-1,  -1,-1,-1,3,-1,  13,11); // R G B h v

	Mode mode = Mode::MODE_640x400x70;
	
  Serial.println(ESP.getFreeHeap());
	if(!vga.init(pins, mode, 8, 3)) while(1) delay(1);
	Serial.println(ESP.getFreeHeap());

	vga.start();

	// for(int y = 0; y < 240; y++)
	// 	for(int x = 0; x < 320; x++)
	// 		vga.dotdit(x, y, x, y, 255-x);

	vga.setFont(FONT_9x16);
	vga.start();

  vga.clear();
	
	// delay(5000);

  // JT VGA STUFF
  // for(int y = 0; y < 240; y++)
	// 	for(int x = 0; x < 320; x++)
	// 		vga.dot(x, y, x, y, 0);

  Serial.print("ESP-IDF "); 
  Serial.println(ESP_IDF_VERSION, HEX); 

  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  // Initialize the current program
  if (currentProgram == 0) {
    setupGalagino();
  } else if (currentProgram == 1){
    setupFlappyBird2P();
  }
}

void setupGalagino() {

  tft.fillScreen(TFT_BLACK);
  // Stop any running tasks (e.g., emulation task)
  if (emulationtask != NULL) {
    vTaskDelete(emulationtask);  // Delete the previous emulation task if it's running
    emulationtask = NULL;        // Reset task handle
  }

  // Free memory allocated for frame_buffer and sprite if already allocated
  if (frame_buffer != NULL) {
    free(frame_buffer);
    frame_buffer = NULL;
  }
  if (sprite != NULL) {
    free(sprite);
    sprite = NULL;
  }
  machine = MCH_MENU;

  Serial.println("Galagino"); 

  #ifdef WORKAROUND_I2S_APLL_PROBLEM
  Serial.println("I2S APLL workaround active"); 
#endif

  // this should not be needed as the CPU runs by default on 240Mht nowadays
  setCpuFrequencyMhz(240000000);

  Serial.print("Free heap: "); Serial.println(ESP.getFreeHeap());
  Serial.print("Main core: "); Serial.println(xPortGetCoreID());
  Serial.print("Main priority: "); Serial.println(uxTaskPriorityGet(NULL));  

  // allocate memory for a single tile/character row
  frame_buffer = (unsigned short*)malloc(224*8*2);
  sprite = (struct sprite_S*)malloc(128 * sizeof(struct sprite_S));
  Serial.print("Free heap: "); Serial.println(ESP.getFreeHeap());

  // make button pins inputs
  pinMode(BTN_START_PIN, INPUT_PULLUP);
#ifdef BTN_COIN_PIN
  pinMode(BTN_COIN_PIN, INPUT_PULLUP);
#endif

#ifdef NUNCHUCK_INPUT
  nunchuckSetup();
#else
  pinMode(BTN_LEFT_PIN, INPUT_PULLUP);
  pinMode(BTN_RIGHT_PIN, INPUT_PULLUP);
  pinMode(BTN_DOWN_PIN, INPUT_PULLUP);
  pinMode(BTN_UP_PIN, INPUT_PULLUP);
  pinMode(BTN_FIRE_PIN, INPUT_PULLUP);
#endif

  // initialize audio to default bitrate (24khz unless dkong is
  // the only game installed, then audio will directly be 
  // initialized to dkongs 11765hz)

  // AV commented audio
  // audio_init();

// #ifdef LED_PIN
//   leds_init();
// #endif

  // let the cpu emulation run on the second core, so the main core
  // can completely focus on video
  xTaskCreatePinnedToCore(
      emulation_task, /* Function to implement the task */
      "emulation task", /* Name of the task */
      4096,  /* Stack size in words */
      NULL,  /* Task input parameter */
      2,  /* Priority of the task */
      &emulationtask,  /* Task handle. */
      0); /* Core where the task should run */
}

unsigned char buttons_get(void) {
  unsigned char input_states = 0;

  // Read joystick and button states
  // int joystick_x_value = analogRead(joystick_x);
  // int joystick_y_value = analogRead(joystick_y);
  // int joystick_sw_value = digitalRead(joystick_sw);
  // int button_1_value = digitalRead(button_1);
  // int button_2_value = digitalRead(button_2);

  int joystick_x_value = myData.joystick_x_value;
  int joystick_y_value = myData.joystick_y_value;
  int joystick_sw_value = myData.joystick_sw_value;
  int button_1_value = myData.button_1_value;
  int button_2_value = myData.button_2_value;

  int x = 0;
  int y = 0;

  if (joystick_x_value < 1500 || joystick_x_value > 2500 || joystick_y_value < 1500 || joystick_y_value > 2500) {
    float angle = atan2(joystick_y_value-2000,joystick_x_value-2000);
    float pi = 3.14159;
    float num = 2.6;
    if (angle > (num * -1) * pi/12 && angle <= num * pi/12) {
      x = 1;
    }
    else if (angle > num * pi/12 && angle <= (12-num) * pi/12) {
      y = 1;
    }
    else if (angle > (12 - num) * pi/12 || angle <= (num - 12) * pi/12) {
      x = -1;
    }
    else {
      y = -1;
    }
  }

  // Map joystick x/y to left/right/up/down buttons
  input_states |= (x == -1) ? BUTTON_LEFT : 0;
  input_states |= (x == 1) ? BUTTON_RIGHT : 0;
  input_states |= (y == -1) ? BUTTON_UP : 0;
  input_states |= (y == 1) ? BUTTON_DOWN : 0;


  // // Map joystick x/y to left/right/up/down buttons
  // input_states |= (joystick_x_value < 1500) ? BUTTON_LEFT : 0;
  // input_states |= (joystick_x_value > 2500) ? BUTTON_RIGHT : 0;
  // input_states |= (joystick_y_value < 1500) ? BUTTON_UP : 0;
  // input_states |= (joystick_y_value > 2500) ? BUTTON_DOWN : 0;

  // // Map joystick switch to fire button
  // input_states |= (!button_2_value) ? BUTTON_FIRE : 0;

  // Map button_1 to start and coin buttons
  input_states |= (!button_1_value) ? (BUTTON_START) : 0; 
  //   input_states |= (!button_1_value) ? (BUTTON_START | BUTTON_COIN) : 0;

  // Map button_2 to fire button
  input_states |= (!button_2_value) ? (BUTTON_FIRE | BUTTON_COIN) : 0;

  return input_states;
}



// AV removed
// void loop(void) {
//   // run video in main task. This will send signals
//   // to the emulation task in the background to 
//   // synchronize video
//   update_screen(); 

// // #ifdef LED_PIN
// //   leds_update();
// // #endif
// }



// -->>>>>> AV the following is to switch programs between galagino and our own

void runGalagino() {
  // run video in main task. This will send signals
  // to the emulation task in the background to
  // synchronize video
  update_screen();

  // #ifdef LED_PIN
  //   leds_update();
  // #endif
}

void renderFlappyBirdMenu(){

  // // Get the Flappy Bird logo
  // int num_logos = sizeof(logos) / sizeof(unsigned short*);
  // const unsigned short* flappybird_logo = NULL;
  // if (num_logos > 0) {
  //   flappybird_logo = logos[num_logos - 1]; // Assuming it's the last logo
  // } else {
  //   Serial.println("No logos available.");
  //   return;
  // }

  int logoHeight = 10;
  int shiftLeft = 20; // Define the shift amount
  int shiftDown = 20;
  // Render the Flappy Bird logo
  for (short row = 0; row < 96; row += 8) {
    render_logo(row, flappybird_logo, true);
    tft.pushImage(-shiftLeft, row + logoHeight, 224, 8, frame_buffer);
    VGAWrite(frame_buffer, 224 * 8, row);
  }

  // Maybe use code to align everything based on screen dimensions? 
  int initMenuHeight = 120;
  tft.setTextColor(TFT_WHITE);
  vga.setTextColor(73, 0);
  tft.setTextSize(2);
  tft.setCursor(10, initMenuHeight);
  vga.setCursor(40, initMenuHeight + 30);
  tft.println("Press to Jump");
  vga.println("Press to Jump");
  
  tft.setCursor(10, initMenuHeight + 30);
  vga.setCursor(40, initMenuHeight + 30 + 30);
  tft.println("P1: Left Btn");
  vga.println("P1: Left Btn");
  tft.setCursor(10, initMenuHeight + 60);
  vga.setCursor(40, initMenuHeight + 60 + 30);
  tft.println("P2: Right Btn");
  vga.println("P2: Right Btn");
  tft.setTextColor(TFT_GREEN);
  vga.setTextColor(1, 0);
  if (!vgaOn) vga.setTextColor(64, 0);
  tft.setTextSize(3);
  tft.setCursor(40, initMenuHeight + 105);
  vga.setCursor(60, initMenuHeight + 105 + 30);
  if (!vgaOn) vga.setCursor(60, initMenuHeight + 105 + 30);
  tft.println("JOYUP");
  if (vgaOn) vga.println("JOYUP");
  if (!vgaOn) vga.println("VGA OFF");
  tft.setCursor(40, initMenuHeight + 135);
  vga.setCursor(60, initMenuHeight + 135 + 30);
  if (!vgaOn) vga.setCursor(60, initMenuHeight + 135 + 30);;
  tft.println("START");
  if (vgaOn) vga.println("START");
  if (!vgaOn) vga.println("USE TTGO");
}

void setupFlappyBird2P() {
  // Stop any running tasks (if any)
  if (emulationtask != NULL) {
    vTaskDelete(emulationtask);  // Delete the previous task
    emulationtask = NULL;        // Reset task handle
  }
  inMenu = true;

  tft.fillScreen(TFT_BLACK);
  vga.fillRect(0, 0, 630, 390, 0);
  vga.clear();
  pinMode(FLAPPY_BIRD_SWITCH_BUTTON_PIN, INPUT_PULLUP);  // Initialize button pin
  pinMode(BTN_START_PIN, INPUT_PULLUP);  // Start button
  pinMode(BTN_JUMP_P1_PIN, INPUT_PULLUP);  // Player 1 jump button
  pinMode(BTN_JUMP_P2_PIN, INPUT_PULLUP);  // Player 2 jump button
  initFlappyBird2P(); 
}

void runFlappyBird2P() {
  static unsigned long lastDebounceTime = 0;
  static int buttonState = HIGH;  // current debounced button state
  static int lastButtonReading = HIGH;  // previous reading from the input pin

  int reading = myData.joystick_y_value; // FLAPPY_BIRD_SWITCH_BUTTON_PIN (joystick up to start)

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
  }

  // Debouncing logic
  if ((millis() - lastDebounceTime) > 50) {
    // If the button state has changed
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) { //&& !flappyBirdStarted
        // Button is pressed, toggle between menu and game
        flappyBirdStarted = false;
        inMenu = !inMenu;
        tft.fillScreen(TFT_BLACK);  // Clear screen
        vga.fillRect(0, 0, 630, 390, 0);
        vga.clear();
      }
    }
  }

  lastButtonReading = reading;

  // Render either the menu or the game depending on the current state
  if (inMenu) {
    flappyBirdStarted = false;
    renderFlappyBirdMenu();  // Show the menu
  } else {
    flappyBirdStarted = true;
    FlappyBird2P();  // Run the game
  }
}

// Button debounce variables for each player
unsigned long lastDebounceTimeP1 = 0;
unsigned long lastDebounceTimeP2 = 0;
unsigned long lastDebounceTimeStart = 0;
unsigned long debounceDelay = 50;

int lastButtonStateP1 = HIGH;
int lastButtonStateP2 = HIGH;
int lastButtonStateStart = HIGH;


void FlappyBird2P(){
  handleInput();
  // Run the main Flappy Bird 2P game logic
  mainFlappyBird2P();
}


// Handle input for both players and the start button
void handleInput() {
  // Player 1 jump (left button)
  if (isButtonPressed(myData.button_1_value, lastDebounceTimeP1, lastButtonStateP1)) {
    birdJump(bird1);  // Player 1's bird jumps
    // Play tone
    if (!gameOver) tone(tonePin, 1047, 40);
  }

  // Player 2 jump (right button)
  if (isButtonPressed(second_controller_button, lastDebounceTimeP2, lastButtonStateP2)) {
    birdJump(bird2);  // Player 2's bird jumps
    // Play tone
    if (!gameOver) tone(tonePin, 1175, 40);
  }

  // Start button press to start the game or restart after game over
  if (isButtonPressed(myData.joystick_y_value, lastDebounceTimeStart, lastButtonStateStart)) {
    if (!flappyBirdStarted) {
      flappyBirdStarted = true;
      startGame();  // Start the game if it hasn't started yet
    } else if (gameOver) {
      initFlappyBird2P();  // Restart the game if game over
    }
  }
}

// Function to check if a button was pressed, with debounce logic
bool isButtonPressed(int value, unsigned long &lastDebounceTime, int &lastButtonState) {
  int reading = value;

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW) {  // Button is pressed (active-low)
      lastButtonState = reading;
      return true;
    }
  }

  lastButtonState = reading;
  return false;
}


// // Setup the menu system
// void setupMenuSystem() {
//   pinMode(SOUND_TOGGLE_BUTTON_PIN, INPUT_PULLUP);  // Configure button pin

//   tft.fillScreen(TFT_BLACK);
//   vga.fillRect(0, 0, 630, 390, 0);
//   tft.setTextColor(TFT_WHITE);
//   vga.setTextColor(73, 0);
//   tft.setTextSize(2);
  
//   // Display menu title
//   tft.setCursor(20, 10);
//   vga.setCursor(20, 10 + 30);
//   tft.println("Settings");
//   vga.println("Settings");

//   // Display updated sound state
//   tft.setCursor(10, 40);
//   vga.setCursor(10, 40+30);
//   if (soundOn) {
//     tft.setTextColor(TFT_GREEN);
//     vga.setTextColor(1, 0);
//     tft.println("Sound: ON");
//     vga.println("Sound: ON");
//   } else {
//     tft.setTextColor(TFT_RED);
//     vga.setTextColor(64, 0);
//     tft.println("Sound: OFF");
//     vga.println("Sound: OFF");
//   }

//   tft.setTextColor(TFT_WHITE);
//   vga.setTextColor(73, 0);
//   tft.setCursor(10, 80);
//   vga.setCursor(10, 80+30);
//   tft.println("Press X To");
//   vga.println("Press X To");
//   tft.setCursor(10, 110);
//   vga.setCursor(10, 110+30);
//   tft.println("Toggle Sound");
//   vga.println("Toggle Sound");
// }

// void runMenuSystem() {
//   // Check if the button is pressed (active-low button)
//   if (myData.button_1_value == LOW) {
//     delay(200);  // Simple debounce delay
//     soundOn = !soundOn;  // Toggle the sound state
    
//     // Clear the screen and update the menu with the new sound state
//     tft.fillScreen(TFT_BLACK);
//     vga.fillRect(0, 0, 630, 390, 0);
//     tft.setTextColor(TFT_WHITE);
//     vga.setTextColor(73, 0);
//     tft.setTextSize(2);
    
//     // Display menu title
//     tft.setCursor(20, 10);
//     vga.setCursor(20, 10 + 30);
//     tft.println("Settings");
//     vga.println("Settings");

//     // Display updated sound state
//     tft.setCursor(10, 40);
//     vga.setCursor(10, 40+30);
//     if (soundOn) {
//       tft.setTextColor(TFT_GREEN);
//       vga.setTextColor(1, 0);
//       tft.println("Sound: ON");
//       vga.println("Sound: ON");
//     } else {
//       tft.setTextColor(TFT_RED);
//       vga.setTextColor(64, 0);
//       tft.println("Sound: OFF");
//       vga.println("Sound: OFF");
//     }

//     tft.setTextColor(TFT_WHITE);
//     vga.setTextColor(73, 0);
//     tft.setCursor(10, 80);
//     vga.setCursor(10, 80+30);
//     tft.println("Press X To");
//     vga.println("Press X To");
//     tft.setCursor(10, 110);
//     vga.setCursor(10, 110+30);
//     tft.println("Toggle Sound");
//     vga.println("Toggle Sound");
    
//     // Add delay to avoid bouncing issues
//     delay(500);
//   }
// }

bool vgaOn = true;  // Toggle VGA display for flappy Bird

// Setup the menu system
void setupMenuSystem() {

  // Clear both screens (TFT and VGA)
  tft.fillScreen(TFT_BLACK);
  vga.fillRect(0, 0, 630, 390, 0);
  vga.clear();
  
  // Set text color for both displays
  tft.setTextColor(TFT_WHITE);
  vga.setTextColor(73, 0);
  tft.setTextSize(2);

  // Display menu title
  tft.setCursor(20, 10);
  vga.setCursor(20, 10 + 30);
  tft.println("Settings");
  vga.println("Settings");

  // Display updated sound state
  tft.setCursor(10, 40);
  vga.setCursor(10, 40 + 30);
  if (soundOn) {
    tft.setTextColor(TFT_GREEN);
    vga.setTextColor(1, 0);
    tft.println("Sound: ON");
    vga.println("Sound: ON");
    tonePin = 18;
    buzzer = 18;
  } else {
    tft.setTextColor(TFT_RED);
    vga.setTextColor(64, 0);
    tft.println("Sound: OFF");
    vga.println("Sound: OFF");
    tonePin = 16;
    buzzer = 16;
  }

  // Display VGA state
  tft.setCursor(10, 80);
  vga.setCursor(10, 80 + 30);
  if (vgaOn) {
    tft.setTextColor(TFT_GREEN);
    vga.setTextColor(1, 0);
    tft.println("FB VGA: ON");
    vga.println("FB VGA: ON");
  } else {
    tft.setTextColor(TFT_RED);
    vga.setTextColor(64, 0);
    tft.println("FB VGA: OFF");
    vga.println("FB VGA: OFF");
  }

  // Instructions for toggling
  tft.setTextColor(TFT_WHITE);
  vga.setTextColor(73, 0);
  tft.setCursor(10, 120);
  vga.setCursor(10, 120 + 30);
  tft.println("B1: Sound");
  vga.println("B1: Sound");
  
  tft.setCursor(10, 180);
  vga.setCursor(10, 180 + 30);
  tft.println("B2: VGA");
  vga.println("B2: VGA");
}

void runMenuSystem() {
  // Check if the sound toggle button is pressed (active-low button)
  if (myData.button_1_value == LOW) {
    delay(200);  // Simple debounce delay
    soundOn = !soundOn;  // Toggle the sound state
    
    // Redraw the menu
    setupMenuSystem();

    // Add delay to avoid bouncing issues
    delay(500);
  }

  // Check if the VGA toggle button is pressed (active-low button)
  if (myData.button_2_value == LOW) {
    delay(200);  // Simple debounce delay
    vgaOn = !vgaOn;  // Toggle the VGA state

    // Redraw the menu
    setupMenuSystem();

    // Add delay to avoid bouncing issues
    delay(500);
  }
}


void loop() {
  static unsigned long lastDebounceTime = 0;
  static int buttonState = HIGH;        // the current debounced button state
  static int lastButtonReading = HIGH;  // the previous reading from the input pin

  // Menu button?
  int reading = myData.joystick_sw_value;

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
    lastButtonReading = reading;
  }

  if ((millis() - lastDebounceTime) > 50) {
    // If the button state has changed
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        // Button is pressed
        currentProgram = (currentProgram + 1) % NUM_PROGRAMS; // Cycle through 0, 1, 2
        Serial.print("Switched to ");
        // Reinitialize the program
        if (currentProgram == 0) {
          Serial.println("Galagino");
          setupGalagino();
        } else if (currentProgram == 1) {
          Serial.println("Flappy Bird 2P");
          setupFlappyBird2P();
        } else if (currentProgram == 2) {
          Serial.println("Menu System");
          setupMenuSystem();
        }
      }
    }
  }

  // Run the selected program based on the current state
  if (currentProgram == 0) {
    runGalagino();
    processAudio();
  } else if (currentProgram == 1) {
    runFlappyBird2P();
  } else if (currentProgram == 2) {
    runMenuSystem();
  }


}


// AV original galagino only code
// void loop(void) {
//   // run video in main task. This will send signals
//   // to the emulation task in the background to
//   // synchronize video
//   update_screen();

// // #ifdef LED_PIN
// //   leds_update();
// // #endif
// }
