# AVR_adc Library

The `AVR_adc` library provides a simple interface to the Analog to Digital Converter's single interrupt on an Arduino running with an ATmega328P and *probably* works on an ATmega168 as well -- they are extremely similar.

The code has been tested on projects created in the Arduino IDE version 1.8.13, and also in PlatformIO (platformio.org), with no problems on either system.

## Arduino

To install the library:

* Click Sketch->Include Library->Add .ZIP Library;
* Navigate to where the zip file for this library is located on your system;
* Select it and click the OK button;
* Follow the prompts.


## PlatformIO

As of the time of typing, I have not built this library into a format suitable for uploading to the official PlatformIO Library Registry. 

This means you have to proceed as described below.

### Install into a Project

To install this library for a single project:

* Create the project in the normal manner.
* Create directory `lib/AVR_ac`.
* Unzip the latest releases Zip file.
* Uncompress it somewhere safe.
* Copy the contents of the `src` directory from the downlaod to the new directory you c reated above.
* That's it. You will not need any `lib_deps` to use the library.

Norman Dunbar

4 November 2020.
