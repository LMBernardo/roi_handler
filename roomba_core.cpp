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
// TODO:
//      1. Possibly merge command functions into generic function

#include "roomba_core.h"

// ROI opcode macros
#include "roi_opcodes.h"

//--------------------------
// Utilities
//--------------------------

// Counts words in a string
// Taken from: https://codereview.stackexchange.com/questions/30547/number-of-words-in-a-string
unsigned int roomba_core::count_words(std::string const& str)
{
        std::stringstream stream(str);
        return std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
}

// Split 16 bit word into two bytes
// Results stored in reference variable byte_array
int roomba_core::split_word(int word, unsigned char* byte_array){
        // Cast int as pointer to char, then dereference each byte
        char* bytes;
        bytes = (char*) &word;

        // Assign them but swap them (high bits first)1
        byte_array[0] = (unsigned char) bytes[1];
        byte_array[1] = (unsigned char) bytes[0];

        // Debug
        std::cout << "Int: " << word << std::endl;
        std::cout << "Binary: ";
        unsigned int _size = sizeof(char) * 16;
        for(unsigned _s = 0; _s < _size; ++_s)
                std::cout << ((word >> (_size - 1 - _s)) & 1);
        std::cout << std::endl;

        std::cout << "Hex bytes: ";
        printf("%X", bytes[1]);
        printf("%X\n", bytes[0]);

        std::cout << "Binary bytes: ";
        _size = sizeof(char) * 8;
        for(unsigned _s = 0; _s < _size; ++_s)
                std::cout << (((int)bytes[1] >> (_size - 1 - _s)) & 1);
        std::cout << " ";
        for(unsigned _s = 0; _s < _size; ++_s)
                std::cout << (((int)bytes[0] >> (_size - 1 - _s)) & 1);
        std::cout << std::endl;
        //

        return 0;
}

int roomba_core::connect(){
        if ( s_serial.rs_open() < 0) {
                int errsv = errno;
                std::cout << "Failed to open serial console" << std::endl;
                return errsv;
        } else {
                std::cout << "Successully opened serial console" << std::endl;
                return 0;
        }
}

int roomba_core::power_off(){
        std::cout << "Powering off.." << std::endl;
        if ( s_serial.send_byte(POWER) < 0 ) {
                int errsv = errno;
                std::cout << "Failed to send command." << std::endl;
                std::cout << "Errno: " << errsv << std::endl;
                return -1;
        }
        return 0;
}

int roomba_core::init_roi(){
        std::cout << "Starting ROI..." << std::endl;
        if ( s_serial.send_byte(START) < 0 ) {
                int errsv = errno;
                std::cout << "Failed to send command." << std::endl;
                std::cout << "Errno: " << errsv << std::endl;
                return -1;
        }
        std::cout << "ROI should be in PASSIVE mode." << std::endl;
        return 0;
}

int roomba_core::safe_mode(){
        std::cout << "Sending 'control' command" << std::endl;
        if ( s_serial.send_byte(CONTROL) < 0 ) {
                int errsv = errno;
                std::cout << "Failed to send command." << std::endl;
                std::cout << "Errno: " << errsv << std::endl;
                return -1;
        }
        std::cout << "ROI should be in SAFE mode." << std::endl;
        return 0;
}

// FIXME: Make "Song" class to track multiple full songs as well as song-switch timing to play
//        full song at once.
int roomba_core::add_song(std::string song){
        std::cout << "Adding song..." << std:: endl;
        unsigned int num_bytes = count_words(song);

        if (num_bytes % 2 != 0) {
                std::cout << "Invalid song." << std::endl;
                return -1;
        }

        // Two bytes per note, 16 notes per song
        unsigned int num_songs = num_bytes / 32;
        unsigned int remaining_note_bytes = num_bytes % 32;
        if ( remaining_note_bytes != 0 ) num_songs++;
        if (num_songs == 0) num_songs++;

        std::cout << "Using " << num_songs << " songs from 0 to " << num_songs-1 << std::endl;

        // Split bytes into vector
        std::istringstream iss(song);
        std::vector<std::string> song_bytes;
        std::string song_buffer;

        while (iss >> song_buffer) {
                song_bytes.insert(song_bytes.begin(), song_buffer);
        }

        int num_note_bytes = 32;
        for (unsigned int current_song=0; current_song < num_songs; current_song++) {
                remaining_note_bytes = num_bytes % 32;
                std::string song_data = "";
                song_data = std::to_string(SONG) + " " + std::to_string(current_song);
                if (num_bytes < 32) {
                        song_data = song_data + " " + std::to_string(remaining_note_bytes / 2);
                        num_note_bytes = remaining_note_bytes;
                } else {
                        song_data = song_data + " " + "16";
                }

                for (int i=0; i < num_note_bytes / 2; i++) {
                        song_data = song_data + " " + song_bytes.back();
                        song_bytes.pop_back();
                        song_data = song_data + " " + song_bytes.back();
                        song_bytes.pop_back();
                        num_bytes = num_bytes-2;
                }
                std::cout << "Data for song " << current_song << ": " << song_data << std::endl;
                if ( s_serial.send_byte_array(song_data) < 0 ) {
                        return -1;
                }
        }
        return 0;
}

int roomba_core::play_song(std::string song_num){
        std::string command_string = std::to_string(PLAY) + " " + song_num;
        if ( s_serial.send_byte_array(command_string) < 0 ) {
                return -1;
        } else {
                return 0;
        }
}

// Control motors (vacuum, brush, side brush)
// Format is: Main Brush Direction, Side Brush Direction, Main Brush, Vacuum, Side Brush
// The first two bits are 0 (default direction) or 1 (opposite). The last three turn each motor on or off.
int roomba_core::motors(std::string motor_data){
        std::string command_string = std::to_string(MOTORS) + " " + motor_data;
        std::cout << "Motor data: " << command_string << std::endl;
        if ( s_serial.send_byte_array(command_string) < 0 ) {
                return -1;
        } else {
                return 0;
        }
}

int roomba_core::drive(std::string drive_data){
        //std::string command_string = DRIVE + " ";
        //command_string += drive_data;

        int int_list[2];

        // Use istringstream to parse data string
        std::string string_int = "";
        std::istringstream iss(drive_data);

        // Convert string bytes to raw bytes (unsigned char)
        unsigned int i = 0;
        while (iss >> string_int) {
                int_list[i] = std::stoi(string_int);
                i++;
        }

        unsigned char command_bytes[5];

        // Split each int into two bytes
        unsigned char velocity[2];
        split_word( int_list[0], velocity );
        unsigned char radius[2];
        split_word( int_list[1], radius );

        // Probably a tedious / dumb way to do this
        command_bytes[0] = (unsigned char) DRIVE;
        command_bytes[1] = velocity[0];
        command_bytes[2] = velocity[1];
        command_bytes[3] = radius[0];
        command_bytes[4] = radius[1];

        for (int i = 0; i < 5; i++) {
                std::cout << "Command " << i << ": " << (int) command_bytes[i] << std::endl;
        }

        if ( s_serial.send_byte_array(command_bytes, 5) < 0 ) {
                return -1;
        } else {
                return 0;
        }
        return 0;
}

// Drive wheels separately
// Takes 4 data bytes: Right Velocity High, Right Velocity Low, Left Velocity High, Left Velocity Low
// -500 to 500 mm/s
int roomba_core::drive_wheels(std::string drive_data){

        int int_list[2];

        // Use istringstream to parse data string
        std::string string_int = "";
        std::istringstream iss(drive_data);

        // Convert string bytes to raw bytes (unsigned char)
        unsigned int i = 0;
        while (iss >> string_int) {
                int_list[i] = std::stoi(string_int);
                i++;
        }

        unsigned char command_bytes[5];

        // Split each int into two bytes
        unsigned char right[2];
        split_word( int_list[0], right );
        unsigned char left[2];
        split_word( int_list[1], left );

        // Probably a tedious / dumb way to do this
        command_bytes[0] = (unsigned char) DRIVE_WHEELS;
        command_bytes[1] = right[0];
        command_bytes[2] = right[1];
        command_bytes[3] = left[0];
        command_bytes[4] = left[1];

        for (int i = 0; i < 5; i++) {
                std::cout << "Command " << i << ": " << (int) command_bytes[i] << std::endl;
        }

        if ( s_serial.send_byte_array(command_bytes, 5) < 0 ) {
                return -1;
        } else {
                return 0;
        }
        return 0;
}

// Control LEDs
// TODO: Add led_data format to comments
int roomba_core::leds(std::string led_data){
        std::string command_string = std::to_string(LEDS) + " " + led_data;
        std::cout << "LED data: " << command_string << std::endl;
        if ( s_serial.send_byte_array(command_string) < 0 ) {
                return -1;
        } else {
                return 0;
        }
}

// Control power LED individually (for convenience)
int roomba_core::power_led(std::string led_data){
        // Fill in other LED bits with zeros
        led_data = "0 " + led_data;
        return ( leds(led_data) );
}
