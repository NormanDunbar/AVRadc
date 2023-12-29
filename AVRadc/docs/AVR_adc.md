# AVRadc Library

This library can be usd with the Arduino IDE and also with other systems, such as PlatformIO. The library makes handling the Analog Comparator (AC) interrupt on the Arduino and/or a plain ATmega328P, very simple indeed.

## Library Documentation

### Types

#### onInterrupt

This type defines the function that will be called when the interrupt is triggered. The function must be as follows:

```
void interruptFunction(uint16_t adcReading) {
    // Do something on interrupt here;
    // The parameter adcReading is the latest
    // value from the ADC.
}
```

It will be called if, and only if, it has been attached to the interrupt by the `onInterruptTriggered()` function. See below for details.

#### refVoltage_t

The reference voltage is the voltage that the ADC will compare the test voltage against. The permitted values are:

* **adc::VREF_EXTERNAL_AREF**  The reference voltage is the voltage on the `AREF` pin.
* **adc::VREF_EXTERNAL_AVCC**  The reference voltage is the voltage on the `AVCC` pin - the board supply voltage, normally, as the ATmega328P should be powered with the same (or very very similar) voltage on the `VCC` pin and the `AVCC` pin.
* **adc::VREF_INTERNAL**  The internal 1.1V bandgap reference voltage will be used as the reference voltage.


#### testVoltage_t

The test voltage is the pin where the ADC will measure the unknown voltage. The permitted values are:

* **adc::VTEST_A0_PC0**     The ADC will measure the voltage on Arduino pin `A0`, AVR pin `PC0`.
* **adc::VTEST_A1_PC1**     The ADC will measure the voltage on Arduino pin `A1`, AVR pin `PC1`.
* **adc::VTEST_A2_PC2**     The ADC will measure the voltage on Arduino pin `A2`, AVR pin `PC2`.
* **adc::VTEST_A3_PC3**     The ADC will measure the voltage on Arduino pin `A3`, AVR pin `PC3`.
* **adc::VTEST_A4_PC4**     The ADC will measure the voltage on Arduino pin `A4`, AVR pin `PC4`.
* **adc::VTEST_A5_PC5**     The ADC will measure the voltage on Arduino pin `A5`, AVR pin `PC5`.
* **adc::VTEST_A6**     The ADC will measure the voltage on Arduino pin `A6`, only available on the surface mount ATmega328PAU microcontroller.
* **adc::VTEST_A7**     The ADC will measure the voltage on Arduino pin `A7`, only available on the surface mount ATmega328PAU microcontroller.
* **adc::VTEST_TEMP_SENSOR**     The ADC will measure the voltage on the internal temperature monitoring sensor. This option requires that the reference voltage be `adc::VREF_INTERNAL` and the auto triggering is not enabled.
* **adc::VTEST_BANDGAP**     The ADC will measure the internal 1.1V voltage as opposed to using any external pins.
* **adc::VTEST_GND**     The ADC will measure the ground voltage. 

The latter two are a little weird. If you use `adc::VTEST_BANDGAP` then the ADC always reads 227-229 (at least, mine does) which works out at 1.11-1.12V. Using `adc::VTEST_GND` returns zero on my devices, representing GND. I assume that these are perhaps configuration options?


#### alignment_t

The ADC can return it's 10 bit result in one of two ways. The two parameters permitted are:


* **adc::ALIGN_RIGHT**  In Right alignment, the 10 bit results is returned in normal mode where the highest bits are bits 9 and 8 of the result, and the lowest bits are bits 7 through 0. Bits 15 through 10 are garbage. 
* **adc::ALIGN_LEFT**  In left alignment, the result comes with the highest bits in bits 15 through 8 or the result, and the two lowest bits are in bits 7 and 6. The remaining bits, 5 through 0, are garbage.

#### prescaler_t

The ADC must be clocked at a frequency between 50 and 200 KHz. The system clock, 16 MHz on an Arduino, needs to be scaled down to suit. The permitted prescaler values are:

* **adc::PRESCALE_1**   The system clock frequency is divided by 1.
* **adc::PRESCALE_2**   The system clock frequency is divided by 2.
* **adc::PRESCALE_4**   The system clock frequency is divided by 4.
* **adc::PRESCALE_8**   The system clock frequency is divided by 8.
* **adc::PRESCALE_16**   The system clock frequency is divided by 16.
* **adc::PRESCALE_32**   The system clock frequency is divided by 32.
* **adc::PRESCALE_64**   The system clock frequency is divided by 64.
* **adc::PRESCALE_128**   The system clock frequency is divided by 128.

The default on an Arduino is `adc::PRESCALE_128` which gives a frequency of 125 KHz. You can run the ADC at a higher frequency but the results will not give the full 10 bit resolution.

#### autotrigger_t

The ADC can be run in single shot or auto triggering modes. The permitted values are:

* **adc::AUTOTRGR_DISABLED**    The ADC is in single shot mode.
* **adc::AUTOTRGR_ENABLED**     The ADC will self trigger, after being manually started.

#### autosource_t

If the ADC is auto triggereing, then one of 8 different sources can be configured. The interrupt flags for the various sources, is used to start the next conversion. This will occur when the flag changes from 0 to 1.

The permitted values are:

* **adc::AUTOTRGR_FREE_RUN**    The ADC will start a new conversion as soon as the current one completes.
* **adc::AUTOTRGR_AC**  The ADC will start a new conversion whenever the Analog Converter's interrupt flag is set.
* **adc::AUTOTRGR_INT0**  The ADC will start a new conversion whenever the `INT0` interrupt flag is set.
* **adc::AUTOTRGR_T0_MATCH_A**  The ADC will start a new conversion whenever Timer/counter 0's compare match A interrupt flag is set.
* **adc::AUTOTRGR_T0_OVERFLOW**  The ADC will start a new conversion whenever  Timer/counter 0's overflow interrupt flag is set.
* **adc::AUTOTRGR_T1_MATCH_B**  The ADC will start a new conversion whenever Timer/counter 1's compare match B interrupt flag is set.
* **adc::AUTOTRGR_T1_OVERFLOW**  The ADC will start a new conversion whenever  Timer/counter 1's overflow interrupt flag is set.
* **adc::AUTOTRGR_T1_INPCAP**  The ADC will start a new conversion whenever Timer/counter 1's Input capture Unit's interrupt flag is set.


### Functions


**`void AVRadc.begin(...)`**

The full template for the `begin()` function is:

```
    bool begin(refVoltage_t refVoltage,
               testVoltage_t testPin,
               alignment_t alignment = ADC_ALIGN_RIGHT,
               prescaler_t prescaler = ADC_PRESCALE_128,
               autotrigger_t autoTrigger = ADC_AUTOTRGR_DISABLED,
               autosource_t autoSource = ADC_AUTOTRGR_FREE_RUN);
```

The parameters allow the ADC to be configured as desired, but the ADC will not begin making conversions until manually started.

The defaults cause the ADC to:

* Return its result right aligned;
* To run with a divide by 128 prescaler - which is the default for an Arduino running at 16 MHz;
* To run in single shot mode.

The only mandatory parameters are the reference voltage and the test voltage parameters.

**NOTE**: Calling `begin()` will configure the ADC but will not start it running. To do that, you must call `start()`.



For example:

```
// Configure the ADC.
bool ok = AVRadc.begin(adc::VREF_EXTERNAL_AREF,
                       adc::VTEST_A0_PC0);

if (ok) {
    // Attach my interrupt function.
    AVRadc.onInterruptTriggered(myADCInterrupt);

    // And start converting.
    AVRadc.start();
} else {
    // Handle failure to configure ADC here.
    ...
}
```


**`void AVRadc.end()`**

Disables the ADC and any interrupt handling by powering off the ADC internally. It can be restarted by calling `begin()` again.

For example:

```
AVRadc.end();
```

All that  this function does is to power off the AC. It does not reconfigure the AC in any way -- all your previous settings remain. No interrupts will trigger when disabled, but the interrupt flag internal to the ATmega328P, will be set if the AC inputs determine.



**`void AVRadc.onInterruptTriggered(onInterrupt functionName)`**

This function attaches a function in your code to the INT0 interrupt. The function will be called each time the interrupt is triggered.

```
void myFunction() {
    digitalWrite(13, HIGH);
}

void setup() {
...
   AVRadc.onInterruptTriggered(myFunction);
...
}
```

**`void AVRadc.interrupt()`**

This function is intended for internal use only. It is what allows the ISR (Interrupt Service Routine) for the AC to execute the function passed to `onInterruptTriggered()`.

The `interrupt()` function can be called from user code, as a means of manually executing the interrupt function, but this feature should be used with great care.

The function has to be `public` because the ISR isn't part of the `AVRadc` object, and cannot be. (Unless you know different?)


### Your Interrupt Function

You need to write a function to handle the interrupt, when one occurs. The function should look like this:

```
void myInterruptFunction(uint16_t adcReading) {
    // Do something ...
}
```

The function takes a 16 bit unsigned integer parameter and does not return a result.



## Example Sketches


### Example Arduino Sketch

The following sketch shows the ADC interrupt being used to read a voltage level from a potentiometer attached to pin `A3` and to display the value on the Serial Monitor.


```
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
}
```

### Example PlatformIO Sketch

The Arduino sketch above can be replicated in AVR C++ for the PlatformIO system, as per the following code. Note, we have to do our own USART initialisation etc! Thre's no Arduino handholding here I'm afraid.

```
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
#include <stdlib.h>
#include <util/delay.h>

// Somewhere to hold ADC values for loop() to use.
volatile uint16_t adcValue = 0;


// The interrupt routine.
void adcInterrupt(uint16_t adcReading) {
    // Grab the ADC reading for display 
    // on the Serial Monitor.
    adcValue = adcReading;
}


// Initialise the USART to 8-n-1 at 9600 baud.
void initUSART_9600() {
	// POwer up the USART.
	PRR &= ~(1 << PRUSART0);
	
	// Double speed mode
	UCSR0A = (1 << U2X0);
	
	// 8-N-1.
	UCSR0C = (1 << UCSZ01)|(1 << UCSZ00);
	
	// 9600 baud.
	UBRR0H = 0;
	UBRR0L = 207;
	
	// Enable TX only.
	UCSR0B |= (1 << TXEN0);
}


// Print a single character to the USART.
void USARTputc(const uint8_t c) {
	// Wait for the USART to be ready.
	while (!(UCSR0A & (1 << UDRE0))) ; // Wait until UDRE0 set.
	
	// Transmit a byte.
	UDR0 = c;
}


// Print a text strinmg to the USART.
void USARTprintText(const char *s) {
	// Print each character.
	while (*s) {
		USARTputc(*s++);
	}
}


// Print a signed 16 bit number to the USART.
void USARTprintNumber(const int16_t n) {
	// Converstion buffer, big enough for "65536\0".
	char buffer[6] = {0};
	
	// Convert from number to text. Needs stdlib.h.
	if (itoa((int)n, &buffer[0], 10) != NULL ) {
		USARTprintText(buffer);
	} 	
}


int main() {
	//--------------------------------------------------------
	//                                                   SETUP
	//--------------------------------------------------------
    initUSART_9600();

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
    _delay_ms(20);
    
    // Don't forget. ADC won't work without it!
    sei();


	//--------------------------------------------------------
	//                                                    LOOP
	//--------------------------------------------------------
	while (1) {
    	// Display the current ADC reading.
    	USARTprintText("ADC Value: ");
    	USARTprintNumber(adcValue);
    	USARTprintText("\r\n");
    	
    	// Delay a bit.
    	_delay_ms(250);
    }
}
```

