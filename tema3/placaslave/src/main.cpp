//Michael Klements
//The DIY Life
//17 May 2019 
#include <Arduino.h>
#include <SPI.h>
#define RED_LED1 9
#define GREEN_LED1 8
#define BLUE_LED1 7
#define RED_LED2 6
#define GREEN_LED2 5
#define BLUE_LED2 4
#define BTNRED1 150
#define BTNGREEN1 250
#define BTNBLUE1 300
#define BTNRED2 350
#define BTNGREEN2 450
#define BTNBLUE2 800
#define RGBRED1 A5
#define RGBGREEN1 2
#define RGBBLUE1 3
#define RGBRED2 A4
#define RGBGREEN2 A2
#define RGBBLUE2 A3
#define ANALOG_PIN A1
#define ROUND_TIME 15000
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int buttonValue;
bool startRound = false;
int player = 1;
int playertoSend = 1;
char player1Score = 0;
char player2Score = 0;
int currentMillis;
int roundStartMillis;
bool correctButton = false;
bool btnRed1 = false;
bool btnGreen1 = false;
bool btnBlue1 = false;
bool btnRed2 = false;
bool btnGreen2 = false;
bool btnBlue2 = false;
int setRGBTime;
int reactionTime;
int currentColor;
int score = 0;
bool roundEnded = false;
bool startScoreCommunication = false;
bool roundStartMaster = false;
unsigned long roundEndMillis=0;
//functiile setRGB1 si setRGB2 seteaza in mod aleator culoarea unui LED RGB
void setRGB1(){
   currentColor = random(1, 4);
   switch (currentColor){
     case 1:
      digitalWrite(RGBRED1, HIGH);
      digitalWrite(RGBGREEN1, LOW);
      digitalWrite(RGBBLUE1, LOW);
      Serial.println("Red1");
      break;
     case 2:
      digitalWrite(RGBGREEN1, HIGH);
      digitalWrite(RGBRED1, LOW);
      digitalWrite(RGBBLUE1, LOW);
      Serial.println("Green1");
      break;
     case 3:
      digitalWrite(RGBBLUE1, HIGH);
      digitalWrite(RGBRED1, LOW);
      digitalWrite(RGBGREEN1, LOW);
      Serial.println("Blue1");
      break;
   }
    //stinge celalalt LED
    digitalWrite(RGBRED2, LOW);
    digitalWrite(RGBGREEN2, LOW);
    digitalWrite(RGBBLUE2, LOW);
}
void setRGB2(){
   currentColor = random(1, 4);
   switch (currentColor){
     case 1:
      digitalWrite(RGBRED2, HIGH);
      digitalWrite(RGBGREEN2, LOW);
      digitalWrite(RGBBLUE2, LOW);
      Serial.println("Red2");
      break;
     case 2:
      digitalWrite(RGBGREEN2, HIGH);
      digitalWrite(RGBRED2, LOW);
      digitalWrite(RGBBLUE2, LOW);
      Serial.println("Green2");
      break;
     case 3:
      digitalWrite(RGBBLUE2, HIGH);
      digitalWrite(RGBRED2, LOW);
      digitalWrite(RGBGREEN2, LOW);
      Serial.println("Blue2");
      break;
   }
    //stinge celalalt LED
    digitalWrite(RGBRED1, LOW);
    digitalWrite(RGBGREEN1, LOW);
    digitalWrite(RGBBLUE1, LOW);

}
//functia getButtonValue determina daca un buton a fost apasat
void getButtonValue(){
  if (buttonValue > BTNRED1 && buttonValue < BTNGREEN1){
    btnRed1 = true;
  }
  else{
    btnRed1 = false;
  }
  if (buttonValue > BTNGREEN1 && buttonValue < BTNBLUE1){
    btnGreen1 = true;
  }
  else{
    btnGreen1 = false;
  }
  if (buttonValue > BTNBLUE1 && buttonValue < BTNRED2){
    btnBlue1 = true;
  }
  else{
    btnBlue1 = false;
  }
  if (buttonValue > BTNRED2 && buttonValue < BTNGREEN2){
    btnRed2 = true;
  }
  else{
    btnRed2 = false;
  }
  if (buttonValue > BTNGREEN2 && buttonValue < BTNBLUE2){
    btnGreen2 = true;
  }
  else{
    btnGreen2 = false;
  }
  if (buttonValue > BTNBLUE2){
    btnBlue2 = true;
  }
  else{
    btnBlue2 = false;
  }
}
//functia correctButtonPressed determina daca butonul corespunzator culorii date de RGB a fost apasat
int correctButtonPressed(){
  if(player == 1){
    switch (currentColor){
      case 1:
        if (btnRed1){
          return 1;
        }
        else{
          return 0;
        }
        break;
      case 2:
        if (btnGreen1){
          return 1;
        }
        else{
          return 0;
        }
        break;
      case 3:
        if (btnBlue1){
          return 1;
        }
        else{
          return 0;
        }
        break;
    }
    }
  else if(player == 2){
    switch (currentColor){
      case 1:
        if (btnRed2){
          return 1;
        }
        else{
          return 0;
        }
        break;
      case 2:
        if (btnGreen2){
          return 1;
        }
        else{
          return 0;
        }
        break;
      case 3:
        if (btnBlue2){
          return 1;
        }
        else{
          return 0;
        }
        break;
    }
  }
}
void startGame(){
  player =  1;
  player1Score = 0;
  player2Score = 0;
  startRound = true;
  currentMillis = millis();
  roundStartMillis = currentMillis;
  Serial.println("Game Started");
  correctButton = true;
  startScoreCommunication = true;
}
void round(){
  if(currentMillis - roundStartMillis < ROUND_TIME){
    if (millis() - lastDebounceTime > debounceDelay){
      //Serial.println("button pressed");
      getButtonValue();
      lastDebounceTime = millis();
    }
    if(correctButton){
      correctButton = false;
      if (player == 1){
        setRGB1();
        Serial.println("Set RGB1");
        setRGBTime = millis();
      }
      else{
        setRGB2();
        Serial.println("Set RGB2");
        setRGBTime = millis();
      }
    }
    if (correctButtonPressed()){
      reactionTime = millis() - setRGBTime;
      Serial.print("Reaction Time: ");
      Serial.println(reactionTime);
      if (reactionTime < 1000){
        score = 4;
      }
      else if (reactionTime < 2000){
        score = 3;
      }
      else if (reactionTime < 3000){
        score = 2;
      }
      else{
        score = 1;
      }
      if (player == 1){
        player1Score = player1Score + score;
        //Serial.println("Player 1 Scored");
      }
      else{
        player2Score = player2Score + score;
        //Serial.println("Player 2 Scored");
      }
      correctButton = true;
    }
  }
  else{
    if (player == 1){
      Serial.println("Player 1 Scored");
      player = 2;
      correctButton = true;
    }
    else{
      player = 1;
      playertoSend = 1;
      startRound = false;
      Serial.println("round ended");
      Serial.print("Player 1 Score: ");
      Serial.println((int)player1Score);
      Serial.print("Player 2 Score: ");
      Serial.println((int)player2Score);
      digitalWrite(RGBRED2, LOW);
      digitalWrite(RGBGREEN2, LOW);
      digitalWrite(RGBBLUE2, LOW);
      digitalWrite(RGBRED1, LOW);
      digitalWrite(RGBGREEN1, LOW);
      digitalWrite(RGBBLUE1, LOW);
      roundEndMillis = millis();
      roundEnded = true;
      roundStartMaster = false;
      startScoreCommunication = true;
    }
    roundStartMillis = millis();
  }
}
void setup()
{
  pinMode(RED_LED1, OUTPUT);
  pinMode(GREEN_LED1, OUTPUT);
  pinMode(BLUE_LED1, OUTPUT);
  pinMode(RED_LED2, OUTPUT);
  pinMode(GREEN_LED2, OUTPUT);
  pinMode(BLUE_LED2, OUTPUT);
  pinMode(RGBRED1, OUTPUT);
  pinMode(RGBGREEN1, OUTPUT);
  pinMode(RGBBLUE1, OUTPUT);
  pinMode(RGBRED2, OUTPUT);
  pinMode(RGBGREEN2, OUTPUT);
  pinMode(RGBBLUE2, OUTPUT);
  pinMode(ANALOG_PIN, INPUT);
  pinMode(MISO, OUTPUT); 
  SPCR |= _BV(SPE);  // enable SPI
  SPI.attachInterrupt();  // interrupt handler
  Serial.begin(28800);
}

void loop()
{
  buttonValue = analogRead(ANALOG_PIN);  //Read in the button value
  //Serial.println(buttonValue);   //Print the button value to the serial monitor
  currentMillis = millis();
  digitalWrite(RED_LED1, HIGH); 
  digitalWrite(GREEN_LED1, HIGH);
  digitalWrite(BLUE_LED1, HIGH);
  digitalWrite(RED_LED2, HIGH);
  digitalWrite(GREEN_LED2, HIGH);
  digitalWrite(BLUE_LED2, HIGH);
  if (millis() - roundEndMillis > 5000){
    if(buttonValue > BTNRED1-30){
      if (!startRound){
        startGame();
        lastDebounceTime = millis();
      }
    }
    }
  if (startRound){
    round();
  }
  
}

 ISR(SPI_STC_vect) {
  char c = SPDR;  // Citește comanda de la master
  if (roundEnded) {
    roundEnded = false;
    SPDR = 'N';  // Trimite "round ended"
    player = 1;  // Resetează playerul
    return;
  }
  if (startRound && !roundStartMaster) {
    roundStartMaster = true;
    Serial.println("sent round started to master");
    SPDR = 'S';  // Trimite "round started"
    return;
  }
  if (startScoreCommunication) {
    if (SPDR == 'B') {  // Master cere scorul playerului 1
      SPDR = player1Score;
    } else if (SPDR == 'A') {  // Master cere scorul playerului 2
      SPDR = player2Score;
    } 
    return;
  }
}