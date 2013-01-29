#include <LiquidCrystal.h>

//ultrasonic sound
const int triggerPin = 13;
const int echoPin = 12;

const int box_empty_level = 60;
const int box_full_level = 45;
long fuel_level = 0;

const int EMPTY = 1;
const int FULL = 0;
int last_major_state = EMPTY;


const int ON_BUTTON = 2;
const int OFF_BUTTON = 3;
const int VAC_OFF = 0;
const int VAC_ON = 1;
const int CYCLON_FULL = 400;

int vac_state = VAC_OFF;
int photoResistorValue = 0;

const int lightPin = 0; 
const int lcd_rs = 10;
const int lcd_enable =9;
const int lcd_d4 = 4;
const int lcd_d5 = 5;
const int lcd_d6 = 6;
const int lcd_d7 = 7;
const int lcd_columns =16;
const int lcd_row = 2;

//Light control
const int light = 11;

LiquidCrystal lcd(lcd_rs, lcd_enable, lcd_d4, lcd_d5, lcd_d6, lcd_d7);

void setup() {
  // initialize serial communication
  // Serial.begin(9600);

  pinMode(triggerPin, OUTPUT);
  pinMode(ON_BUTTON, OUTPUT);
  pinMode(OFF_BUTTON, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(light, OUTPUT);

  lcd.begin(lcd_columns, lcd_row);
}

void loop()
{

  fuel_level = get_fuel_level();
  photoResistorValue = analogRead(lightPin);

  lcd.setCursor(0, 0);
  lcd.print("R:");
  lcd.print(photoResistorValue);

  lcd.setCursor(0, 1);
  lcd.print("L:      ");
  lcd.setCursor(2, 1);
  lcd.print(fuel_level);

  //Serial.print(fuel_level);
  //Serial.print("fuel_level");
  //Serial.println();

  lcd.setCursor(8, 0);
  lcd.print("S1:     ");
  lcd.setCursor(11, 0);

  if (box_is_empty()){
    lcd.print("Empty");
     turn_light_on();
    last_major_state = EMPTY;

  }
  else if(box_is_full()){
    lcd.print("Full");
    last_major_state = FULL;  
    turn_vac_off();//Just to be sure.
    turn_light_off();
  }
  else{
    lcd.print("Normal");
  }
  
  lcd.setCursor(8, 1);
  lcd.print("Vac:     ");
  lcd.setCursor(12, 1);

  if (last_major_state == EMPTY){
    if(cyclon_is_full(photoResistorValue)){
      lcd.print("OFF");
      turn_vac_off();
    }
    else{ 
      turn_vac_on();
      lcd.print("ON");
    }
  }
  delay(1000);
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}

long get_fuel_level(){
  long duration;

  //sending the signal, starting with LOW for a clean signal
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(triggerPin, LOW);

  //setting up the input pin, and receiving the duration in
  //microseconds for the sound to bounce off the object infront

  duration = pulseIn(echoPin, HIGH);

  return microsecondsToCentimeters(duration);
}

boolean box_is_empty(){
  return fuel_level > box_empty_level;
}

boolean box_is_full(){
  return fuel_level < box_full_level;
}

boolean cyclon_is_full(int resistor_value){
  return resistor_value > CYCLON_FULL; 
}

void turn_vac_on(){
  if(vac_is_off()){
    digitalWrite(ON_BUTTON, HIGH);
    delay(500);
    digitalWrite(ON_BUTTON, LOW);
    vac_state = VAC_ON;
  }
}

void turn_vac_off(){
  if(vac_is_on()){
    digitalWrite(OFF_BUTTON, HIGH);
    delay(500);
    digitalWrite(OFF_BUTTON, LOW);
    vac_state = VAC_OFF;
  }
}

boolean vac_is_on(){
  return vac_state == VAC_ON;
}

boolean vac_is_off(){
  return vac_state == VAC_OFF;
}

void turn_light_on()
{
  digitalWrite(light, HIGH);
}

void turn_light_off(){
  digitalWrite(light, LOW);
}


