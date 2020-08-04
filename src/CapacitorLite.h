/*
  CapacitorLite.h - Library for measuring capacitance
  Created by Jon Nethercott @ Codewrite, 28 May 2018
  Released into the public domain under the GNU GPLv3
  https://github.com/codewrite/arduino-capacitor
*/
#ifndef CapacitorLite_h
#define CapacitorLite_h

#include "Arduino.h"

// Stray capacitance and Pullup resistance will vary depending on board -
// For maximum accuracy calibrate with known capacitor.

class CapacitorLite
{
  public:
    CapacitorLite(int outPin, int inPin);
    void Calibrate(unsigned int strayCap);
    unsigned int Measure();         // returns the capacitance in pF * 100
    void ShowDebug(bool on);        // true to show debug messages 
#if defined(__SAM3X8E__)
    void SetResolution(int bits);
#endif
  private:
    unsigned int _maxAdcValue = 1023;
    static unsigned int _inCapToGnd;       // in pF * 100
    int _outPin;
    int _inPin;
    bool _showDebugMessages;
};

#endif
