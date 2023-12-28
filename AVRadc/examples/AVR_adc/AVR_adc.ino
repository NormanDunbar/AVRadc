//=============================================================
// A small test of the AVRadc object which allows easy use of
// the ADC's interrupt.
//
// Connect a potentiometer to VCC and GND, then connect the
// wiper to pin A3. The ADC will read from that pin and will
// display the resulting conversion value on the Serial
// Monitor.
//
// Each time that the ADC has a new value, it will call the
// adcInterrupt() function. The new value will be copied to a
// volatile variable used in the loop function to display the
// value on the Serial Monitor.
//
// NOTE: The speed at which the ADC does conversions far far
// outweighs the speed at which the loop() function can be
// called fast enough to display the results! This example
// definitely misses a number of conversion values as they are
// written into adcValue by the interrupt function, but are
// then overwritten by the next conversion result while loop()
// is still getting its act together!
//
// Norman Dunbar
// 4 November 2020.
//=============================================================
#include "AVR_adc.h"

// Somewhere to hold ADC values for loop() to use.
volatile uint16_t adcValue = 0;


// The interrupt routine.
void adcInterrupt(uint16_t adcReading) {
    // Grab the ADC reading for display
    // on the Serial Monitor.
    adcValue = adcReading;
}


void setup() {
    Serial.begin(9600);

    // Call adcInterrupt() every time the
    // ADC has a new value.
    AVRadc.onInterruptTriggered(adcInterrupt);

    // Initialise the ADC to run in free running mode
    // reading data from pin A3 with the reference
    // voltage taken from pin AVCC.
    AVRadc.begin(adc::VREF_EXTERNAL_AVCC,
                 adc::VTEST_A3_PC3,
                 adc::ALIGN_RIGHT,
                 adc::PRESCALE_128,
                 adc::AUTOTRGR_ENABLED,
                 adc::AUTOTRGR_FREE_RUN);

    // Config complete, start the ADC.
    AVRadc.start();

    // Allow the ADC to settle down.
    delay(20);
}


void loop() {
    // Display the current ADC reading.
    Serial.print("ADC Value: ");
    Serial.println(adcValue);
    delay(250);
}
