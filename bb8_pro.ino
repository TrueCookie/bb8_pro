#include <DFPlayer_Mini_Mp3.h>
#include <SoftwareSerial.h>

int const BlueLedProjector = 9; //Голографический проектор
int const LedBlueHeadlight = 10; //Крайний синий огонёк на голове
int const LedBlue1 = 14;//NC мигалка на корпусе
int const LedBlue2 = 15;//NC мигалка на корпусе
int const LedRed = 2;//NC огонёк на корпусе
int const LedRedEye = 6;//Красный глаз
int const LedWhite = 8;//Белый огонёк на голове
int const LedWhiteEye = 3;//Белый озвучивающий светодиод на голове
int const moveSense = 7;//Датчик движения 
int const knockSense = 16; //Датчик вибрации 
int const photoSense = 5; // Датчик света
int const ledPin = 13; // Светодиод
int const ledPin11 = 11;
int const ledPin12 = 12;// Светодиод
const int touchSense = 17;// Сенсор

const int vibro_sense = 160; // Уровень чувствительности для датчика вибрации
const int light_sense = 100; // Уровень чувствительности для датчика вибрации

unsigned long int timing = 0;
bool light_flag = digitalRead(photoSense);
bool vibro_flag = digitalRead(knockSense);

bool vibro();
void vibro_react();
void move_react();
void RedEye_bright();
void LedWhite_fade();
int run_time();
void rand_play(int begin, int end);
void speak();
void blinking(int someLed, int count);
void projector();
void blue_blinking();
void touch_react();
void light_check();

void setup(){
Serial.begin(9600);
mp3_set_serial(Serial);

pinMode(LedRed, OUTPUT);
pinMode(LedRedEye, OUTPUT);
pinMode(LedWhite, OUTPUT);
pinMode(LedWhiteEye, OUTPUT);
pinMode(photoSense, INPUT);
pinMode(moveSense, INPUT);
pinMode(knockSense, INPUT);
pinMode(ledPin11, OUTPUT);
pinMode(ledPin12, OUTPUT);
pinMode(ledPin, OUTPUT); 
pinMode(LedBlue2, OUTPUT); 
pinMode(LedBlue1, OUTPUT); 
pinMode(LedBlueHeadlight, OUTPUT); 
pinMode(BlueLedProjector, OUTPUT); 

analogWrite(LedRedEye, 255);
analogWrite(LedWhite, 255);

analogWrite(LedRed, 255);

digitalWrite(LedBlue1, HIGH);
digitalWrite(LedBlue2, HIGH);

delay(15000);
Serial.println("All ready");
mp3_set_volume(45);
mp3_play(23);
}

void loop(){
  light_check();
  //move_react();
  vibro_react();
  touch_react();
}

bool vibro(){
  if(vibro_flag != digitalRead(knockSense) && run_time() > 500){
    timing = millis();
    vibro_flag = digitalRead(knockSense);
    return true;
  }else{
    return false;
  }
}

void vibro_react(){
  if(vibro()){
    Serial.println("vibro");
    LedWhite_fade();
    RedEye_bright();
    rand_play(45,61);
    speak();
    blinking(LedRed, 3);
    blinking(LedBlueHeadlight, 2);
    blue_blinking();
  }
}

void move_react(){
  if(digitalRead(moveSense)){
    Serial.println("move");
    LedWhite_fade();
    rand_play(0,44);
    rand_play(3,40);
    speak();
    projector();
    blinking(LedBlueHeadlight, 2);
    blue_blinking();
  }
}

void RedEye_bright(){
  int light_level = 60;
  int delta = 15;
  timing = millis();  //?
  while(light_level < 255 && run_time() < 250){
    light_level += delta;
    analogWrite(LedRedEye, light_level);
    timing = millis();
  }
}

void LedWhite_fade(){ //let LedWhite fade 3 seconds
  if(run_time() < 3000){
    analogWrite(LedWhite, 128);
    timing = millis();
  }else{
    analogWrite(LedWhite, 255);
  }
}

int run_time(){ 
  return millis() - timing;
}

void rand_play(int begin, int end){
  mp3_play(random(begin, end));
  Serial.println(" *rand_play* ");
}

void speak(){
  blinking(LedWhiteEye, 3);
}

void blinking(int someLed, int count){
  int light_level = 0;
  int delta = 15;
  timing = millis();  //?
  for(int i = 0; i < count; ++i){
    while(light_level < 255 && run_time() < 1000){
      light_level += delta;
      analogWrite(someLed, light_level);
      timing = millis();
    }
    while(light_level > 0 && run_time() < 1000){
      light_level -= delta;
      analogWrite(someLed, light_level);
      timing = millis();
    }
  }
}

void projector(){
  timing = millis();
  if(run_time() < 5000){
    digitalWrite(BlueLedProjector, HIGH);
  }else{
    digitalWrite(BlueLedProjector, LOW);
  }
}

void blue_blinking(){
  for(int i = 0; i < 3; ++i){
    timing = millis();
    if(run_time() < 500){
      digitalWrite(LedBlue2, LOW);
      digitalWrite(LedBlue1, HIGH);
    }else{
      digitalWrite(LedBlue1, LOW);
      digitalWrite(LedBlue2, HIGH);
    }
  }
  digitalWrite(LedBlue1, HIGH);
  digitalWrite(LedBlue2, HIGH);
}

void touch_react(){
  if(digitalRead(touchSense)){
    rand_play(31, 44);
    Serial.println("Oh.. Someone touch me");
  }
}

void light_check(){
  if(run_time() > 1000){
    if(light_flag == false && digitalRead(photoSense) == true){
      mp3_set_volume(30);
      light_flag = true;
      Serial.println("light is OFF");
      mp3_play(23);
    }else if(light_flag == true && digitalRead(photoSense) == false){
      mp3_set_volume(45);
      light_flag = false;
      Serial.println("light is ON");
      mp3_play(22);
    }
    timing = millis();
  }
}
