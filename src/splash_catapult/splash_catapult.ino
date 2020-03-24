#include <Servo.h> // twelve servo objects can be created on most boards

// *************************** 0317 note ******************************
// The hardware strucutre still need more robust mechanism 
// - tangle structure (tmore test on elastic band and spring combination)
// - weight loading (need more robust container)
// *************************** 0317 note ******************************

// *************************** 0323 note ******************************
// Hardware improvement:
// - The band can be extended more to provide better elasticity
// - Change the fulcrum (support point) structure and also increase the moment arm
// - Test heavier and bigger object
// *************************** 0323 note ******************************


// psuh button as the catapult trigger
const int button_pin = 2;
int buttonState = 0;
int lastState = 0;

// 180 degree servo as the locker (single actuator for damping)
Servo locker;
const int locker_pin = 6;
int locker_pos = 0; // init
int locker_final_angle = 90;
int locker_delay = 5;

// 360 degree servo as the rotator (single actuator for contracting and releasing the spring)
Servo rotator;  // contract the spring or band
const int rotator_pin = 9;
int rotator_pos = 0; // init
// rotation_speed (slower when approach)
int rotator_counter_clockwise_speed = 20;
int rotator_clockwise_speed = 160;
int rotator_duration = 3800;
int rotator_stop = 90;

// // reset function for rotator (spin small angle)
//  rotator.write(160);
//  delay(200);
//  rotator.write(90);

void setup() {
  Serial.begin(9600); // initialize serial communication at 9600 bits per second:
  pinMode(button_pin, INPUT);  // make the button's pin an input  
  rotator.attach(rotator_pin); // make the rotator's pin an output 
  locker.attach(locker_pin);   // make the locker's pin an output
  locker.write(locker_pos); 
}

void loop() {

    // press button to trigger
      int buttonState = digitalRead(button_pin);
      Serial.println(buttonState);

      if (buttonState != lastState) { // check the state change
          if (buttonState == HIGH) { // check if triggered

            // lock the catapult (counter-clockwise spin locker)
              Serial.println("locker lock!");
              for (locker_pos = 0; locker_pos <= locker_final_angle; locker_pos +=1) {
                locker.write(locker_pos);
                delay(locker_delay);
              }
            
            // contract spring (counter-clockwise spin rotator)
              Serial.println("rotator contract!");
              rotator.write(rotator_counter_clockwise_speed); // counter-clockwise spin
              delay(rotator_duration);
              rotator.write(rotator_stop); // stop
            
            // unlock the catapult (clockwise spin locker)
              Serial.println("locker unlock!");
              for (locker_pos=locker_final_angle; locker_pos >= 0; locker_pos -=1){
                locker.write(locker_pos);
                delay(locker_delay);
              }
            
            // splash water (delay X seconds calculated by formula after unlock catapult)
            // ...
                  
            
            // release spring (clockwise spin rotator)
              Serial.println("rotator release!");
              rotator.write(rotator_clockwise_speed); // clockwise spin
              delay(rotator_duration + 300);
              rotator.write(rotator_stop); // stop
          }
      }
      
      delay(50);  // delay in between reads for stability
      lastState = buttonState;
}
