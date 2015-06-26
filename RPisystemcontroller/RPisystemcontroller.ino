#include <CapacitiveSensor.h>
#include <avr/sleep.h>

/*
 * @author  Mahyar Koshkouei
 * @since   2015-06-18
 * @license GPLv3. Uses some code taken from http://playground.arduino.cc/Learning/ArduinoSleepCode
 *          which is also licensed under GPLv3.
 *
 * For portable game console project
 *
 * 1) If RPi is off, pressing and holding the start button for 5 seconds
 *    will turn on the power circuit (which in turn will turn on the RPi).
 *    Otherwise the Arduino Pro Mini will be in power saving mode with
 *    the select button used as a interrupt to wake it.
 *
 * 2) If RPi is on:
 *        - pressing select will make the corresponding pin HIGH.
 *        - touching the L button  will make the corresponding pin HIGH.
 *        - touching the R button  will make the corresponding pin HIGH.
 *
 */

const byte wakePin = 2;    // Pin used for waking up (Start button)
const byte shutdownOut = 9;// Shutdown Output
const byte lButtonOut = 8; // L button Output
const byte rButtonOut = 7; // R button Output
const byte startButtonOut = 6; // Start button Output
const byte rpiPower = 5;   // RPi Power
const byte capSend = 4;    // Send pin for capacitance touch
const byte lBtnIn = 3;     // Recieve pin for capacitance touch - L button
const byte rBtnIn = 2;     // Recieve pin for capacitance touch - R button

// 1 megaohm resistor between pins capSend & lBtnIn, pin lBtnIn is sensor pin, add wire, foil
CapacitiveSensor   lbtn_cs = CapacitiveSensor(capSend, lBtnIn);

// 1 megaohm resistor between pins capSend & rBtnIn, pin rBtnIn is sensor pin, add wire, foil
CapacitiveSensor   rbtn_cs = CapacitiveSensor(capSend, rBtnIn);

void wakeUpNow()        // here the interrupt is handled after wakeup
{
  // execute code here after wake-up before returning to the loop() function
  // timers and code using timers (serial.print and more...) will not work here.
  // we don't really need to execute any special functions here, since we
  // just want the thing to wake up
}

void setup()
{
  pinMode(shutdownOut, OUTPUT);   // Shutdown Output
  pinMode(lButtonOut, OUTPUT);   // L button Output
  pinMode(rButtonOut, OUTPUT);   // R button Output
  pinMode(startButtonOut, OUTPUT);   // Start button Output
  pinMode(rpiPower, INPUT);    // RPi Power
  //pinMode(6, OUTPUT);   // Power LED NO NEED ANYMORE
  //pinMode(5, INPUT);    // Low Battery Detection
  pinMode(wakePin, INPUT);    // Start button Input (interrupt enabled)

  //digitalWrite(13, HIGH);

  /* Now it is time to enable an interrupt. In the function call
   * attachInterrupt(A, B, C)
   * A   can be either 0 or 1 for interrupts on pin 2 or 3.
   *
   * B   Name of a function you want to execute while in interrupt A.
   *
   * C   Trigger mode of the interrupt pin. can be:
   *             LOW        a low level trigger
   *             CHANGE     a change in level trigger
   *             RISING     a rising edge of a level trigger
   *             FALLING    a falling edge of a level trigger
   *
   * In all but the IDLE sleep modes only LOW can be used.
   */
  //attachInterrupt(0, wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function
  // wakeUpNow when pin 2 gets LOW
}

void sleepNow()         // here we put the arduino to sleep
{
  /* Now is the time to set the sleep mode. In the Atmega8 datasheet
   * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
   * there is a list of sleep modes which explains which clocks and
   * wake up sources are available in which sleep mode.
   *
   * In the avr/sleep.h file, the call names of these sleep modes are to be found:
   *
   * The 5 different modes are:
   *     SLEEP_MODE_IDLE         -the least power savings
   *     SLEEP_MODE_ADC
   *     SLEEP_MODE_PWR_SAVE
   *     SLEEP_MODE_STANDBY
   *     SLEEP_MODE_PWR_DOWN     -the most power savings
   *
   * For now, we want as much power savings as possible, so we
   * choose the according
   * sleep mode: SLEEP_MODE_PWR_DOWN
   *
   */
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here

  sleep_enable();          // enables the sleep bit in the mcucr register
  // so sleep is possible. just a safety pin

  /* Now it is time to enable an interrupt. We do it here so an
   * accidentally pushed interrupt button doesn't interrupt
   * our running program. if you want to be able to run
   * interrupt code besides the sleep function, place it in
   * setup() for example.
   *
   * In the function call attachInterrupt(A, B, C)
   * A   can be either 0 or 1 for interrupts on pin 2 or 3.
   *
   * B   Name of a function you want to execute at interrupt for A.
   *
   * C   Trigger mode of the interrupt pin. can be:
   *             LOW        a low level triggers
   *             CHANGE     a change in level triggers
   *             RISING     a rising edge of a level triggers
   *             FALLING    a falling edge of a level triggers
   *
   * In all but the IDLE sleep modes only LOW can be used.
   */

  attachInterrupt(0, wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function
  // wakeUpNow when pin 2 gets LOW

  sleep_mode();            // here the device is actually put to sleep!!
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP

  sleep_disable();         // first thing after waking from sleep:
  // disable sleep...
  detachInterrupt(0);      // disables interrupt 0 on pin 2 so the
  // wakeUpNow code will not be executed
  // during normal running time.
  
  {
    byte temp = 1;    // 1 if button has not been held down for long enough
    boolean current = LOW;            // Current state of the button
    boolean previous = LOW;           //Current state of the button
    int count;   // How long the button was held (secs)
    unsigned long firstTime = 0;          // how long since the button was first pressed 
    
    while (temp){
      current = digitalRead(wakePin);
      if (current == LOW && firstTime = 0) {
        firstTime = millis();
      } else if (current == LOW &&
    }
    
    if (current == LOW && previous == HIGH && millis() - firstTime > 200) {
      firstTime = millis();    // if the buttons becomes press remember the time
    }

    if (current == LOW && ((millis() - firstTime) % 1000) < 20 && millis() - firstTime > 500) {
      ledblink(1, 50, ledPin); // Each second the button is held blink the indicator led and
      count++;                // and 1 to the counter
    }
    if (current == HIGH && previous == LOW && count >= 3 && count < 6) {
      ledblink(10, 200, ledPin); // When the button is released if the counter is between the
    }                          // two numbers (3-6 blinks or secs) run the program

    if (current == HIGH && previous == LOW && count >= 0 && count < 3) {
      Serial.println("It Works!!!"); //This is where you would put you differnt functions
    }                                // with diffent time parameters

    if (current == HIGH) { // reset the counter if the button is not pressed
      count = 0;
    }

    previous = current;
  }
}

void loop()
{
  if (digitalRead(rpiPower) == LOW) {
    attachInterrupt(0, wakeUpNow, LOW);   // When interrupt 0, put device to sleep.
    sleepNow();     // sleep function called here
  } else {

  }
}

