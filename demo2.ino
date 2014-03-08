#include<LiquidCrystal.h>
#include<NewPing.h>
int distance;
int temp = 16;
unsigned int stage = 0;
unsigned char pattern[7][8] = {
  {0x0e,0x1b,0x11,0x11,0x11,0x11,0x11,0x1f},
  {0x0e,0x1b,0x11,0x11,0x11,0x11,0x1f,0x1f},
  {0x0e,0x1b,0x11,0x11,0x11,0x1f,0x1f,0x1f},
  {0x0e,0x1b,0x11,0x11,0x1f,0x1f,0x1f,0x1f},
  {0x0e,0x1b,0x11,0x1f,0x1f,0x1f,0x1f,0x1f},
  {0x0e,0x1b,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f},
  {0x0e,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f}
};

unsigned char blank[8] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

LiquidCrystal lcd(42,40,38,36,34,32,30,28,26,24,22);
NewPing sonar(12,11,200);



void setup() {
  Serial.begin(9600);
  for(int i = 0; i < 7; i++){
    lcd.createChar(i,pattern[i]);
  }
  lcd.createChar(7,blank);
  lcd.begin(16,2);
  lcd.setCursor(0,1);
  for(int i = 0; i < 16; i++){
    lcd.write(0xff);
  }
  lcd.setCursor(0,0);
}

void loop() {
  /*lcd.print(sonar.ping_median(10)/US_ROUNDTRIP_CM);
  lcd.setCursor(0,1);
  lcd.print(sonar.ping_cm());
  delay(500);
  lcd.clear();
*/
  distance = sonar.ping_median(10)/US_ROUNDTRIP_CM;
  lcd.print("Distance:");
  lcd.print(distance);
  lcd.print("cm");
  lcd.setCursor(15,0);
  lcd.write(byte(stage%7));
  lcd.setCursor(0,1);
  if((distance) < temp){
    lcd.setCursor(distance,1);
    for(int j = (distance); j < min(temp,16); j++){    
      lcd.write(byte(7));
    }
  }
  else if((distance) >= temp){
    lcd.setCursor(temp,1);
    for(int j = temp; j < min(distance,16); j++){
      lcd.write(0xff);
    }
  }
  Serial.print("distance:");
  Serial.println(distance);
  Serial.print("temp:");
  Serial.println(temp);
  temp = distance;
  delay(500);
  lcd.setCursor(9,0);
  lcd.write(byte(7));
  lcd.write(byte(7));
  lcd.write(byte(7));
  lcd.write(byte(7));
  lcd.write(byte(7));
  lcd.setCursor(0,0);
  stage = stage + 1;
  
}
