#include <Arduino.h>

#include <Wire.h>

#include "BlinkLightChain.hpp"
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 5, 6, 7, 8);

const int ROT[] = {0, 100, 0xFF, 0};
const int GRUEN[] = {100, 10, 0x35, 0};
const int BLAU[] = {0, 100, 0x00, 0};

bool prevVal = false;

int curState = 0;
const int maxStates = 4;

const uint16_t states[] = {500, 5000, 0, UINT16_MAX};

BlinkLightChain blc(4, states[0]);

void setup()
{
    pinMode(12, INPUT); // set button INPUT
    pinMode(4, OUTPUT);
    // set up the LCD's number of columns and rows:

    lcd.begin(16, 2);

    analogWrite(9, 0xFF - BLAU[0]);
    analogWrite(10, 0xFF - GRUEN[0]);
    analogWrite(11, 0xFF - ROT[0]);

    // Print a message to the LCD.
    lcd.print("Merry Christmas!");
    lcd.setCursor(0, 1);
    lcd.print("1080970");

    blc.init();

    delay(1000);
}

void loop()
{
    delay(100);
    const bool button = digitalRead(12) == LOW;
    if (button != prevVal)
    {
        prevVal = button;
        if (button)
        {
            curState = (curState + 1) % maxStates;

            analogWrite(9, 0xFF - BLAU[curState]);
            analogWrite(10, 0xFF - GRUEN[curState]);
            analogWrite(11, 0xFF - ROT[curState]);

            blc.setDelay(states[curState]);
        }
    }

    blc.blink();
}
