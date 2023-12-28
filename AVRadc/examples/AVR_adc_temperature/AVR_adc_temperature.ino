//=============================================================
// A small test of the AVRadc object which allows easy use of
// the ADC's interrupt. This example displays the internal 
// temperature of the ATmega328P.
//
// The AVRadc object should be configured in single shot mode
// with the internal sensore as the test voltage. The reference
// voltage must be the internal 1.1V.
//
// NOTE: This is not very accurate. When running in a loop, you
// will see the odd strange reading. However, it's not too bad.
//
// Norman Dunbar
// 4 November 2020.
//=============================================================
#include "AVR_adc.h"

// Somewhere to hold ADC values for loop() to use.
volatile uint16_t temperatureValue = 0;


// The interrupt routine.
void adcInterrupt(uint16_t adcReading) {
    // Grab the sensor reading for display
    // on the Serial Monitor.
    temperatureValue = adcReading;
}


void setup() {
    Serial.begin(9600);

    // Call adcInterrupt() every time the
    // ADC has a new value.
    AVRadc.onInterruptTriggered(adcInterrupt);

    // Initialise the ADC to run in single shot mode
    // reading data from the temp sensor with the 
    // reference voltage from the 1.1V internal source.
    // Only two parameters required here, the rest default
    // correctly.
    AVRadc.begin(adc::VREF_INTERNAL,
                 adc::VTEST_TEMP_SENSOR);

    // Config complete, start the ADC. This reading is
    // discarded.
    AVRadc.start();

    // Allow the ADC to settle down.
    delay(20);
}


void loop() {
    // Take another reading.
    AVRadc.start();

    // Display the temperature reading using one of many
    // ways to convert the ADC reading to degrees. See
    // the data sheet for details.
    float degreesC = (temperatureValue - 324.31) / 1.22;

    Serial.print("Arduino Temperature is: ");
    Serial.print(degreesC);
    Serial.print(" C which is ");

    // Convert to Fahrenheit. C * 1.8 + 32.
    Serial.print(degreesC * 1.8 + 32);
    Serial.println(" F.");

    // Delay a bit between readings.
    delay(1500);
}
