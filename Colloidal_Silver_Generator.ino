  const int switchPin = 4;     // Relay Control Pin
  const int testPin = 3;       // Test pin, bring high to switch relay
  const int rLedPin = 10;      // Red LED
  const int gLedPin = 11;      // Green LED
  const int potPin = 5;        // Potentiometer for fan speed control
  const int MOSFETpin = 9;     //Fan PWM Output Pin
  boolean flip = false;
  boolean cooking = true;
  boolean flash = false;

  int pwm;
  int avg = 0;
  long switchingPeriod = 300000; //(ms) AKA 5 min
  long totalCookingTime = 10800000; // 3 hour cook time
  long fanRDelay = 100; // update fan speed every 100ms
  long blinkPeriod = 1000; //Operation LEDs blink every 1s
  // Overall Times
  long startTime = 0;
  long timeElapsed = 0;
  // Polarity Switching Times
  long timeOfLastFlip = 0;
  long timeSinceFlip = 0;
  // Fan Read Times
  long timeSinceFanR = 0;
  long timeOfLastFanR = 0;
  // LED Blink Times
  long timeSinceBlink = 0;
  long timeOfBlink = 0;  
  
  void setup() {

    // set all the other pins you're using as outputs:
    pinMode(switchPin, OUTPUT); 
    pinMode(gLedPin, OUTPUT);
    pinMode(rLedPin, OUTPUT);    
    pinMode(testPin, INPUT);

    // blink the LED 3 times. This should happen only once.
    // if you see the LED blink three times, it means that the module
    // reset itself,. probably because the motor caused a brownout
    // or a short.
    blink(gLedPin, 3, 100);
    blink(switchPin, 1, 500); //Test relay tripping
    digitalWrite(switchPin, flip);   // flip output
    startTime = millis();
    timeOfLastFlip = startTime;
    // Initialize fan speed
    pwm = map(analogRead(potPin),0,1023,0,255);     
    timeOfLastFanR = startTime;
    // Initialize Blink Time
    timeOfBlink = startTime;
  }

  void loop() {
    // Determines when to flip the polarity of the relay
    timeSinceFlip = millis() - timeOfLastFlip;
    if ((timeSinceFlip >= switchingPeriod)||(testPin == HIGH)){
        flip = !flip;
        digitalWrite(switchPin, flip);
        timeOfLastFlip = millis();
    }
    // Determines when to update the fan speed from to POT
    timeSinceFanR = millis() - timeOfLastFanR;
    if (timeSinceFanR >= fanRDelay){
      avg += analogRead(potPin);
      avg = avg / 2;
      pwm = map(avg,0,1023,0,255);    
      analogWrite(MOSFETpin, pwm);
      timeOfLastFanR = millis();
    }
    
    //Determines when to flash the operation LEDs
    timeSinceBlink = millis() - timeOfBlink;
    if (timeSinceBlink >= blinkPeriod){
      if(cooking){
          if(flip){
            blink(gLedPin, 1, 100);
          }else{
            blink(rLedPin, 1, 100);
          } 
      }else{
        digitalWrite(gLedPin, flash);
        digitalWrite(rLedPin, !flash);
        flash = !flash;
      }
      timeOfBlink = millis();      
    }
    
    // Determines when cooking time is over
    timeElapsed = millis() - startTime;    
    if(timeElapsed >= totalCookingTime){
      cooking = false;
      blinkPeriod = 500;
    }
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
