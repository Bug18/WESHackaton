#include <Arduino.h>
#include "RPMHandler.h"
#include "BregHandler.h"
#include "BobHandler.h"


uint32_t past_delta;
uint32_t delta;

uint32_t past_delta_sum;
uint32_t delta_sum;

#define ZUBAC_ANGLE 15.0
#define ZUBAC1_ANGLE 195.0
#define INC_DELAY 124

int zubac_count = 0;
bool is_toothless = false;
bool is_on_first_rotation = false;
int treashold = 0;
bool firstBreg = true;
float precalculatedDelay;

volatile uint16_t timer_overflow = 0;

void chargeDischargeSparkPlug() {
    delayMicroseconds(precalculatedDelay);
	digitalWrite(B_OUT, HIGH);
	delayMicroseconds(1360);
	digitalWrite(B_OUT, LOW);
} 

// neven funckije
float timeFromAngle(float angle) {
    return angle / (ZUBAC_ANGLE / (((float)past_delta_sum / 24) / 16));
}

float angleFromTime(float time) {
    return time * (ZUBAC_ANGLE / (((float)past_delta_sum / 24) / 16));
}

// end neven funckije
float calculateSpark(){
	noInterrupts();
	
	int _zubac_count = zubac_count;
    interrupts();
	// uint32_t _past_delta = past_delta / 16.0; // number of time that the timer register overflowed
	
	unsigned int _delay;

    float current_rpm = getRPM();

    int value = analogRead(pinMAP);
    float angle = getAngle(fmap((float)value, 102, 920, 15, 100), current_rpm);

    float tp = timeFromAngle(angle);
    float perAngle = angleFromTime(1500.0f + tp);
    float beta = 360 - ZUBAC1_ANGLE - perAngle;
    _delay = timeFromAngle(beta);
    // Serial.println(_delay);

    //  = (360 - angle - (1500 * 15.0 / _past_delta) - current_position) * _past_delta / 15.0;

    // float micro_spr_360 = 60.0 / current_rpm * 1000000.0 / 360;
    // _delay = (360 - angle - 1.0 / micro_spr_360 * 1500 - current_position) * micro_spr_360;


	// delayMicroseconds(_delay - INC_DELAY);  // 1.5ms corrected for instruction
	

    return _delay;
}

ISR(TIMER1_OVF_vect)
{
  timer_overflow++;
}

ISR(TIMER1_CAPT_vect){
    static bool firstRisingPassed = false;
    static uint32_t last_time_read = 0;

    uint16_t _timer_overflow = timer_overflow;

    // If an overflow happened but has not been handled yet
    // and the timer count was close to zero, count the
    // overflow as part of this time.
    if ((TIFR1 & _BV(TOV1)) && (ICR1 < 1024))
        _timer_overflow++;
    
    if(firstRisingPassed){
        past_delta = delta;
        
        uint32_t current_time = _timer_overflow;
        current_time = (current_time << 16) | ICR1;

        delta = current_time - last_time_read;

        if(past_delta * 2 < delta){       //end of cycle
            past_delta_sum = delta_sum + delta;
            
            delta_sum = 0;                  // NGL fix
            past_delta = 0;

            is_toothless = true;
        } else {
            delta_sum += delta;
        }
    }
    
    last_time_read = _timer_overflow;
    last_time_read = (last_time_read << 16) | ICR1;
    
    firstRisingPassed = true;

    if(bregChecked){  // provijeri je li proso kalibracijski brijeg
        if(is_toothless){  // provijeri je li trenutni uzlazni brid dolazi nakon rupe koljenstog vratila
            zubac_count += 3;
            zubac_count = zubac_count % 24;
            
			is_on_first_rotation = !is_on_first_rotation;
            treashold++;

            if(treashold > 4) bregExists = false;
        
        } else {
            zubac_count += 1;
        }

        is_toothless = false;


        if(zubac_count == 1){
            if(is_on_first_rotation || !bregExists){
                if(firstBreg) precalculatedDelay = calculateSpark();
                chargeDischargeSparkPlug();
            }
            firstBreg = false;
        }
        if(zubac_count == 22){
            if(is_on_first_rotation || !bregExists){
                if(!firstBreg) calculateSpark();
            } 
        }
        
    }
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
    
    float return_value;
    if(past_delta_sum == 0)
        return_value = 0;
    else
        return_value = 60.0 / ((past_delta * 24.0  / 16.0) / 1000000);

    interrupts();

    return return_value;
}