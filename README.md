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

<hr/>

I originally came up with this idea back in 2014. For some explanation of how it works please have a look at these links: 
https://hackaday.com/2014/01/22/capacitance-measurement-with-the-arduino-uno/#more-113173 
http://wordpress.codewrite.co.uk/pic/2014/01/21/cap-meter-with-arduino-uno/ 
http://wordpress.codewrite.co.uk/pic/2014/01/25/capacitance-meter-mk-ii/ <br/>
When I get a chance I'll update the Wiki with some of the above information and some oscilloscope screen shots...
