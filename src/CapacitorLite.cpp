/*
  Capacitor.cpp - Library for measuring capacitance
  Created by Jon Nethercott @ Codewrite, 28 May 2018
  Released into the public domain under the GNU GPLv3
  https://github.com/codewrite/arduino-capacitor
*/

#include "Arduino.h"
#include "limits.h"
#include "CapacitorLite.h"

#if defined(__SAM3X8E__)
    #define STRAY_CAP (1750);
#elif defined(__PIC32MX3XX__)
    #define STRAY_CAP (1800);
#elif defined(__AVR_ATmega168__)
    #define STRAY_CAP (2800);
#elif defined(__AVR_ATmega328P__)
    #define STRAY_CAP (2630);
#elif defined(__AVR_ATmega32U4__)
    #define STRAY_CAP (3600);
#elif defined(__AVR_ATtinyX5__)
    #define STRAY_CAP (1900);
#else
    #define STRAY_CAP (2448);
#endif

unsigned int CapacitorLite::_inCapToGnd = STRAY_CAP;      // in pF *100

CapacitorLite::CapacitorLite(int outPin, int inPin)
{
    _outPin = outPin;
    _inPin = inPin;
    digitalWrite(_outPin, LOW);
    digitalWrite(_inPin, LOW);
    pinMode(_outPin, OUTPUT);
    pinMode(_inPin, OUTPUT);
    _showDebugMessages = false;
}

void CapacitorLite::Calibrate(unsigned int strayCap)
{
    _inCapToGnd = strayCap;
}

void CapacitorLite::ShowDebug(bool on)
{
    _showDebugMessages = on;
    Serial.print(F("Stray Cap: "));
    Serial.print(_inCapToGnd);
    Serial.print(F(" (pF*100)"));
}

#if defined(__SAM3X8E__)
void CapacitorLite::SetResolution(int bits)
{
    analogReadResolution(bits);
    _maxAdcValue = (1 << bits) - 1;
}
#endif

// Capacitor under test between _outPin and _inPin
// Returns capacitance in pF * 100
unsigned int CapacitorLite::Measure()
{
    long capacitance;

#if defined(__AVR_ATtinyX5__)
        pinMode(analogInputToDigitalPin(_inPin-A0), INPUT);  // Convert AnalogPin to Digital for ATTinyX5 chips
#else
        pinMode(_inPin, INPUT);                 // Rising high edge on OUT_PIN
#endif
    digitalWrite(_outPin, HIGH);

	int val = analogRead(_inPin);

    digitalWrite(_outPin, LOW);
#if defined(__AVR_ATtinyX5__)
    pinMode(analogInputToDigitalPin(_inPin-A0), OUTPUT);  // Convert AnalogPin to Digital for ATTinyX5 chips, Clear everything for next measurement
#else
    pinMode(_inPin, INPUT);                 // Rising high edge on OUT_PIN
#endif

    // Calculate result
    capacitance = (long)val * _inCapToGnd / (max(_maxAdcValue - val, 1));
    if (_showDebugMessages)
    {
        Serial.print(F("Cap adc: "));
        Serial.println(val);
    }

    return (unsigned int)min(capacitance, UINT_MAX);
}
