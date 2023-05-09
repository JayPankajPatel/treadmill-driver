#include <LiquidCrystal_I2C.h>

#define ENABLE 2
#define RAISE 3
#define LOWER 4
#define BUTTON 5
#define T3 7

enum Direction {UP, DOWN};

int buttonState;
int direction = UP;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void displayIncline(int ADCReading) {
  // The user manual says that incline is between 0% and 15%
  long percentage = map(ADCReading, 156, 796, 0, 15);

  lcd.setCursor(0, 0);
  lcd.print(percentage);
  lcd.print("%");
  // Make the trailing % sign of 10% disappear if we drop from 10% to 9%
  if (percentage < 10) lcd.print(" ");
}

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(ENABLE, OUTPUT);
  pinMode(RAISE,  OUTPUT);
  pinMode(LOWER,  OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(T3,     OUTPUT);

  digitalWrite(T3, HIGH);
  digitalWrite(ENABLE, HIGH);

  buttonState = digitalRead(BUTTON);
}

void loop() {
  displayIncline(analogRead(A1));

  int newState1 = digitalRead(BUTTON);
  if (newState1 == buttonState) return; // No change

  delay(10);

  int newState2 = digitalRead(BUTTON);
  if (newState2 != newState1) return; // Just a flutter, not a proper press

  buttonState = newState2;

  if (buttonState) {
    digitalWrite(direction == UP ? RAISE : LOWER, HIGH);
  } else {
    digitalWrite(direction == UP ? RAISE : LOWER, LOW);
    direction = direction == UP ? DOWN : UP;
  }
}