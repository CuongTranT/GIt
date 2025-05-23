#pragma once
#include <Arduino.h>

// ===== DISPLAY BUFFER =====
extern uint8_t Buffer_display[16][4];

// ===== GPIO SETUP =====
void setupPins();
void pulse(int pin);

// ===== DISPLAY FUNCTIONS =====
void setRow(uint8_t row);
void displayRowPair(uint8_t row);
void clearDisplay();
void setPixel(int x, int y, bool color);
void testPixel(int x, int y);
