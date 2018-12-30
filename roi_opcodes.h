//--------------------------
// Macros for ROI opcodes
// Based on Roomba 500 OI specifications
// https://www.irobot.lv/uploaded_files/File/iRobot_Roomba_500_Open_Interface_Spec.pdf
//--------------------------
// Author: Layne Bernardo
// Email: retnuh66@gmail.com
//
// Created July 25th, 2018
// Version 0.1
//--------------------------

#ifndef _ROI_OPCODES_H_INCLUDED_
#define _ROI_OPCODES_H_INCLUDED_

// ROI Control
//---------------------
// Start ROI
#define START 128
// Change baud rate
#define BAUD 129
// Go to safe mode from passive (this is NOT interchangeable with SAFE command on Roomba 405, control command MUST be issued to go to safe mode from passive)
#define CONTROL 130
// Go to safe mode from full
#define SAFE 131
// Go to full mode from safe
#define FULL 132
// Turn Roomba off
#define POWER 133

// Behavior
//---------------------
#define SEEK_DOCK 143
// Set cleaning schedule
#define SCHEDULE 167

// Cleaning
//---------------------
// Spot cleaning
#define SPOT 134
// Normal clean cycle
#define CLEAN 135
// Max clean
#define MAX 136

// Motors
//---------------------
// Drive roomba (speed, turn radius)
#define DRIVE 137
// Drive individual wheels (right side speed, left side speed)
#define DRIVE_WHEELS 145
// Drive individual wheels (PWM speed control)
#define DRIVE_PWM 146
// Non-drive motors on/off (vacuum, main brush, side brush)
#define MOTORS 138
// Non-drive motors PWM speed control
#define MOTORS_PWM 144

// Lights and Sounds
//---------------------
// Control red/green LEDs
#define LEDS 139
// Add song
#define SONG 140
// Play song
#define PLAY 141

// Sensor Data
//---------------------
// Continuously return specified sensor data packets every 15ms
#define STREAM 148
// Pause / resume stream without resetting list of sensors
#define STREAM_CONTINUE 150
// Query a single sensor
#define QUERY 142
// Return list of sensor packets from a list of multiple sensors
#define QUERY_LIST 149

// Misc
//---------------------
// Set day/time
#define SET_TIME 168
// Emulate pressing physical buttons
#define BUTTONS 165

// Roomba 560 and 570 Specific
//---------------------
// Control scheduling LEDs on Roomba 560 and 570
#define LEDS_SCHEDULE 162
// Full control over 7-seg displays on Roomba 560 and 570
#define LEDS_SCHEDULE_RAW 163
// Display ASCII characters on 7-seg displays on Roomba 560 and 570
#define LEDS_SCHEDULE_ASCII 164

#endif //roi_opcodes.h
