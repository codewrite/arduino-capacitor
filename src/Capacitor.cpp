/*
  Capacitor.cpp - Library for measuring capacitance
  Created by Jon Nethercott @ Codewrite, 28 May 2018
  Released into the public domain under the GNU GPLv3
  https://github.com/codewrite/arduino-capacitor
*/

#include "Arduino.h"
#include "Capacitor.h"

#if defined(__AVR_ATmega328P__)
    #define STRAY_CAP (26.29);
    #define R_PULLUP (39.40);
#elif defined(__AVR_ATmega168__)
    #define STRAY_CAP (24.48);
    #define R_PULLUP (34.80);
#else
    #define STRAY_CAP (24.48);
    #define R_PULLUP (34.80);
#endif

float Capacitor::_inCapToGnd = STRAY_CAP;      // in pF
float Capacitor:: _rPullup = R_PULLUP;         // in k ohms

Capacitor::Capacitor(int outPin, int inPin)
{
    _outPin = outPin;
    _inPin = inPin;
    digitalWrite(_outPin, LOW);
    digitalWrite(_inPin, LOW);
    pinMode(_outPin, OUTPUT);
    pinMode(_inPin, OUTPUT);
    _showDebugMessages = false;
}

void Capacitor::Calibrate(float strayCap, float pullupRes)
{
    _inCapToGnd = strayCap;
    _rPullup = pullupRes;
}

void Capacitor::ShowDebug(bool on)
{
    _showDebugMessages = on;
    Serial.print(F("Stray Cap: "));
    Serial.print(_inCapToGnd);
    Serial.print(F(" (pF), R Pullup: "));
    Serial.print(_rPullup);
    Serial.println(F(" (kOhm)"));
}

// Capacitor under test between _outPin and _inPin
float Capacitor::Measure()
{
    float capacitance;

    pinMode(_inPin, INPUT);                 // Rising high edge on OUT_PIN
    digitalWrite(_outPin, HIGH);
    int val = analogRead(_inPin);
    digitalWrite(_outPin, LOW);
    pinMode(_inPin, OUTPUT);                // Clear everything for next measurement

    if (val < _maxAdcValue - _maxAdcValue / 42)
    {
        //Low value capacitor - calculate result
        capacitance = (float)val * _inCapToGnd / (float)(max(_maxAdcValue - val, 1));
        if (_showDebugMessages)
        {
            Serial.print(F("Low val cap adc: "));
            Serial.println(val);
        }
    }
    else
    {
        //Big capacitor (>1nF) - so use RC charging method
        delayMicroseconds(2);               // Wait for capacitor to discharge

        pinMode(_outPin, INPUT_PULLUP);     // Start charging with internal pullup
        unsigned long u1 = micros();
        
        delayMicroseconds(200);             // Charge for about 200us

        pinMode(_inPin, INPUT);             // Stop charging
        unsigned long u2 = micros();        // Note: overflows approx every 70 mins
        digitalWrite(_outPin, HIGH);        // Pull both sides of capacitor up
        pinMode(_outPin, OUTPUT);
        int adcVal = analogRead(_inPin);    // Now read level capacitor has charged up to
        val = _maxAdcValue - adcVal;

        unsigned long u3 = 0;
        unsigned long u4 = 0;
        if (val < _maxAdcValue / 100)
        {
            // Really big capacitor (>1uF)
            pinMode(_outPin, INPUT_PULLUP);
            pinMode(_inPin, OUTPUT);        // Start charging again
            u3 = micros();
            delay(20);
            pinMode(_inPin, INPUT);         // Stop charging
            u4 = micros();
            digitalWrite(_outPin, HIGH);    // Pull both sides of capacitor up
            pinMode(_outPin, OUTPUT);
            adcVal = analogRead(_inPin);
            val = _maxAdcValue - adcVal;    // Measure level wrt Vcc (not Gnd)

            while (adcVal < _maxAdcValue - _maxAdcValue / 8)
            {
                pinMode(_inPin, INPUT_PULLUP);  // Discharge slowly to about 0.6V
                delay(5);
                pinMode(_inPin, INPUT);
                adcVal = analogRead(_inPin);
            }
        }
        else
        {
            pinMode(_inPin, INPUT_PULLUP);  // Discharge slowly for a bit
            delay(1);
            pinMode(_inPin, INPUT);
        }

        // Work out the actual charge time
        unsigned long t = (u2 > u1 ? u2 - u1 : u1 - u2) + (u4 > u3 ? u4 - u3 : u3 - u4);
      
        digitalWrite(_outPin, LOW);           // Discharge remainder quickly
        digitalWrite(_inPin, LOW);
        pinMode(_inPin, OUTPUT);

        //Calculate result
        capacitance = -(float)t * 1000.0 / _rPullup
                        / log(1.0 - (float)val / (float)_maxAdcValue);
        if (_showDebugMessages)
        {
            Serial.print(F("High val cap t: "));
            Serial.print(t);
            Serial.print(F(", adc: "));
            Serial.println(val);
        }
    }
    return capacitance;
}
