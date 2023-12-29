# AVR_adc Library

The `AVR_adc` library provides a simple interface to the Analog to Digital Converter interrupt on an Arduino running with an ATmega328P and *probably* works on an ATmega168 as well -- they are extremely similar.

The code has been tested on projects created in the Arduino IDE version 1.8.13, and also in PlatformIO (platformio.org), with no problems on either system. *However*, the library as supplied is not in the correct format form PlatformIO projects---but you can extract all the source files from the `src` directory, and add those to your project.

## Arduino

To install the library:

* Click Sketch->Include Library->Add .ZIP Library;
* Navigate to where the zip file for this library is located on your system;
* Select it and click the OK button;
* Follow the prompts.


## PlatformIO

As of the time of typing, I have not built this library into a format suitable for uploading to the official PlatformIO Library Registry.

* Unzip the downloaded zip file.
* In your project's directory, create a new directory under `lib`, named `AVR_adc`.
* Copy files from the `src` directory *of the extracted zip file* to thie new directory you created above.
* That's it. You only need to `#include` the appropriate header file in your code, and the library will be used without requiring any `lib_deps` settings. Easy!


Norman Dunbar

27 October 2020 (onwards!).
