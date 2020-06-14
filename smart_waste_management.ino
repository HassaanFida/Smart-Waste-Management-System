#include <LiquidCrystal.h>
#include<Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial sim(A1,A2);  //tx, rx

String number = "+8801521109365"; // +880 is the country code

Servo myservo;

int sonar(int trig, int echo)
{
  int dis, t;
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  t=pulseIn(echo, HIGH);
  dis=(int)(t*(0.034/2));
  return dis;
}

int ot=9, oe=10, it=7, ie=8;
int d7=2, d6=3, d5=4, d4=5, e=11, rs=12;
int sp=6;

LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

void setup() {
  myservo.attach(sp);
  lcd.begin(16, 2);
  pinMode(ot, OUTPUT);
  pinMode(oe, INPUT);
  pinMode(it, OUTPUT);
  pinMode(ie, INPUT);
  Serial.begin(9600);  
  sim.begin(9600);
}

int state = 0,timer = 0, state_motor = 2;
void loop() {
  int id, od;
  od=sonar(ot, oe);
  id=sonar(it, ie);
 
  float per=(100-(100/24)*id);
  
  if(per<100){
    timer = 0;
    state = 0;
    if(od<30){
      if(state_motor != 0) {
        myservo.write(0);
        state_motor = 0;
      }
    }
    else{
      if(state_motor != 1) {
        myservo.write(180);
        state_motor = 1;
      }
    }
  }
  else {
    if(state_motor != 1) {
        myservo.write(180);
        state_motor = 1;
    }
    if(timer < 3) {
      timer++;
    }
    else{
      timer = 0;
      if(state == 0){
       SendMessage();
       state=1;
       //Serial.println("Yahoo");
      }
    }  
  }
  
  lcd.clear();

  if(per>100) per=100;
  lcd.print(per);
  lcd.print(" %");
  lcd.setCursor(0,1);
  lcd.print("filled up");
  delay(1000);
}


void SendMessage()
{
  Serial.println("asdf");
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000);
  String SMS = "Hi Minhaz,the garbage bin located at kuet EEE building has been filled up.Please collect the waste.";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);
  delay(1000);
}
