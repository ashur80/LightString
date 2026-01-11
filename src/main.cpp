#include <Arduino.h>

#include <Wire.h>

#include "BlinkLightChain.hpp"
#include <LiquidCrystal.h>
// main.cpp
// Simple controller for a light-chain and LCD backlight.
// - Uses `BlinkLightChain` to control a separate light-chain output pin.
// - Adjusts LCD backlight color per mode using PWM (inverted values for active-low hardware).
// - Button cycles modes between fast, slow, always-on, and off.
//
// Notes:
// - Timing uses `millis()` and unsigned arithmetic to tolerate overflow.
// - Special delay sentinels are defined in `include/BlinkLightChain.hpp`.
// LCD Configuration
constexpr int LCD_RS = 2;
constexpr int LCD_EN = 3;
constexpr int LCD_D4 = 5;
constexpr int LCD_D5 = 6;
constexpr int LCD_D6 = 7;
constexpr int LCD_D7 = 8;
constexpr int LCD_COLS = 16;
constexpr int LCD_ROWS = 2;

// LCD backlight pin configuration
constexpr int LCD_BACKLIGHT_BLUE_PIN = 9;
constexpr int LCD_BACKLIGHT_GREEN_PIN = 10;
constexpr int LCD_BACKLIGHT_RED_PIN = 11;

// Button / light-chain control pin
constexpr int BUTTON_PIN = 12;
constexpr int LIGHT_CHAIN_PIN = 4;

// Timing constants
constexpr unsigned long STARTUP_DELAY_MS = 1000UL;
constexpr unsigned long LOOP_POLL_MS = 100UL;
constexpr unsigned long BLINK_FAST_MS = 500UL;
constexpr unsigned long BLINK_SLOW_MS = 5000UL;

// Mode indices
enum ModeIndex { MODE_FAST = 0, MODE_SLOW = 1, MODE_ON = 2, MODE_OFF = 3 };
constexpr int NUM_MODES = 4;

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Color struct for LCD backlight levels (0-255). Using a single struct
// makes the code clearer than parallel arrays and groups RGB values
// per mode in one place.
struct Color {
    uint8_t r; // red channel (0..255)
    uint8_t g; // green channel
    uint8_t b; // blue channel
};

// Colors per mode: {R, G, B}
// Keep these constexpr so they live in flash/ROM on AVR boards.
constexpr Color COLORS[NUM_MODES] = {
    { 0,   100,  0   }, // MODE_FAST: mostly green
    { 100, 10,   100 }, // MODE_SLOW: pink
    { 0xFF, 0x35, 0x00 }, // MODE_ON: ornage
    { 0,   0,    0   }  // MODE_OFF: backlight off
};

bool prevVal = false;

int curState = 0;
const int maxStates = NUM_MODES;

const unsigned long states[NUM_MODES] = {BLINK_FAST_MS, BLINK_SLOW_MS, BLINK_ALWAYS_ON, BLINK_ALWAYS_OFF};

BlinkLightChain blc(LIGHT_CHAIN_PIN, states[MODE_FAST]);

void setup()
{
    // Configure pins
    pinMode(BUTTON_PIN, INPUT);
    pinMode(LIGHT_CHAIN_PIN, OUTPUT);

    // Initialize LCD with number of columns/rows
    lcd.begin(LCD_COLS, LCD_ROWS);

    // Set initial LCD backlight using inverted PWM values. The hardware
    // uses active-low backlight control, so we invert the 0..255 values
    // before writing to the PWM pins.
    constexpr int MAX_PWM = 0xFF;
    analogWrite(LCD_BACKLIGHT_BLUE_PIN, MAX_PWM - COLORS[0].b);
    analogWrite(LCD_BACKLIGHT_GREEN_PIN, MAX_PWM - COLORS[0].g);
    analogWrite(LCD_BACKLIGHT_RED_PIN, MAX_PWM - COLORS[0].r);

    // Print a message to the LCD.
    lcd.print("Merry Christmas!");
    lcd.setCursor(0, 1);
    lcd.print("1080970");

    blc.init();

    delay(STARTUP_DELAY_MS);
}

void loop()
{
    // Simple polling loop. `delay` here reduces CPU usage; 
    delay(LOOP_POLL_MS);

    // Read button (assumes LOW when pressed)
    const bool button = digitalRead(BUTTON_PIN) == LOW;
    if (button != prevVal)
    {
        prevVal = button;
        if (button)
        {
            curState = (curState + 1) % maxStates;
            // Update backlight color for the newly selected mode
            constexpr int MAX_PWM = 0xFF;
            analogWrite(LCD_BACKLIGHT_BLUE_PIN, MAX_PWM - COLORS[curState].b);
            analogWrite(LCD_BACKLIGHT_GREEN_PIN, MAX_PWM - COLORS[curState].g);
            analogWrite(LCD_BACKLIGHT_RED_PIN, MAX_PWM - COLORS[curState].r);

            // Notify the blink controller of the new mode's timing.
            // `setDelay` uses sentinel values (defined in the header)
            // to indicate always-on / always-off.
            blc.setDelay(states[curState]);
        }
    }

    blc.blink();
}
