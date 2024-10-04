#include <Servo.h>

Servo steer;

#define in1 2
#define ena 3
#define in2 4
#define servo 5
#define cTrig 6
#define cEcho 7 
#define rTrig 8
#define rEcho 9 
#define lTrig 10
#define lEcho 11 
#define rpr 12
#define rpl 13 
#define r2Trig A0
#define r2Echo A1 
#define l2Trig A2
#define l2Echo A3 


int cdis,rdis,ldis,r2dis,l2dis;
long milli,tim, before ,after;
int speedcam=30;
char rotate='n';

void setup() {
  pinMode(in1,OUTPUT);
  pinMode(ena,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(servo,OUTPUT);
  pinMode(cTrig,OUTPUT);
  pinMode(cEcho,INPUT);
  pinMode(rpr,INPUT);
  pinMode(rpl,INPUT);
  pinMode(rTrig,OUTPUT);
  pinMode(rEcho,INPUT);
  pinMode(lTrig,OUTPUT);
  pinMode(lEcho,INPUT);
  pinMode(r2Trig,OUTPUT);
  pinMode(r2Echo,INPUT);
  pinMode(l2Trig,OUTPUT);
  pinMode(l2Echo,INPUT);
  steer.attach(servo);
  Serial.begin(9600);
  moove(0, 0 ,500);

}

void loop() {
//  for(int i=0; i<12; i++){
//    start();
//  }
//  moove(0,0,9000000); 
ultra();

 if(digitalRead(rpr)){
  before = millis();
  while(digitalRead(rpr)){
  moove(speedcam,45,1);
 } 
  after= millis();
  moove(30,0,700);  
  after =after-before;
  moove(speedcam,-45,after);
  }
  else if(digitalRead(rpl)){
  before = millis();
  while(digitalRead(rpl)){
  moove(speedcam,-45,1);
 } 
  after= millis();
  moove(speedcam,0,700);  
  after =after-before;
  moove(speedcam,40,after);
  }
  
 else if(cdis<=15){
  moove(-speedcam,0,50);
   moove(0,0,300);
  moove(-speedcam,-80,2000);
 
}

else{
  ultra();
  if(rdis <=20){moove(30,-20 ,1);}
  else if(ldis <=20){moove(speedcam,20 ,1);}
  else{
    moove(speedcam,0,1);
  }
}
}

void start(){
  ultra();
  while((rdis<80)&&(ldis<80)){
    if(rdis <=15){moove(60,-20 ,1);}
  else if(rdis <=25){moove(50,-15 ,1);}
  else if(ldis <=15){moove(50,20 ,1);}
  else if(ldis<=25){moove(50,15,1);}
  else{moove(50,0,1);} 
    ultra();
}
moove(-40,0,50);
  if(rotate=='n'){
       if(rdis>=80){rotate='r';}
  else if(ldis>=80){rotate='l';}
  else{moove(-40,0,100);ultra();
        if(rdis>=80){rotate='r';}
        else if(ldis>=80){rotate='l';}}
  }
  ultra();
  if(cdis<43){
    moove(-10,0,200);
    moove(-50,0,500);
    moove(10,0,200);
  }
   turn();
   tim=millis();
   while((millis()-tim)<1250){
   if(rdis <=15){moove(60,-20 ,1);}
  else if(rdis <=25){moove(50,-15 ,1);}
  else if(ldis <=15){moove(50,20 ,1);}
  else if(ldis<=25){moove(50,15,1);}
  else{moove(60,0,1);} 
   }
}

void ultra(void){
  digitalWrite(rTrig,1);
  delayMicroseconds(10);
  digitalWrite(rTrig,0);
  int rt = pulseIn(rEcho,1,9000);
  rdis = 0.034*rt/2;
  if(rdis==0){rdis=97;}

  digitalWrite(lTrig,1);
  delayMicroseconds(10);
  digitalWrite(lTrig,0);
  int lt = pulseIn(lEcho,1,9000);
  ldis = 0.034*lt/2;   
  if(ldis==0){ldis=97;} 
  
  digitalWrite(cTrig,1);
  delayMicroseconds(10);
  digitalWrite(cTrig,0); 
  int ct = pulseIn(cEcho,1,9000);
  cdis = 0.034*ct/2;
  if(cdis==0){cdis=97;}

  digitalWrite(r2Trig,1);
  delayMicroseconds(10);
  digitalWrite(r2Trig,0);
  int r2t = pulseIn(r2Echo,1,9000);
  r2dis = 0.034*r2t/2;
  if(r2dis==0){r2dis=97;}
  
  digitalWrite(l2Trig,1);
  delayMicroseconds(10);
  digitalWrite(l2Trig,0);
  int l2t = pulseIn(l2Echo,1,6000);
  l2dis = 0.034*l2t/2;
  if(l2dis==0){l2dis=97;}
}

void ultra_test(){
  ultra();
  Serial.print(l2dis);
  Serial.print(" ");
  Serial.print(ldis);
  Serial.print("   ");
  Serial.print(cdis);
  Serial.print("   ");
  Serial.print(rdis);
  Serial.print(" ");
  Serial.println(r2dis);
}

void turn(){
  if(rotate=='r'){
    moove(50,85,450);
    ultra();
    while(cdis<=95){
    moove(50,85 ,1);
    ultra();
}
   moove(50,85,150);
  }
  else if(rotate=='l'){
    moove(50,-85,450);
    ultra();
    while(cdis<=95){
    moove(50,-85 ,1);
    ultra();
}
   moove(50,-85,120);
  }
}

void moove(int s,int d, long t){
if (s>=0){
 s= map(s,0,100,0,255);
 digitalWrite(in1,1);
 digitalWrite(in2,0);
 analogWrite(ena,s);
 
}else{
  s= map(s,-1,-100,0,255);
 digitalWrite(in1,0);
 digitalWrite(in2,1);
 analogWrite(ena,s);
  }

if (d>=0){
  d = map(d,0,100,77,30);
  steer.write(d);}
  else{
  d = map(d,-1,-100,77,120);
  steer.write(d);
}
delay(t);
}
