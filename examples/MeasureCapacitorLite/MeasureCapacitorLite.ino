/*
    MeasureCapacitorLite

    Measures the capacitance between D7 and A2.
    Prints the result to the Serial Monitor.

    This is the "Lite" version. It uses a lot less program storage and is a lot quicker than the "Standard" version.
    However, it will only measure from 0.2pF to 655pF.
    Also, note that Measure() returns an unsigned int (pF * 100) rather than a float.

    For more detailed usage examples (e.g. liquid level measurement) and explanation of how the library works please see the wiki:
    https://github.com/codewrite/arduino-capacitor/wiki

    This example code is in the public domain.
*/
#include <CapacitorLite.h>

// Capacitor under test.
CapacitorLite cap1(7,A2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(cap1.Measure());  // Measure the capacitance (in pF * 100), print to Serial Monitor
  delay(1000);                     // Wait for 1 second, then repeat
}
