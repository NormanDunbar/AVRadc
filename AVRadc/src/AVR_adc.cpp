#include "AVR_adc.h"

namespace adc {

    // Constructor.
    AVR_adc::AVR_adc() {
        interruptFunction = nullptr;
        isEnabled = false;
    }


    // Configure, but don't start, the ADC. The defaults are 
    // suitable for a full 10 bit result, aligned right and for
    // a 16 MHz board - similar to an Arduino.
    bool AVR_adc::begin(refVoltage_t refVoltage,
                        testVoltage_t testPin,
                        alignment_t alignment,
                        prescaler_t prescaler,
                        autotrigger_t autoTrigger,
                        autosource_t autoSource) {

        // Validate incompatible options.
        // If we are using the temperature sensor, we must also
        // use the internal voltage reference. There is no auto
        // triggering allowed with the temperature sensor.
        if (testPin == adc::VTEST_TEMP_SENSOR &&
            refVoltage != adc::VREF_INTERNAL &&
            autoTrigger == adc::AUTOTRGR_ENABLED) {
                return false;
        }

        // Parameters are now validated. Let's do it!

        // * Turn on power to the ADC.
        PRR &= ~(1 << PRADC);

        // * Set reference, alignment and source;
        ADMUX = (refVoltage | alignment | testPin);

        // If the Analog Comparator is using the MUX, don't
        // mess with it. Preserve the ACME bit.
        ADCSRB &= (1 << ACME);
        
        // * Enable auto trigger source, if required, we will 
        // enable autotriggering later.
        if (autoTrigger == adc::AUTOTRGR_ENABLED) {
            ADCSRB |= autoSource;
        }

        // * Power off the digital input buffer, as appropriate;
        if (testPin <= adc::VTEST_A5_PC5) {
            DIDR0 |= (1 << testPin);
        }

        // * Set prescaler, interrupt, auto trigger if required,
        // and enable the ADC. This doesn't start a conversion.
        ADCSRA = ((1 << ADEN) | (1 << ADIE) | 
                  prescaler | 
                  autoTrigger);

        return true;
    }


    // Make sure we have a function to call when the
    // CA interrupt fires.
    void AVR_adc::onInterruptTriggered(onInterrupt functionName) {
        // Probably best not to change this when enabled!
        if (!isEnabled) {
            interruptFunction = functionName;
        }
    }

} // End of namespace

// Define our one instance of the AVR_adc class.
adc::AVR_adc AVRadc;


// This is the ISR for the ADC interrupt. It will
// call the required user function if there is one.
ISR(ADC_vect) {
    AVRadc.interrupt();
}

