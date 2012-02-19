  const int switchPin = 4;    // Relay Pin
  const int testPin = 3;      // Test pin, bring high to switch relay
  const int ledPin = 13;      // LED
  const int potPin = 5;
  const int MOSFETpin = 9;    //Fan Pin
  boolean flip = false;

  int pwm;
  int avg;
  long delaylen = 300000; //(ms) AKA 5 min
  void setup() {

    // set all the other pins you're using as outputs:
    pinMode(switchPin, OUTPUT); 
    pinMode(ledPin, OUTPUT);
    pinMode(testPin, INPUT);

    // blink the LED 3 times. This should happen only once.
    // if you see the LED blink three times, it means that the module
    // reset itself,. probably because the motor caused a brownout
    // or a short.
    blink(ledPin, 3, 100);

    digitalWrite(switchPin, flip);   // flip output
    
  }

  void loop() {
    avg = 0;
    for (long x = 0; x < ((delaylen - 100)/10); x++){
      avg += analogRead(potPin);
      avg = avg / 2;
      pwm = map(avg,0,1023,0,255);    
      analogWrite(MOSFETpin, pwm);
      if (testPin == HIGH){
        digitalWrite(switchPin, flip);
        flip = !flip;
        blink(ledPin, 1, 250);
      }
      delay(10);
    }
    
    flip = !flip;   
    digitalWrite(switchPin, flip);   // flip output
    blink(ledPin, 1, 100);
  }
  
  void blink(int whatPin, int howManyTimes, int milliSecs) {
    int i = 0;
    for ( i = 0; i < howManyTimes; i++) {
      digitalWrite(whatPin, HIGH);
      delay(milliSecs/2);
      digitalWrite(whatPin, LOW);
      delay(milliSecs/2);
    }
  }
