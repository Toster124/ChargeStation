#include "LCD_1602_RUS.h"
#include "thermistorMinim.h"
thermistor therm(0, 10000, 3950);  // пин, сопротивление при 25 градусах (R термистора = R резистора!), бета-коэффициент
thermistor therm1(2, 10000, 3950);  // пин, сопротивление при 25 градусах (R термистора = R резистора!), бета-коэффициент
#include <GyverPWM.h>
LCD_1602_RUS lcd(0x27, 16, 2);
int value;
float Voltage = 0.0, c; // переменная для расчета напряжения
#define zeroamp 2.5
#define oneamp 2.350
int u = 0; // переменная для числа пребразования 0-1023
void setup()
{
  pinMode(9, OUTPUT);
  pinMode(A3, INPUT);
  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  PWM_frequency(9, 150000, FAST_PWM);
  //Serial.begin(9600);

}

void loop()
{

lcd.setCursor(0, 0);
lcd.print(L"М ");
  lcd.print(String(therm.getTempAverage()));
  lcd.print(L"  Д ");
  lcd.print(String(therm1.getTempAverage()));
  lcd.setCursor(0, 1);
  
  if (therm.getTempAverage() > therm1.getTempAverage()){
    setvalue(therm.getTempAverage());
  }
  if (therm1.getTempAverage() > therm.getTempAverage()){
    setvalue(therm1.getTempAverage());
  }
  lcd.print(L"К ");
  lcd.print(String(map(value, 0, 255, 0, 100)));
  lcd.print(L" %   ");
  String tok = String(current());
  Serial.println(tok);
  lcd.print(tok.substring(0, 3));
  lcd.print(L"A    ");
  Serial.println(value);
  delay(500);
}
void setvalue(float temp){
  Serial.print("1:");
    Serial.print(temp);
    Serial.println();
    if (temp >= 28) {
      if (value == 0){
        PWM_set(9, 240);
        delay(300);
      }
      value = map(temp, 28, 40, 60, 255);
      }
    if (temp < 25) {
      value = 0;
    }
    if (temp > 40) {
      value = 255;
    }
    PWM_set(9, value);
}
float current() {
  u = 0;
  for(int i = 0; i != 30; i++){
    u += analogRead(A3); // измерение 
  }
  u /= 30;
  Voltage = (u * 5.00) / 1024; // пересчет измерения в вольты
  c = abs((Voltage - zeroamp) / (oneamp - zeroamp));
  return(c);
}
