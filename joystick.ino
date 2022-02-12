#include <Keyboard.h>
#include <Bounce.h>

// States, named, i.e. numbered, after the read IO pins
#define S0 0  // 00
#define S1 1  // 01
#define S2 2  // 10
#define S3 3  // 11

// Transitions, named after the state sequence
#define S013 (S0<<4 | S1<<2 | S3) // i.e. S0->S1->S3 001011
#define S320 (S3<<4 | S2<<2 | S0) //                 111000
#define S310 (S3<<4 | S1<<2 | S0) // CCW             110100
#define S023 (S0<<4 | S2<<2 | S3) //                 001011

#define SW_LA 7
#define SW_LB 8
#define SW_LC 6

#define B_TA 9
#define B_TB 10
#define B_TC 11

#define SW_TA 21
#define SW_TB 20
#define SW_TC 19

#define HANDHELD_1_KEY '1'
#define HANDHELD_2_KEY '2'
#define HANDHELD_3_KEY '3'
#define HANDHELD_4_KEY '4'

int rotA_1 = 0;
int rotA_2 = 1;
int historyA = 0;

int rotB_1 = 2;
int rotB_2 = 3;
int historyB = 0;

int rotC_1 = 4;
int rotC_2 = 5;
int historyC = 0;

int posA = 0;
int posB = 0;
int posC = 0;

unsigned int handheld_screen = 1;  // First screen is #1
char handheld_key[4] = {HANDHELD_1_KEY, HANDHELD_2_KEY, HANDHELD_3_KEY, HANDHELD_4_KEY};

Bounce switch_LB = Bounce(SW_LB, 10);
Bounce switch_TA = Bounce(SW_TA, 10);
Bounce switch_TB = Bounce(SW_TB, 10);
Bounce switch_TC = Bounce(SW_TC, 10);

long t = 0;


void setup() {
  Keyboard.begin();
  Joystick.useManualSend(true);
  int historyA = 0;
  int historyB = 0;
  int historyC = 0;

  pinMode(rotA_1, INPUT_PULLUP);
  pinMode(rotA_2, INPUT_PULLUP);
  pinMode(rotB_1, INPUT_PULLUP);
  pinMode(rotB_2, INPUT_PULLUP);
  pinMode(rotC_1, INPUT_PULLUP);
  pinMode(rotC_2, INPUT_PULLUP);
  pinMode(SW_LA, INPUT_PULLUP);
  pinMode(SW_LB, INPUT_PULLUP);
  pinMode(SW_LC, INPUT_PULLUP);
  pinMode(B_TA,  INPUT_PULLUP);
  pinMode(B_TB,  INPUT_PULLUP);
  pinMode(B_TC,  INPUT_PULLUP);
  pinMode(SW_TA, INPUT_PULLUP);
  pinMode(SW_TB, INPUT_PULLUP);
  pinMode(SW_TC, INPUT_PULLUP);
  
  //Serial.begin (115200);

  historyA = (digitalRead(rotA_1) << 1) | digitalRead(rotA_2);
  historyA = historyA << 4 | historyA << 2 | historyA; // harmless historyA
  historyB = (digitalRead(rotB_1) << 1) | digitalRead(rotB_2);
  historyB = historyB << 4 | historyB << 2 | historyB; // harmless historyB
  historyC = (digitalRead(rotC_1) << 1) | digitalRead(rotC_2);
  historyC = historyC << 4 | historyC << 2 | historyC; // harmless historyC

  // Update button and switch status to prevent keyboard
  // input at connection.
  switch_LB.update();
  switch_TA.update();
  switch_TB.update();
  switch_TC.update();

  t = millis();
}


void loop() {
    uint8_t newstateA;
    uint8_t newstateB;
    uint8_t newstateC;
    int btn_down;
    int btn_up;

    switch_LB.update();
    switch_TA.update();
    switch_TB.update();
    switch_TC.update();

    if (switch_LB.fallingEdge()) Keyboard.print("5");
    if (switch_LB.risingEdge()) Keyboard.print("5");
    if (switch_TA.fallingEdge()) Keyboard.print("G");
    if (switch_TA.risingEdge()) Keyboard.print("G");
    if (switch_TB.fallingEdge()) Keyboard.print("6");
    if (switch_TB.risingEdge()) Keyboard.print("6");
    if (switch_TC.fallingEdge()) Keyboard.print("7");
    if (switch_TC.risingEdge()) Keyboard.print("7");
    
    newstateA = (digitalRead(rotA_1) << 1) | digitalRead(rotA_2);
    newstateB = (digitalRead(rotB_1) << 1) | digitalRead(rotB_2);
    newstateC = (digitalRead(rotC_1) << 1) | digitalRead(rotC_2);

    if (newstateA != (historyA & 0x3)) {
      historyA = ((historyA << 2) | newstateA) & 0x3f;
 
      switch (historyA) {
        case S013: // CW
          //break;
        case S320:
          posA += 2;
          break;
          
        case S310: //CCW
          //break;
        case S023:
          posA -= 2;
          break;
      }
    }

    if (newstateB != (historyB & 0x3)) {
      historyB = ((historyB << 2) | newstateB) & 0x3f;
 
      switch (historyB) {
        case S013: // CW
          break;
        case S320:
          posB++;
          break;
          
        case S310: //CCW
          break;
        case S023:
          posB--;
          break;
      }
    }

    if (newstateC != (historyC & 0x3)) {
      historyC = ((historyC << 2) | newstateC) & 0x3f;
      
      switch (historyC) {
        case S013: // CW
          break;
        case S320:
          posC++;
          break;
          
        case S310: //CCW
          break;
        case S023:
          posC--;
          break;
      }
    }
    
    if ((millis() - t) > 10) {
      t = millis();
      int cnt_down = 1;
      
      Joystick.X(map(analogRead(A0), 0, 1023, 1023, 0));
      Joystick.Y(map(analogRead(A1), 0, 1023, 1023, 0));
      Joystick.Z(analogRead(A2));
      Joystick.sliderLeft(analogRead(A3));
      Joystick.sliderRight(analogRead(A4));
      Joystick.button(13, !digitalRead(B_TA));
      Joystick.button(14, !digitalRead(B_TB));
      Joystick.button(15, !digitalRead(B_TC));
      //Joystick.button(16, !digitalRead(SW_TA));
      //Joystick.button(17, !digitalRead(SW_TB));
      //Joystick.button(18, !digitalRead(SW_TC));

      // Rotary encoder and SW A
      if (digitalRead(SW_LA) == HIGH) {
        btn_down = 1;
        btn_up = 2;
        cnt_down = 2;
      }
      else {
        btn_down = 7;
        btn_up = 8;
        cnt_down = 1;
      }

      if (posA > 0){
        Joystick.button(btn_down, 1);
        posA -= cnt_down;
      }
      else if (posA < 0){
        Joystick.button(btn_up,1);
        posA += cnt_down;
      }
      else {
        Joystick.button(btn_down,0);
        Joystick.button(btn_up,0);
      }


      // Rotary encoder and SW B
      if (digitalRead(SW_LB) == HIGH) {
        btn_down = 3;
        btn_up = 4;
        cnt_down = 1;
      }
      else {
        btn_down = 3;
        btn_up = 4;
        cnt_down = 1;
      }
      if (posB > 0){
        Joystick.button(btn_down, 1);
        posB -= cnt_down;
      }
      else if (posB < 0){
        Joystick.button(btn_up,1);
        posB += cnt_down;
      }
      else {
        Joystick.button(btn_down,0);
        Joystick.button(btn_up,0);
      }

      btn_down = 5;
      btn_up = 6;
      // Rotary encoder and SW C
      int hh = !digitalRead(SW_LC);
      if (posC > 0){
        if (hh == HIGH) {
          Joystick.button(btn_down, 1);
        }
        else {
          handheld_screen--;
          Keyboard.print(handheld_key[handheld_screen % 4]);
        }
        posC--;
      }
      else if (posC < 0){
        if (hh == HIGH) {
          Joystick.button(btn_up,1);
        }
        else {
          handheld_screen++;
          Keyboard.print(handheld_key[handheld_screen % 4]);
        }
        posC++;
      }
      else {
        Joystick.button(btn_down,0);
        Joystick.button(btn_up,0);
      }

      Joystick.send_now();
    }
    
}
