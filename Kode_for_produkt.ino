#include "pitches.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(2,3,4,5,6,7);
int ledBlue = 13;
int lightPin = 4;
int resetBut = 12;

// Button-style counting of light 
boolean lastAdd = LOW;
boolean currentAdd = LOW;

unsigned long timeSinceMovementSensored = 0;

// Found 100+ to be normal lighting conditions
int points = 0;
int last_points = 0;
boolean start = true;

void setup() {
  lcd.begin(16, 2);
  
  pinMode(ledBlue, OUTPUT);
  
  // For the light sensor
  Serial.begin(9600);
}

// Normal button debounce
// Currently not in use
boolean debounce(boolean last, int but){
  boolean current = digitalRead(but);
  if(last != current){
    delay(5);
    current = digitalRead(but);
  }
  return current;
}

// Custom debounce for light sensor
boolean debounceLight(boolean last, int light){
  // If the resistance is less than the int value, register a "button" press
  boolean current = 200 > (analogRead(light));
  if(last != current){
    delay(5);
    current = 200 > (analogRead(light));
  }
  return current;
}

// A fast scale for when the program starts
void playSoundStart(){
  tone(8, NOTE_C4, 50);
  delay(20);
  tone(8, NOTE_D4, 50);
  delay(20);
  tone(8, NOTE_E4, 50);
  delay(20);
  tone(8, NOTE_F4, 50);
  delay(20);
  tone(8, NOTE_G4, 50);
  delay(20);
  tone(8, NOTE_A4, 50);
  delay(20);
  tone(8, NOTE_B4, 50);
  delay(20);
  tone(8, NOTE_C5, 50);
  delay(20);
}


void printScreen(){
    lcd.clear();
  
  // animation of points with sound
  if(points != last_points){
    for(int i=16; i>=1; i--){
      lcd.setCursor(i,0);
      lcd.print(points);
      delay(40);
      
      lcd.setCursor(i-1,0);     
      lcd.print(points);
      
      delay(40);
    // falling sound effect
      tone(8, 362 + (i*263/32), 50);
      lcd.clear();
    }
  }
  
  lcd.print(points);
  lcd.setCursor(0,1);
  lcd.print("points");
  
  // 'new point is in place' sound effect
  if(points != last_points){
    delay(300);
    tone(8, NOTE_C4, 300);
    delay(100);
    tone(8, NOTE_E4, 300);
    delay(100);
    tone(8, NOTE_C5, 300);
  }
  
    delay(1500);
}


void loop(){
  
  // Plays startup sound
  if(start){
    playSoundStart();
    start = !start;
  }
  
  digitalWrite(ledBlue, HIGH);
  
  // Checks if the light has been blocked and adds point
  currentAdd = debounceLight(lastAdd, lightPin);
  if(currentAdd == HIGH && lastAdd == LOW){
    points++;
  last_points = points - 1;
  }
  else{
    last_points = points;
  }

  printScreen();
}
