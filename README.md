Capacitor
=======
Arduino library to measure capacitance from 0.2pF to over 100uF with no external hardware.

Introduction
==========
This library allows capacitance to be measured between two pins. One pin must be an analog pin.

Usage
=====
Declare a Capacitor object, e.g.:
````cpp
Capacitor cap1(7, A2);
````
To measure the value of a capacitor (e.g. in the case above between pin D7 and A2) call the _Measure()_ method, e.g.:
````cpp
float val = cap1.Measure()
````
Returns the capacitance in picofarads, in this case in the variable ````val````.<br/>
(note that 1nF = 1000pF and 1uF = 1000000pF)
