/*
  Capacitor.h - Library for measuring capacitance
  Created by Jon Nethercott @ Codewrite, 28 May 2018
  Released into the public domain under the GNU GPLv3
  https://github.com/codewrite/arduino-capacitor
*/
#ifndef Capacitor_h
#define Capacitor_h

#include "Arduino.h"

// Stray capacitance and Pullup resistance will vary depending on board -
// For maximum accuracy calibrate with known capacitor.

class Capacitor
{
  public:
    Capacitor(int outPin, int inPin);
    void Calibrate(float strayCap, float pullupRes);
    float Measure();                // returns the capacitance in pF
    void ShowDebug(bool on);        // true to show debug messages 
  private:
    const int _maxAdcValue = 1023;
    static float _inCapToGnd;       // in pF
    static float _rPullup;          // in k ohms
    int _outPin;                    // For electrolytic capatitors, connect to positive end
    int _inPin;
    bool _showDebugMessages;
};

#endif
