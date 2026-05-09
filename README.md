# AVR128DA32 ADC Voice Record

This project is an AVR128DA32 audio experiment created in MPLAB X.

It is focused on learning how to handle voice or audio data with the AVR128DA32 using the ADC for input and the DAC for output. The project can be used as a base for simple voice playback, recorded sound experiments, or future voice recording projects.

## Features

- AVR128DA32 microcontroller project
- MPLAB X compatible project files
- DAC-based audio output test
- stored audio sample in a header file
- simple and beginner-friendly project structure

## Files

- `main.c` - main application code
- `welcome_to_the_show_8k_u8.h` - stored audio sample data
- `Makefile` - build file
- `nbproject/` - MPLAB X project configuration files

## Purpose

This project was created for testing and learning:

- how audio sample data can be stored in program memory
- how audio can be sent to an output stage
- how AVR128DA32 can be used in simple embedded audio applications

## Tools Used

- AVR128DA32
- MPLAB X IDE
- XC8 compiler

## Future Work

- add microphone input
- add full ADC recording support
- improve playback quality
- document hardware connections
- add amplifier and speaker connection notes
