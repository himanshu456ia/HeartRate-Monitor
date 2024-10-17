#include <LiquidCrystal.h>
#include <TimerOne.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

int HBSensor = 4;
int HBCount = 0;
int HBCheck = 0;
int TimeinSec = 0;
int HBperMin = 0;
int HBStart = 2;
int HBStartCheck = 0;

void setup() {

  //LCD is initialized with the number of columns and rows (20x4).
  lcd.begin(20, 4);
  //sensor and button pins are configured as input pins.
  pinMode(HBSensor, INPUT);
  pinMode(HBStart, INPUT_PULLUP);
  // timer is initialized using TimerOne library to generate an interrupt every 0.8 seconds
  Timer1.initialize(800000); 
  Timer1.attachInterrupt( timerIsr );
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HR currently : ");
  lcd.setCursor(0,1);
  lcd.print("Time(s): ");
  lcd.setCursor(0,2);
  lcd.print("HR per minute : 0.0");
}

void loop() {
  //It checks if a button (connected to HBStart) is pressed. If the button is pressed, it sets HBStartCheck to 1, indicating that heartbeats should be counted.
  if(digitalRead(HBStart) == LOW){
    lcd.setCursor(0,3);
    lcd.print("Wait! Calculating HR");
    HBStartCheck = 1;
  }
  if(HBStartCheck == 1)
  {
    //If HBStartCheck is 1 (indicating that heartbeat counting is active), the code monitors the heart rate sensor.
      if((digitalRead(HBSensor) == HIGH) && (HBCheck == 0))
      {
        //When a heartbeat is detected, the HBCount is incremented, and the LCD is updated to display the heartbeat count.
        
        HBCount = HBCount + 1;
        HBCheck = 1;
        lcd.setCursor(14,0);
        lcd.print(HBCount);
        lcd.print(" ");
      }
      if((digitalRead(HBSensor) == LOW) && (HBCheck == 1))
      {
        HBCheck = 0;   
      }
        //The code also keeps track of the time in seconds using TimeinSec.
        //If TimeinSec reaches 10 seconds, it calculates the heart rate per minute, displays it on the LCD, and resets the heartbeat count and time.
      if(TimeinSec == 10)
      {
          HBperMin = HBCount * 6;
          HBStartCheck = 0;
          lcd.setCursor(14,2);
          lcd.print(HBperMin);
          lcd.print(" ");
          lcd.setCursor(0,3);
          lcd.print("Press Button again.");
          HBCount = 0;
          TimeinSec = 0;      
      }
  }
}


 //The timerIsr function is an interrupt service routine that gets called when the timer expires. It is used to update the time in seconds when HBStartCheck is 1.

void timerIsr()
{
  if(HBStartCheck == 1)
  {
      TimeinSec = TimeinSec + 1;
      lcd.setCursor(14,1);
      lcd.print(TimeinSec);
      lcd.print(" ");
  }
}
