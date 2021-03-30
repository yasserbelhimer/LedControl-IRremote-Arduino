#include <IRremote.h>
#include "LedControl.h"
#include "Timer.h"

const int RECV_PIN = 7;
LedControl lc = LedControl(12,11,10,3);
IRrecv irrecv(RECV_PIN);
decode_results results;
int j = 0,jk = 0;


byte tx[80]={B00000000,B00000000,B01111100,B10000010,B10000010,B10000010,B01111100,B00000000,
             B00000000,B00000000,B11111110,B00000100,B00001000,B00000100,B11111110,B00000000,
             B00000000,B00000000,B01111100,B10000010,B10000010,B10000010,B00000000,B00000000,
             B00111100,B01000010,B10010101,B10100001,B10100001,B10010101,B01000010,B00111100,
             B00000000,B00000000,B01111110,B10000000,B10000000,B10000000,B01111110,B00000000,
             B00000000,B00000000,B10001100,B10010010,B10010010,B01100010,B00000000,B00000000,
             B00000000,B00000000,B00000010,B00000010,B11111110,B00000010,B00000010,B00000000,
             B00000000,B00000000,B11111110,B00010000,B00010000,B11111110,B00000000,B00000000,
             B00000000,B00000000,B11111110,B10010010,B10010010,B01101100,B00000000,B00000000,
             B00111100,B01000010,B10100101,B10010001,B10010001,B10100101,B01000010,B00111100};
void shift(){
  byte h;
  h = tx[0];
  for(int i=0;i<79;i++){
    tx[i] = tx[i+1];
  }
  tx[79] = h;
}
void scroll(){
  int r = 0;
  for(int i = 0;i<64;i++){
    lc.setRow(2,i,tx[i]);
    lc.setRow(1,i,tx[i+8]);
    lc.setRow(0,i,tx[i+16]);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);

  for(int i = 0 ;i<3;i++){
    lc.shutdown(i,false);
    lc.setIntensity(i,8); // Set brightness to a medium value
    lc.clearDisplay(i); // Clear the display
  }
}
byte none[] = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};

void drawNone(){
  for(int i = 0;i<8;i++){
      lc.setRow(2,i,none[i]);
      lc.setRow(1,i,none[i]);
      lc.setRow(0,i,none[i]);
  }
}
void loop() {
  drawHello();
  delay(250);
  drawNone();
  delay(250);
  // put your main code here, to run repeatedly:
   if (irrecv.decode(&results)){
    switch(results.value){
      case 0xFF30CF: {// afficher une forme
        drawFaces(0);
        int vrai = 1;
        while(vrai){
          if (irrecv.decode(&results)){
            switch(results.value){
              case 0xFF02FD:{
                  drawFaces(1);
                break ;
              }
              case 0xFF22DD:{
                  drawFaces(-1);
                break ;
              }
              case 0xFF906F:{
                  vrai = 0;
                break ;
              }
            }
            irrecv.resume();
          }
        }
        break;
      }
      case 0xFF18E7: {// alphabets
        Serial.println("2");
        drawAlphabet(0);
        int vrai = 1;
        while(vrai){
          if (irrecv.decode(&results)){
            switch(results.value){
              case 0xFF02FD:{
                  drawAlphabet(1);
                break ;
              }
              case 0xFF22DD:{
                  drawAlphabet(-1);
                break ;
              }
              case 0xFF906F:{
                  vrai = 0;
                break ;
              }
            }
            irrecv.resume();
          }
        }
        break;
      }
      case 0xFF7A85: {
        Serial.println("3");
        int vrai = 1;
        while(vrai){
          if (irrecv.decode(&results)){
            switch(results.value){
              case 0xFF906F:{
                  vrai = 0;
                break ;
              }
            }
            irrecv.resume();
          }
         scroll();
         shift();
         delay(300);
        }
        break;
      }
    }
        irrecv.resume();
  }
}

// happy face
byte faceces[][8]= {{B00111100,B01000010,B10010101,B10100001,B10100001,B10010101,B01000010,B00111100},
                    {B00111100,B01000010,B10010101,B10010001,B10010001,B10010101,B01000010,B00111100},
                    {B00111100,B01000010,B10100101,B10010001,B10010001,B10100101,B01000010,B00111100},
                    {B00000000,B01000010,B00100000,B00101100,B00100000,B01000010,B00000000,B00000000},
                    {B00000000,B00100010,B01000000,B01011100,B01000000,B00100010,B00000000,B00000000},
                    {B00001100,B00010010,B00100010,B01000100,B01000100,B00100010,B00010010,B00001100},
                    {B00000000,B11000011,B01000010,B11111111,B11111111,B01000010,B11000011,B00000000},
                    {B01001111,B01001000,B01001000,B01111111,B00001001,B00001001,B01111001,B00000000},
                    {B00000000,B00000000,B00001000,B01111100,B00001010,B00000010,B00000000,B00000000}};
int abslut(int x,int y){
  if(x<0){
    x =-x;
    x =y-x;
  }
   
  return x;
}
void drawFaces(int k){
    // Display sad face
    if(k==1)
        j++;
    else if (k==-1)
        j--;
    for(int i = 0;i<8;i++){
      lc.setRow(2,i,faceces[abslut(j%9,9)][i]);
      lc.setRow(1,i,faceces[abslut((j+1)%9,9)][i]);
      lc.setRow(0,i,faceces[abslut((j+2)%9,9)][i]);
  }
}

byte alphabet[][8]=   {{B00000000,B00000000,B11111100,B00010010,B00010010,B11111100,B00000000,B00000000},//A
                       {B00000000,B00000000,B11111110,B10010010,B10010010,B01101100,B00000000,B00000000},//B
                       {B00000000,B00000000,B01111100,B10000010,B10000010,B10000010,B00000000,B00000000},//C
                       {B00000000,B11111111,B10000001,B10000001,B01000010,B00111100,B00000000,B00000000},//D
                       {B00000000,B00000000,B11111110,B10010010,B10010010,B10010010,B00000000,B00000000},//E
                       {B00000000,B00000000,B11111110,B00010010,B00010010,B00010010,B00000000,B00000000},//F
                       {B00000000,B00000000,B01111100,B10000010,B10100010,B01100010,B00000000,B00000000},//G
                       {B00000000,B00000000,B11111110,B00010000,B00010000,B11111110,B00000000,B00000000},//H
                       {B00000000,B00000000,B10000010,B11111110,B10000010,B00000000,B00000000,B00000000},//I
                       {B00000000,B00000000,B01100000,B10000000,B10000000,B01111110,B00000000,B00000000},//J
                       {B00000000,B00000000,B11111110,B00010000,B00101000,B01000100,B00000000,B00000000},//K
                       {B00000000,B00000000,B11111110,B10000000,B10000000,B10000000,B00000000,B00000000},//L
                       {B00000000,B00000000,B11111110,B00000100,B00001000,B00000100,B11111110,B00000000},//M
                       {B00000000,B11111110,B00000100,B00011000,B00100000,B11111110,B00000000,B00000000},//N
                       {B00000000,B00000000,B01111100,B10000010,B10000010,B10000010,B01111100,B00000000},//O
                       {B00000000,B00000000,B11111110,B00010010,B00010010,B00001100,B00000000,B00000000},//P
                       {B00000000,B01111100,B10000010,B10000010,B10100010,B01111100,B10000000,B00000000},//Q
                       {B00000000,B00000000,B11111110,B00010010,B00010010,B11101100,B00000000,B00000000},//R
                       {B00000000,B00000000,B10001100,B10010010,B10010010,B01100010,B00000000,B00000000},//S
                       {B00000000,B00000000,B00000010,B00000010,B11111110,B00000010,B00000010,B00000000},//T
                       {B00000000,B00000000,B01111110,B10000000,B10000000,B10000000,B01111110,B00000000},//U
                       {B00000000,B00000000,B00001100,B00110000,B01000000,B00110000,B00001100,B00000000},//V
                       {B00001100,B00110000,B01100000,B00010000,B01100000,B00110000,B00001100,B00000000},//W
                       {B00000000,B00000000,B11000110,B00101000,B00010000,B00101000,B11000110,B00000000},//X
                       {B00000000,B00000110,B00001000,B11110000,B00001000,B00000110,B00000000,B00000000},//Y
                       {B00000000,B11000010,B10100010,B10010010,B10001010,B10000110,B00000000,B00000000},//Z
                       {B01111110,B00001000,B00001000,B01111110,B00000000,B01000010,B01111110,B01000010}};

void drawAlphabet(int k){
    // Display sad face
    if(k==1)
        j++;
    else if (k==-1)
      j--;
    for(int i = 0;i<8;i++){
      lc.setRow(2,i,alphabet[abslut(j%26,26)][i]);
      lc.setRow(1,i,alphabet[abslut((j+1)%26,26)][i]);
      lc.setRow(0,i,alphabet[abslut((j+2)%26,26)][i]);
  }
}

void drawHello(){
  for(int i = 0;i<8;i++){
      lc.setRow(2,i,alphabet[7][i]);
      lc.setRow(1,i,alphabet[8][i]);
      lc.setRow(0,i,faceces[0][i]);
  }
}

void scrollAlphabet(){
  drawAlphabet(1);
  delay(300);
}
