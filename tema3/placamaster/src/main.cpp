#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <SPI.h>
#define ROUND_TIME 30000
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo myservo;  // create servo object to control a servo
char player1Score = 0;
char player2Score = 0;
int player = 1;
bool changeLCD=true;
bool roundStart = false;
unsigned long roundStartTime = 0;
unsigned long roundEndTime = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay=50;
bool showStart = true;


// Variabile pentru controlul rotației servo
int pos = 0;                // Poziția curentă a servo
bool forward = true;        // Direcția de mișcare
unsigned long lastMove = 0; // Timpul ultimei actualizări a servo
int stepDelay;              // Întârziere între pașii de rotație
int stepSize = 1;           // Mărimea pasului în grade

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(28800);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);   // Setează servo la poziția 0
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV16);  //divide the clock by 8
  Serial.println("Hello I'm SPI Mega_Master");
  randomSeed(millis());
  pinMode(SS, OUTPUT); // Configurare SS ca OUTPUT
  digitalWrite(SS, HIGH); // SS dezactivat implicit
  stepDelay = ROUND_TIME / (2 * 180);  // ROUND_TIME împărțit pe 2 cicluri a câte 180 de pași
}

void loop() {
  //start SPI communcaiton
  if(changeLCD && roundStart) {
    lcd.clear();
    lcd.print("Player1: ");
    lcd.print((int)player1Score);
    lcd.setCursor(0,1);
    lcd.print("Player2: ");
    lcd.print((int)player2Score);
    changeLCD=false;
    }
  else if(!roundStart && showStart){
    lcd.clear();
    lcd.print("Start round ?");
  }
  digitalWrite(SS, LOW);  // Activează slave
  
  char received;
  if(!roundStart){
    received = SPI.transfer('R');
    if(millis() - roundEndTime >= 5000){
      showStart = true;
    }
  }
  if(received == 'S'){
    roundStart = true;
    roundStartTime = millis();
    changeLCD = true;
    Serial.println("Round start");
    showStart = false;
  }
  if (roundStart){
    unsigned long currentMillis = millis();
  if (currentMillis - lastMove >= stepDelay) {
    if (forward) {
      pos += stepSize;  // Crește poziția
      if (pos >= 180) {
        forward = false;  // Inversare direcție
      }
    } else {
      pos -= stepSize;  // Scade poziția
      if (pos <= 0) {
        forward = true;  // Inversare direcție
      }
    }
    myservo.write(pos);      // Setează poziția servo
    lastMove = currentMillis;  // Actualizează timpul ultimei mișcări
  }
  if(millis()-lastDebounceTime > debounceDelay){
    if (player == 1) {
      received = SPI.transfer('A');
      if (received != 'A' && received != 'B') {
        if (received != player1Score) {
          player1Score = received;  // Actualizează scorul
          changeLCD = true;         // Semnalează actualizare LCD
        }
        player =2;  // Schimbă la playerul 2
      }
      
      // Trimite comanda pentru playerul 1
    } else {
      received = SPI.transfer('B');  // Trimite comanda pentru playerul 2
       if (received != 'A' && received != 'B') {
        if (received != player2Score) {
          player2Score = received;  // Actualizează scorul
          changeLCD = true;         // Semnalează actualizare LCD
        }
        player =1;  // Schimbă la playerul 1
    }
    }
    lastDebounceTime = millis();
    
    

    if (received == 'N') {
      roundStart = false;
      roundStartTime = 0;
      player1Score = 0;
      player2Score = 0;
      pos = 0;
      myservo.write(pos);
      roundEndTime = millis();
    } 
    
  } 
  }
  digitalWrite(SS, HIGH);  // disable Slave Select
  }