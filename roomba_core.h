//--------------------------
// Roomba Core Utilities Header
//--------------------------
// Description:
// Interface to ROI
// Designed specifically for Roomba 405, may not provide full functionality for more advanced models.
//--------------------------
// Author: Layne Bernardo
// Email: retnuh66@gmail.com
//
// Created July 24th, 2018
// Modified: October 8th, 2018
// Version 0.2
//--------------------------

#ifndef _ROOMBA_CORE_H_INCLUDED_
#define _ROOMBA_CORE_H_INCLUDED_

#include <cstdlib>
#include <iostream>

#include <iterator>
#include <vector>
#include <string>

// Serial handler library
#include "serial_handler.h"

class roomba_core
{
  public:

    serial_handler s_serial;

    // Count words in a string
    unsigned int count_words(std::string const& str);
    // Split 16 bits into two bytes
    int split_word(int word, unsigned char* byte_array);

    int connect();

    int power_off();

    int init_roi();

    int safe_mode();

    int add_song(std::string song);

    int play_song(std::string song_num);

    int motors(std::string motor_data);

    int drive(std::string drive_data);

    int drive_wheels(std::string drive_data);

    int leds(std::string led_data);

    int power_led(std::string led_data);

};

#endif // roomba_core.h
