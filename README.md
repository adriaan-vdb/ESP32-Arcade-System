# ESP32 Arcade System

## Overview

Galagino ESP32 Arcade System is an open-source, modular arcade platform built around the ESP32 microcontroller. Designed for both educational and hobbyist use, this project enables users to emulate and play classic arcade games, create custom games, and experiment with hardware interfaces such as controllers, VGA video output, and sound generation.

The project is organized into several modules:
- **Galagino**: The main arcade emulator, supporting games like Galaga, Pac-Man, Donkey Kong, Frogger, Dig Dug, 1942, and a custom Flappy Bird game.
- **Main Controller**: Firmware for a custom arcade controller, supporting joystick and button input, with wireless communication to the main system.
- **VGA Config**: Example and configuration code for VGA video output from the ESP32.
- **Tone Config**: Example code for generating tones and sound effects on the ESP32.

## Features

- Play classic arcade games on ESP32 hardware
- Modular codebase for easy extension and customization
- Custom controller support with joystick and multiple buttons
- VGA video output for authentic arcade display
- Sound and music generation using ESP32's capabilities
- Menu system for switching between games and settings
- Open-source and educational focus

## Directory Structure

```
elec3020-project/
  galagino/         # Main arcade emulator and games
  Main-Controller/  # Custom controller firmware
  VGA-Config/       # VGA output configuration and examples
  Tone-Config/      # Sound/tone generation examples
```

## Getting Started

### Prerequisites

- ESP32 development board (ESP32-S3 recommended)
- Arduino IDE or PlatformIO
- Required libraries: `TFT_eSPI`, `esp_now`, `WiFi`, etc.
- Basic electronics for controller and display connections

### Setup

1. **Clone the repository:**
   ```sh
   git clone https://github.com/yourusername/Galagino-ESP32-Arcade-System.git
   ```
2. **Open the desired module in Arduino IDE or PlatformIO.**
3. **Install required libraries** (see code comments for details).
4. **Connect your hardware** (controller, display, speaker, etc.).
5. **Upload the firmware** to your ESP32 board.

### Running the Emulator

- Upload `galagino.ino` to your ESP32 to run the arcade emulator.
- Use the custom controller firmware in `Main-Controller/` for input.
- VGA and sound configuration examples are available in their respective folders.

## Hardware

- ESP32 development board
- Custom arcade controller (joystick + buttons)
- VGA-compatible display
- Speaker or piezo buzzer for sound output

## Contributing

Contributions are welcome! Please open issues or pull requests for bug fixes, new features, or documentation improvements.

## License

This project is licensed under the GPLv3 License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

- Based on the original Galagino project by Till Harbaum ([GitHub](https://github.com/harbaum/galagino))
- Includes code and inspiration from the Arduino and ESP32 open-source communities 