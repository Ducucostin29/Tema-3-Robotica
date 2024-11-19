# Tema-3-Robotica

În această temă, fiecare echipă va crea un joc de reflex competitiv pentru doi jucători, în care ambii participanți vor concura pentru a obține cel mai mare punctaj, testându-și viteza de reacție. Proiectul se va realiza în echipe de câte două persoane.
Fiecare jucător va avea butoane și LED-uri proprii, iar jocul se va desfășura în mai multe runde. Scopul fiecărui jucător este să apese cât mai rapid butonul care corespunde culorii afișate pe LED-ul RGB al echipei sale. Punctajul fiecărui jucător va fi afișat pe un ecran LCD și se va actualiza pe parcursul jocului. La finalul jocului, jucătorul cu cel mai mare punctaj este declarat câștigător.

## Componentele utilizate
 - 6x LED-uri (2 grupuri de câte 3 leduri, în cadrul unui grup trebuie să avem culori diferite)
 - 2x LED RGB (1 pentru fiecare jucător)
 - 6x butoane (3 pentru fiecare jucător)
 - 20x Rezistoare (7x 220/330ohm, 2x 2K)
 - 1x LCD
 - 1x servomotor
 - 2x Breadboard
 - Fire de legatura
 - 2x Placa Arduino Uno R3 ATmega328P/PA
 - 
## Poze ale setup-ului fizic
 - ![poza tema 3](https://github.com/user-attachments/assets/ef2033e6-9474-48ac-81ec-ea7ef058cfb8)
 - ![tema 3 poza](https://github.com/user-attachments/assets/b9510264-7263-4286-803e-9790f5739f8c)

## Video cu funcționalitatea montajului fizic
[![Watch the video] (https://www.youtube.com/watch?v=G-zg-tN-5XA&ab_channel=AlexandruApostu)

## Schema electrică (TinkerCAD)
- Conectarea LED-urilor albastre, verzi si rosii la pinii 4,5,6,7,8,9 de pe placa Slave.
- Conectarea LED-urilor RGB la pinii 2,3 si A2,A3,A4,A5 de pe placa Slave.
- Utilizarea rezistențelor pentru protecția LED-urilor și pentru stabilizarea butoanelor.
![image](https://github.com/user-attachments/assets/1ca905ea-d34b-4953-80f6-dcb121d4e1b6)
![image](https://github.com/user-attachments/assets/ad0adbd0-1c18-4985-a516-f9e782e55db9)
![image](https://github.com/user-attachments/assets/d5a5618c-ff61-4c73-b2f1-e2b07c65f78b)

### Placa Master
Aceasta placa controleaza LCD-ul, servomotorul si comunicatia cu placuta slave.

Setup LCD:
```
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
```

Control LCD:
```
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
```
Servomotorul trece prin o rotatie completa la fiecare runda a fiecarui jucator, trecand prin cele 180 de pozitie. Isi schimba pozitia la un interval de timp egal cu ROUND_TIME / (2 * 180)
```
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
```
Placa master trimite prin SPI catre slave caracterul 'A', atunci cand runda a inceput si asteapta scorul primului jucator, si caracterul 'B', cand asteapta scorul celui de-al doilea jucator.
```
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
```
Primeste de la placuta slave caracterul 'N' atunci cand runda s-a sfarsit.
```
if (received == 'N') {
      roundStart = false;
      roundStartTime = 0;
      player1Score = 0;
      player2Score = 0;
      pos = 0;
      myservo.write(pos);
      roundEndTime = millis();
    } 

```
### Placa slave
Aceasta placa controleaza LED-urile, trateaza input-urile de la butoane, ruleaza logica jocului si trimite scorul jucatorilor catre placa master prin SPI.

Functiile care seteaza LED-urile RGB in mod aleator:
```
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
```
Functia care ia inputul de pinul analog si decide care buton a fost apasat:
```
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
```
Functia aceasta contine codul pentru runda. Daca este randul jucatorului 1, seteaza RGB1 si verifica inputurile corespunzatoare acestuia, calculeaza viteza de reactie a jucatorului. Asemanator pentru player 2 (seteaza RGB2 si verifica inputurile acestuia). In functie de timpul de reactie este calculat scorul pentru fiecare jucator. De asemenea se verifica daca a trecut timpul designat rundei, si daca a trecut, runda se opreste.
```
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
```
In aceasta functie ISR utilizează SPDR atât pentru a citi comenzile de la master, cât și pentru a trimite date (confirmări, scoruri) înapoi. Toată logica este gestionată în funcție de variabile de stare (roundEnded, startRound, startScoreCommunication), permițând sincronizarea între microcontrolerul master și cel slave. Atunci cand primeste 'A' de la master trimite scorul jucatorului, cand primeste 'B' de la master trimite scorul jucatorului 2, iar cand runda s-a terminat trimite 'N'.

```
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
```
