//Maanya Goenka
//Created with the construction and debugging assistance of Stephen Mohring, David Musicant and Danny Maya
//Foosball_Table 
//Final Project CS 232
//03/10/2020

#include "pitches.h" 
#include <NewPing.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> //library for lcd screen

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // setting the LCD I2C address

int ledArray[] = {7,8,9,10,11,12};
int ledArrayA[] = {7,8,9}; //controlling lights for Carleton team
int ledArrayB[] = {10,11,12}; //controlling lights for Olaf team

int pushButton = 2; //restart button
int buzzer = 45;
boolean state = false;
int countA = 0;
int countB = 0;


int winner[]= {NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4}; 
int duration[] = {4,8,8,4,8,8,4}; //array of duration of notes
int nonDottedNote = (60000 * 4) / 144; //determines speed of music
int duration1 = 0;

void setup() {
  for(int control=7;control<=12;control++){ //sets all led pins to output using a for loop
    pinMode(control, OUTPUT);
  }
  pinMode(5, OUTPUT);
  pinMode(46, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pushButton, INPUT);
  Serial.begin(9600);
  
  digitalWrite(5, 255); //turning on the LED light for compartment 1
  digitalWrite(46, 255); //turning on the LED light for compartment 2
  lcd.begin(16,2); //setting number of characters in each line followed by number of lines
  lcd.setCursor(0,0); 
  lcd.print("Welcome to CS232!"); 
  delay(3000);
  lcd.clear();
  lcd.print("Carleton vs Olaf");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press button to");
  lcd.setCursor(0,1);
  lcd.print("  start game");
  delay(2000);
}
void loop() {
  lcd.clear();
  if(digitalRead(pushButton) == 0){ //if button is pressed, servo releases the ball to the field
    state = true;  
    lcd.setCursor(0,0); //displaying initial scores
    lcd.print("Olaf: 0");
    lcd.setCursor(0,1);
    lcd.print("Carleton: 0");
    delay(2000);
  }
  while(state == true){
    while(countA<3 && countB<3){ //while no one has won the foosball game
      if(analogRead(A5)< 300){ //implies that the ball is blocking the light beam from the LED
        analogWrite(ledArrayB[countA], 255); //lights up led to show score aggregates
        countA++; //counting Olaf's score
        lcd.clear();
        lcd.print("   Go Oles!");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Olaf: ");
        lcd.setCursor(6,0);
        lcd.print(countA);
        lcd.setCursor(0,1);
        lcd.print("Carleton: ");
        lcd.setCursor(10,1);
        lcd.print(countB);
        for (int note=0; note < 7; note++) {
        int note_type = duration[note];
        if (note_type > 0) {
          duration1 = nonDottedNote/note_type;  //duration for non-dotted (regular notes) with positive integers in the array
        } 
        else if (note_type < 0) {
          duration1 = nonDottedNote/abs(note_type); //duration for dotted notes with negative integers in the array
          duration1 *= 1.5; 
        }
        tone(buzzer, winner[note], duration1*0.9);
        delay(duration1*1.12);
      }
        if(countA<3){
          delay(7000);
        }    
        else{
          delay(3000);     
        }
      }
      if(analogRead(A3)<180){ //implies that the ball is blocking the light beam from the LED
        analogWrite(ledArrayA[countB], 255); //lights up led to show score aggregates
        countB++; //counting Carleton's score
        lcd.clear();
        lcd.print("   Go Oles!");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Olaf: ");
        lcd.setCursor(6,0);
        lcd.print(countA);
        lcd.setCursor(0,1);
        lcd.print("Carleton: ");
        lcd.setCursor(10,1);
        lcd.print(countB);
        for (int note=0; note < 7; note++) {
        int note_type = duration[note];
        if (note_type > 0) {
          duration1 = nonDottedNote/note_type;  //duration for non-dotted (regular notes) with positive integers in the array
        } 
        else if (note_type < 0) {
          duration1 = nonDottedNote/abs(note_type); //duration for dotted notes with negative integers in the array
          duration1 *= 1.5; 
        }
        tone(buzzer, winner[note], duration1*0.9);  //10% of the note duration for when it pauses and no tone is emitted
        delay(duration1*1.12);
      }
        if(countB<3){
          delay(7000);
        }    
        else{
          delay(1000);     
        }
      }
      if(countA > countB && countA!=3){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  Carls gotta "); 
        lcd.setCursor(0,1);
        lcd.print("   catch up   ");
        delay(2000);
      }
      else if(countB > countA && countB!=3){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  Oles, you're "); 
        lcd.setCursor(0,1);
        lcd.print("falling behind");
        delay(2000);
      }
    }
    lcd.clear();
    if(countA==3||countB==3){ //when a player makes three points
      if(countB==3){ //displaying winning message on lcd screen
        lcd.print("  Knights win!");
        delay(3000);
      }
      else if(countA==3){
        lcd.print("   Oles win!");
        delay(3000);
      }
      lcd.clear();
      lcd.print("   Game Over");
      delay(3000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Press button to");
      lcd.setCursor(0,1);
      lcd.print("  play again");
      delay(2000);
      for(int control=7;control<=12;control++){ //turning all lights to off condition
        analogWrite(control, 0);
      }
      countA = 0;
      countB = 0;
    }
    if(digitalRead(pushButton)==1){ //situation when button is not pressed
      state = false; //switches state to default value 
    }
  }
}
