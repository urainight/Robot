#include <Servo.h>
#include <ArduinoSort.h>
#include <math.h>
int a1=32.82;
int a2=143.5;
int a3=125.25;
int a4=47+5+3.8;
int shape=0;
int c=0;
String a,receive;
int R_EN = 53; 
int L_EN = 51;
int R_PWM = 2;
int L_PWM = 3;
Servo servo4; 
Servo servo5; 
#define dirPin1 5
#define stepPin1 4
#define dirPin2 7
#define stepPin2 6
#define dirPin3 9
#define stepPin3 8
#define run_btn 43 //run button
#define stop_btn 45 //stop button
#define run_led 11
#define stop_led 10
float pz,angle1,px,py;
float angle2,angle3,angle4;
float Cos3,Sin3,Cos2,Sin2,Cos4,Sin4;
int z;
int x;
int y; 
float val=180/PI;
void ikinematic(float y, float x,float z)
{
  pz=z-1*a4;
  angle1=atan2(y,x)*val;
 
  px=cos(angle1)*x+sin(angle1)*y-a1-0*a4;
  Cos3=(sq(px)+sq(pz)-sq(a2)-sq(a3))/(2*a2*a3);
  Sin3=sqrt(1-sq(Cos3));
  angle3=atan2(Sin3,Cos3)*val;
  Cos2=((a2+a3*Cos3)*pz-(Sin3*a3*px))/(sq(a2+a3*Cos3)+sq(Sin3*a3));
  Sin2=sqrt(1-sq(Cos2));
  angle2=atan2(Sin2,Cos2)*val;
 
  angle4=90-(angle3+angle2);
}
bool start = false;
void run_led_trigger(){
  digitalWrite(run_led, HIGH);
  delay(10);
  digitalWrite(stop_led, HIGH);
  delay(10);
 }
 void stop_led_trigger(){
 digitalWrite(run_led, LOW);
 delay(10);
 digitalWrite(stop_led, LOW);
 delay(10);
 
 }
void start_trigger() { // toggle on/off state of the system
 start = true;
}
void stop_trigger() { // toggle on/off state of the system
 start = false;
}
void setup() 
{
 
 // Sets the two pins as Outputs
 pinMode(stepPin3, OUTPUT);
 pinMode(dirPin3, OUTPUT);
 pinMode(stepPin1, OUTPUT);
 pinMode(dirPin1, OUTPUT);
 pinMode(stepPin2, OUTPUT);
 pinMode(dirPin2, OUTPUT);
 pinMode(R_EN, OUTPUT);
 pinMode(L_EN, OUTPUT);
 pinMode(R_PWM, OUTPUT);
 pinMode(L_PWM, OUTPUT); 
 pinMode(run_led, OUTPUT);
 pinMode(stop_led, OUTPUT); 
 
 digitalWrite(R_PWM, HIGH);
 digitalWrite(L_PWM, HIGH);
 pinMode(run_btn, INPUT_PULLUP);
 pinMode(stop_btn, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(run_btn),start_trigger,FALLING);
 attachInterrupt(digitalPinToInterrupt(stop_btn),stop_trigger,FALLING);
 
 digitalWrite(run_led, LOW);
 digitalWrite(stop_led, LOW);
 servo4.attach(49); 
 servo5.attach(47);
 Serial.begin(9600);
}
String b = "";
int count = 0;
int drop = 0;
void loop() 
{ 
 if (count == 0)
 
{
 if (Serial.available ())
 
{
 a = Serial.readString();
 Serial.println(a);
 
 
 if (a.startsWith("x") == true)
 
{
 receive = a.substring(1);
 x = receive.toInt();
 
 
}
 if (a.startsWith("y") == true)
 
{
 receive = a.substring(1);
 z = receive.toInt();
 
}
 if (a.startsWith("R") == true)
 
{
 shape = 1;
 count = 1;
 drop = 0;
 
}
 if (a.startsWith("H") == true)
 
{
 shape = 2;
 count = 1;
drop = 0;
 
}
 if (a.startsWith("C") == true)
 
{
 shape = 3;
 count = 1;
 drop = 0;
 
}
 delay(100);
 
}
 
}
 if (count == 1)
 
{
 if (c==0 && shape == 3)
 
{
 pick_up();
 drop_cir();
 c = 3;
 shape = 4;
 drop = 1;
 
}
 if (c==0 && shape == 2)
 
{
 pick_up();
 drop_hex();
 c = 2;
 shape = 4;
 drop = 1;
 
}
 if (c==0 && shape == 1)
 
{
 pick_up();
 drop_rec();
 c = 1;
 shape = 4;
 drop = 1;
 
}
 if (c==1 && shape == 1)
 
{
 pick_up_rec();
 drop_rec();
 c = 1;
 shape = 4;
 drop = 1;
 
}
 if (c==1 && shape == 2)
 
{
 pick_up_rec();
 drop_hex();
 c = 2;
 shape = 4;
 drop = 1;
 
}
 if (c==1 && shape == 3)
 
{
 pick_up_rec();
 drop_cir();
 c = 3;
 shape = 4;
 drop = 1;
}
 if (c==2 && shape == 3)
 
{
 pick_up_hex();
 drop_cir();
 c = 3;
 shape = 4;
 drop = 1;
 
}
 if (c==2 && shape == 2)
 
{
 pick_up_hex();
 drop_hex();
 c = 2;
 shape = 4;
 drop = 1;
 
}
 if (c==2 && shape == 1)
 
{
 pick_up_hex();
 drop_rec();
 c = 1;
 shape = 4;
 drop = 1;
 
}
 if (c==3 && shape == 1)
 
{
 pick_up_cir();
 drop_rec();
 c = 1;
shape = 4;
 drop = 1;
 }
 if (c == 3 && shape == 2)
 {
 pick_up_cir();
 drop_hex();
 c = 2;
 shape = 4;
 drop = 1;
 }
 if ( c == 3 && shape == 3 )
 {
 pick_up_cir();
 drop_cir();
 c = 3;
 shape = 4;
 drop = 1;
 } 
 } 
 if (drop == 1) count = 0;
 if (digitalRead(run_btn) == 0 &&start == false) 
 { //start the system
 run_led_trigger();
 start = true;
 Serial.println("Run");
 convey(255);
 }
 if (digitalRead(stop_btn) == 0 && start == true)
{
 Serial.println("Stop");
 start = false;
 stop_led_trigger();
 convey(0);
 if (c == 1)
 {
 go_home_rec();
 c=0;
 }
 if (c == 2)
 {
 go_home_hex();
 c=0;
 }
 if (c == 3)
 {
 go_home_cir();
 c=0;
 }
 }
} 
void stepper_motor1(int angle,int speedd,int a)
{
 int stepp1 = ((angle * 6)*8)/ 1.8;
 digitalWrite(dirPin1, a); 
 for (int i = 0; i < stepp1; i++) 
 {
 digitalWrite(stepPin1, HIGH);
 delayMicroseconds(speedd);
digitalWrite(stepPin1, LOW);
 delayMicroseconds(speedd);
 }
 delay(1000);
}
void stepper_motor2(int angle,int speedd,int a)
{
 int stepp2 = ((angle*5)*32)/1.8; 
 digitalWrite(dirPin2, a); //High quay trai, LOW quay phai
 for (int i = 0; i < stepp2; i++) {
 digitalWrite(stepPin2, HIGH);
 delayMicroseconds(speedd);
 digitalWrite(stepPin2, LOW);
 delayMicroseconds(speedd);
 }
 delay(1000);
} 
void convey(int e)
{
 //analog level
 digitalWrite(R_PWM, HIGH);
 digitalWrite(L_PWM, LOW); //clockwise
 analogWrite(R_EN, e);
 analogWrite(L_EN, e);
}
void stepper_motor3(int angle,int speedd,int a)
{
 int stepp3 = ((angle*5)*8)/1.8; 
 digitalWrite(dirPin3, a); //High quay trai, LOW quay phai
 for (int i = 0; i < stepp3; i++){
  digitalWrite(stepPin3, HIGH);
 delayMicroseconds(speedd);
 digitalWrite(stepPin3, LOW);
 delayMicroseconds(speedd);
 }
 delay(1000);
}
void stepper_motor4(int angle)
{
 servo4.write(angle);
 delay(300);
} 
void stepper_motor5(int angle)
{
 servo5.write(angle);
 delay(200);
}
void pick_up()
{ 
 stepper_motor4(140);
 stepper_motor5(90);
 stepper_motor3(82,200,LOW);//HIGH di ra sau
 stepper_motor2(20,200,HIGH);// HIGH di phia trc
 stepper_motor5(35);
}
void drop_rec()
{
 stepper_motor3(20,400,HIGH);
 stepper_motor1(60,400,LOW);
 stepper_motor4(140);
stepper_motor3(20,400,LOW);
 stepper_motor5(100);
 stepper_motor3(20,400,HIGH); }
void pick_up_rec() {
 stepper_motor4(140);
 stepper_motor1(60,300,HIGH);
 stepper_motor3(20,300,LOW);
 stepper_motor4(140);
 stepper_motor5(45); }
void drop_hex() {
 stepper_motor3(20,400,HIGH);
 stepper_motor1(80,400,LOW);
 stepper_motor4(150);
 stepper_motor3(20,400,LOW);
 stepper_motor5(100);
 stepper_motor3(20,400,HIGH); }
void pick_up_hex() {
 stepper_motor4(140);
 stepper_motor1(80,150,HIGH);
 stepper_motor3(20,300,LOW);
 stepper_motor4(140);
 stepper_motor5(35);
}
void drop_cir() {
 stepper_motor3(20,400,HIGH);
 stepper_motor1(100,400,LOW);
 stepper_motor4(160);
 stepper_motor3(20,400,LOW);
 stepper_motor5(90);
 stepper_motor3(20,400,HIGH); }
void pick_up_cir() {
 stepper_motor4(140);
 stepper_motor1(100,150,HIGH);
 stepper_motor3(20,300,LOW);
 stepper_motor4(140);
 stepper_motor5(35); }
void go_home() {
 stepper_motor2(20,600,LOW);
 stepper_motor3(80,600,HIGH); }
void go_home_cir() {
 stepper_motor1(100,400,HIGH);
 stepper_motor2(20,400,LOW);
 stepper_motor3(60,400,HIGH);
 stepper_motor4(90);
stepper_motor5(45);
}
void go_home_hex()
{
 stepper_motor1(80,400,HIGH);
 stepper_motor2(20,400,LOW);
 stepper_motor3(60,400,HIGH);
 stepper_motor4(90);
 stepper_motor5(45);
}
void go_home_rec()
{
 stepper_motor1(60,400,HIGH);
 stepper_motor2(20,400,LOW);
 stepper_motor3(60,400,HIGH);
 stepper_motor4(90);
 stepper_motor5(45);
}
