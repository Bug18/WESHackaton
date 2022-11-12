#include <Arduino.h>

uint32_t past_delta;
uint32_t delta;

uint32_t past_delta_sum;
uint32_t delta_sum;

volatile uint16_t Overflows = 0;

ISR(TIMER1_OVF_vect)
{
  Overflows++;
}

ISR(TIMER1_CAPT_vect){
    static bool firstRisingPassed = false;
    static uint32_t last_time_read = 0;

    uint16_t overflows = Overflows;

    // If an overflow happened but has not been handled yet
    // and the timer count was close to zero, count the
    // overflow as part of this time.
    if ((TIFR1 & _BV(TOV1)) && (ICR1 < 1024))
        overflows++;
    
    if(firstRisingPassed){
        past_delta = delta;
        
        uint32_t current_time = overflows;
        current_time = (current_time << 16) | ICR1;

        delta = current_time - last_time_read;

        if(past_delta * 1.5 < delta){       //end of cycle
            past_delta_sum = delta_sum + delta;
            
            delta_sum = 0;                  // NGL fix
            past_delta = 0;
        } else {
            delta_sum += delta;
        }
    }
    last_time_read = overflows;
    last_time_read = (last_time_read << 16) | ICR1;
    
    firstRisingPassed = true;
}

void setupRPMTimer() {
    noInterrupts ();  // protected code
    // reset Timer 1
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    TIMSK1 = 0;

    TIFR1 |= _BV(ICF1); // clear Input Capture Flag so we don't get a bogus interrupt
    TIFR1 |= _BV(TOV1); // clear Overflow Flag so we don't get a bogus interrupt

    TCCR1B = _BV(CS10) | // start Timer 1, no prescaler
            _BV(ICES1); // Input Capture Edge Select (1=Rising, 0=Falling)

    TIMSK1 |= _BV(ICIE1); // Enable Timer 1 Input Capture Interrupt
    TIMSK1 |= _BV(TOIE1); // Enable Timer 1 Overflow Interrupt

    past_delta = 0;
    delta = 0;

    past_delta_sum = 0;
    delta_sum = 0;
    interrupts ();
}

float getRPM(){
    noInterrupts();
    
    float retVal;
    if(past_delta_sum == 0)
      retVal = 0;
    else
      retVal = 60.0 / ((past_delta_sum / 16.0) / 1000000);

    interrupts();

    return retVal;
}


void setup(){
    Serial.begin(9600);
    setupRPMTimer();
}

void loop(){       
    Serial.println(getRPM(), 2);
    //delay(1000);  // Slow down output
}