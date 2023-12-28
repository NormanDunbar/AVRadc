#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

namespace adc {

    // a void function to call when the interrupt occurs.
    typedef void (*onInterrupt)(uint16_t);


    // Typedef for the reference voltage source. This can be
    // Internal from 1.1V bandgap, 
    // External on AVCC pin, 
    // External on AREF pin.
    typedef enum refVoltage : uint8_t {
        VREF_EXTERNAL_AREF = 0,
        VREF_EXTERNAL_AVCC = (1 << REFS0),
        VREF_INTERNAL = ((1 << REFS1) | (1 << REFS0))
    } refVoltage_t;


    // Enumeration of the pin where the voltage to compare
    // with the reference voltage. This can be from:
    // Any of the 6 (or 8) Arduino analogue pins, 
    // The internal temperature sensor,
    // A fixed 1.1V bandgap voltage,
    // A fixed 0V GND signal.
    typedef enum testVoltage : uint8_t {
        VTEST_A0_PC0 = 0,
        VTEST_A1_PC1,
        VTEST_A2_PC2,
        VTEST_A3_PC3,
        VTEST_A4_PC4,
        VTEST_A5_PC5,
        // Only on surface mount ATmega328PAU.
        VTEST_A6,
        VTEST_A7,
        // Internal temperature sensor.
        VTEST_TEMP_SENSOR,
        // Internal 1.1V.
        VTEST_BANDGAP = ((1 << MUX3) | 
                         (1 << MUX2) | 
                         (1 << MUX1)),
        // Internal GND (0V)
        TEST_GND = VTEST_BANDGAP | (1 << MUX0)
    } testVoltage_t ;


    // What alignment do we want for the result?
    typedef enum alignment : uint8_t {
        ALIGN_RIGHT = 0,
        ALIGN_LEFT = (1 << ADLAR)
    } alignment_t;


    // The ADC needs to run between 50-200 KHz to get
    // accurate readings. We need a prescaler for our
    // 16 MHz Arduino boards.
    typedef enum prescaler : uint8_t {
        PRESCALE_1 = 0,     // Divide by 1
        PRESCALE_2,         // Divide by 2
        PRESCALE_4,         // Divide by 4
        PRESCALE_8,         // Divide by 8
        PRESCALE_16,        // Divide by 16
        PRESCALE_32,        // Divide by 32
        PRESCALE_64,        // Divide by 64
        PRESCALE_128        // Divide by 128
        } prescaler_t;


    // Do we require the ADC to auto-trigger?
    typedef enum autotrigger : uint8_t {
        AUTOTRGR_DISABLED = 0,
        AUTOTRGR_ENABLED = (1 << ADATE)
    } autotrigger_t;


    // The various auto-trigger sources.
    typedef enum autosource : uint8_t {
        AUTOTRGR_FREE_RUN = 0,
        AUTOTRGR_AC,
        AUTOTRGR_INT0,
        AUTOTRGR_T0_MATCH_A,
        AUTOTRGR_T0_OVERFLOW,
        AUTOTRGR_T1_MATCH_B,
        AUTOTRGR_T1_OVERFLOW,
        AUTOTRGR_T1_INPCAP
    } autosource_t;



    class AVR_adc {

    public:

        // Constructor. There's no destructor as the one
        // AVRac object will never be destructed.
        AVR_adc();


        // Allow a user function to be attached to the interrupt.
        void onInterruptTriggered(onInterrupt functionName);


        // Configure, but don't start, the ADC. The defaults are 
        // suitable for a full 10 bit result, aligned right and for
        // a 16 MHz board - similar to an Arduino.
        bool begin(
            refVoltage_t refVoltage,
            testVoltage_t testPin,
            alignment_t alignment = adc::ALIGN_RIGHT,
            prescaler_t prescaler = adc::PRESCALE_128,
            autotrigger_t autoTrigger = adc::AUTOTRGR_DISABLED,
            autosource_t autoSource = adc::AUTOTRGR_FREE_RUN);


        // We are done with the ADC, power it off.
        void end() {
            PRR |= (1 << PRADC);
            isEnabled = false;
        }


        // ADC is enabled, start a conversion. This is
        // required if using ADC_AUTOTRGR_DISABLED or 
        // ADC_AUTOTRGR_FREE_RUN.
        void start() {
            ADCSRA |= (1 << ADSC);
            isEnabled = true;
        }


        // Execute the AC interrupt code. This should be called
        // from the ISR.
        void interrupt() {
            if (interruptFunction) {
                interruptFunction(ADCW);
            }
        }


    private:

        // (User) function to call when triggered.
        onInterrupt interruptFunction;

        // 
        bool isEnabled;

    };

} // End of namespace adc.


//There will be one of these in the cpp file for this class.
extern adc::AVR_adc AVRadc;

#endif // ADC_H

