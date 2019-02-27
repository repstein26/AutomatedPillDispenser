// Include the Servo library
#include <Servo.h>
#include <LiquidCrystal.h>
#include <DS3231_Simple.h>

// The clock library
DS3231_Simple Clock;

// the servo motors
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// the servo servo pins here
int servoPin1 = 22;
int servoPin2 = 24;
int servoPin3 = 26;
int servoPin4 = 29;
int servoPin5 = 30;

// the button pins
int incrementButtonPins[5] = {3,4,5,6,7};     // the increment button pins
int dispenseButtonPin = 9;               // this is the dispense pin
int buttonPin7 = 0;
int home_button = 8;

int hourPin = 10;
int minutePin = 8;

//buzzer
const int buzzer = 44;
// lastStates
boolean lastState[5] = {LOW, LOW, LOW, LOW, LOW};
boolean dLastState = LOW;
boolean currentState = LOW;
boolean minLastState = LOW;
boolean hourLastState = LOW;

LiquidCrystal lcd(32, 34, 42, 40, 38, 36);

int numOfPills[5] = {0, 0, 0, 0, 0};
int numOfFunnels = 5;

void setup() {
  // Set up the clock, time should already be set. The initial alarm is for
  // a minute after the current time.
  Serial.begin(9600);
  Clock.begin();
  //Clock.promptForTimeAndDate(Serial);

  Clock.disableAlarms();
  DateTime MyTimestamp = Clock.read();
  MyTimestamp.Minute = MyTimestamp.Minute + 1;
  Serial.print("alarm is: " + String(MyTimestamp.Minute));

  Clock.setAlarm(MyTimestamp, DS3231_Simple::ALARM_MATCH_MINUTE);

  pinMode(buzzer, OUTPUT);
  
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);
  servo4.attach(servoPin4);
  servo5.attach(servoPin5);

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  servo4.write(0);
  servo5.write(0);

  // SETUP all button pins to be inputs
  for(int i = 0; i < numOfFunnels; i++){
    pinMode(incrementButtonPins[i], INPUT);
  }
  //Serial.begin(9600);//initialize Serial connection
  lcd.begin(14, 2);
  lcd.print("Set # of pills:");
}

void loop() {      
  
  // Loop through all 5 button and check if they were pressed
  checkIfIncremented();
  if(dispenseButtonPressed(dispenseButtonPin)){
      dispense();
      Serial.println("dispensed");
  }
  uint8_t AlarmsFired = Clock.checkAlarms();
  //Clock.printTimeTo_HMS(Serial);
  //`Serial.println();
  //delay(1000);

  if(AlarmsFired & 2){
    dispense();
    Serial.println("ALARM!!!");
  }
/*  if (homeButtonPressed(home_button)){
     lcd.setCursor(0, 1);
     lcd.print("Settings saved");
     delay(3000);
     lcd.setCursor(0, 1);
     lcd.print("              ");
     lcd.setCursor(0, 1);
  }*/
  /*
  // Check if the dispensed button was pressed
  currentState2 = digitalRead(dispenseButtonPin);
  if (currentState2 == HIGH && lastState2 == LOW) {
    dispense();
    Serial.println("dispensed");
  }

  lastState2 = currentState2;*/
}

void dispense() {
  int counter = 0;
  int i = 0;
  
    while (counter < numOfPills[i]) {
      if (servo1.read() == 0) {
        servo1.write(180);
        delay(1000);
      } else {
        servo1.write(0);
        delay(1000);
      }
      counter++;
    }
    counter = 0;
    i++;

    while (counter < numOfPills[i]){
      if (servo2.read() == 0) {
        servo2.write(180);
        delay(1000);
      } else {
        servo2.write(0);
        delay(1000);
      }
      counter++;
    }
    i++;
    counter = 0;

    while (counter < numOfPills[i]){
      if (servo3.read() == 0) {
        servo3.write(180);
        delay(1000);
      } else {
        servo3.write(0);
        delay(1000);
      }
      counter++;
    }
    i++;
    counter = 0;

    while (counter < numOfPills[i]) {
      Serial.print("im here " + String(i));
      if (servo4.read() == 0) {
        Serial.print("here");
        servo4.write(180);
        delay(1000);
      } else {
        servo4.write(0);
        delay(1000);
      }
      counter++;
    }
    i++;
   counter = 0;

    while (counter < numOfPills[i]) {
      if (servo5.read() == 0) {
        servo5.write(180);
        delay(1000);
      } else {
        servo5.write(0);
        delay(1000);
      }
      counter++;
    }
    tone(buzzer, 4000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);        // ...for 1sec

}

bool buttonPressed(int index){
  currentState = digitalRead(incrementButtonPins[index]);
  //Serial.println("currentState is: " + String(currentState) + "for button " + String(index+1));
  if(currentState && !lastState[index]){
      lastState[index] = currentState;
      return true;
  }
  lastState[index] = currentState;
  return false;
}

bool minutePressed(){
  currentState = digitalRead(minutePin);
  if(currentState && !minLastState){
    minLastState = currentState;
    return true;
  }
  minLastState = currentState;
  return false;
}

bool hourPressed(){
  currentState = digitalRead(hourPin);
  if(currentState && !hourLastState){
    hourLastState = currentState;
    return true;
  }
  hourLastState = currentState;
  return false;
}

/*
bool homeButtonPressed(int index){
  currentState = digitalRead(index);
  //Serial.println("currentState is: " + String(currentState) + "for button " + String(index+1));
  if(currentState && !lastState[index]){
      lastState[index] = currentState;
      return true;
  }
  lastState[index] = currentState;
  return false;
}*/

bool dispenseButtonPressed(int index){
  
   currentState = digitalRead(index);
  //Serial.println("currentState is: " + String(currentState) + "for button " + String(index+1));
  if(currentState && !lastState[index]){
      dLastState = currentState;
      return true;
  }
  dLastState = currentState;
  return false;
}

void checkIfIncremented() {
  for (int i = 0; i < numOfFunnels; i++) {
    if(buttonPressed(i)){
      //Serial.println("Button " + String(i+1) + " pressed.");
      if (numOfPills[i] < 9) {
        numOfPills[i] = numOfPills[i] + 1;
      } else {
        numOfPills[i] = 0;
      }
      lcd.setCursor(0, 1);
      lcd.print("#" + String(i+1) + " Amount: " + String(numOfPills[i]));
    }
    /*
    currentState = digitalRead(incrementButtonPins[i]);
    if (currentState == HIGH && lastState[i] == LOW) {
      Serial.println("Button " + String(i+1) + " pressed.");
      if (numOfPills[i] < 9) {
        numOfPills[i] = numOfPills[i] + 1;
      } else {
        numOfPills[i] = 0;
      }
      lcd.print("#" + String(i+1) + " Amount: " + String(numOfPills[i]));
    }*/

    delay(10);
    //lcd.setCursor(0, 1);
    //lcd.print("Pill " + String(i+1) + "amount " + String(numOfPills[0]));
    lastState[i] = currentState;
  }
}