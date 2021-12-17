#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>

#include "game.h"
#include "button.h"

// connect lcd
// https://www.youtube.com/watch?v=Mr9FQKcrGpA

#define dinPin 12
#define clockPin 11
#define loadPin 10
#define lcdOx 2 
#define lcdOy 16
#define joystickTime 500
#define moveSound 5000
#define pinForRandom 0
#define PinRS 13
#define PinE 9
#define D4 A2
#define D5 4
#define D6 A3
#define D7 8

/* 
 *  For EEPROM:
 *  - from 0 to 11, we keep the details for highscore (name and score for top 3)
 *  - each of these is being keept in buckets of 4 EEPROM bytes
 *  - 1st byte from the bucket is the score and the following 3 are for each char in the name  
 */

unsigned int long long lastTimeJoystickMove = 0;
bool didJoystickMove = false;

const String gameTitle = "Usu The Game";

LiquidCrystal lcd(PinRS, PinE, D4, D5, D6, D7);
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER 

void setup() {
  
  Serial.begin(9600);

  pinMode(buzzerPin, OUTPUT);
  
  pinMode(buttonPinMain, INPUT_PULLUP);
  pinMode(buttonPinSW, INPUT_PULLUP);

  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinBlue, OUTPUT);

  lcd.begin(lcdOy, lcdOx);
  lcd.setCursor((lcdOy - gameTitle.length()) / 2, 0); 
  lcd.print("Usu The Game");
  
  randomSeed(analogRead(pinForRandom));
  gameObject.initGame();
  
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, lvlLeds * ledsIncrement); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  
  
  change(xPos, yPos, 1);
  
  lvlBright = EEPROM.read(eprByteBright);
  lvlContrast = EEPROM.read(eprByteContrast); 
  lvlLeds = EEPROM.read(eprByteLeds); 
  onVolume = (1 == EEPROM.read(eprByteVolume));

  analogWrite(contrastPin, lvlContrast * contrastIncrement);
  analogWrite(backLedPin, lvlBright * brightIncrement);
}

void loop() {
  Serial.println(curOx);
 
  buttonObject.doButtons(lc, firstTimeHere, lcd);

  hereInGame(); 
  
  hereMoveJoyStick();

  hereAtMenu(); 
}

void updateDisplay() {
  
  appleObject.blinkApple();
  playerObject.blinkPlayer(); 
  killerObject.blinkKillers(); 

  bool state = false;
  
  for (byte row = xStartDisplay; row <= xEndDisplay; row++) {
    for (byte col = yStartDisplay; col <= yEndDisplay; col++) {
      if ((matrix[row] & (1ll << col)) != 0) {
        state = true;
      }
      else {
        state = false; 
      }
      
      lc.setLed(0, row - xStartDisplay, col - yStartDisplay, state);
    }
  }
}

void updateHighscore(int &score) {

  bool existed = false;
  int scoreEpr = 0;
  if (nameChar1 == EEPROM.read(1) && nameChar2 == EEPROM.read(2) && nameChar3 == EEPROM.read(3)) {
    scoreEpr = EEPROM.read(0);
    if (score > scoreEpr) {
      EEPROM.update(0, score);
    }
    else return;
    existed = true; 
  }

  else if (nameChar1 == EEPROM.read(5) && nameChar2 == EEPROM.read(6) && nameChar3 == EEPROM.read(7)) {
    scoreEpr = EEPROM.read(4);
    if (score > scoreEpr) {
      EEPROM.update(4, score);
    }
    else return;
    existed = true; 
  }

  else if (nameChar1 == EEPROM.read(9) && nameChar2 == EEPROM.read(10) && nameChar3 == EEPROM.read(11)) {
    scoreEpr = EEPROM.read(8);
    if (score > scoreEpr) {
      EEPROM.update(8, score);
    }
    else return;
    existed = true; 
  }

  scoreEpr = EEPROM.read(8);
  if (score > scoreEpr && existed == false) {
      EEPROM.update(8, score);
      EEPROM.update(9, nameChar1);
      EEPROM.update(10, nameChar2); 
      EEPROM.update(11, nameChar3);  
  }

  int scoreEprHigh = 0;

  scoreEpr = EEPROM.read(8);
  scoreEprHigh = EEPROM.read(4);

  if (scoreEpr > scoreEprHigh) {
    EEPROM.update(8, scoreEprHigh);
    EEPROM.update(4, scoreEpr);
  
    nameChar1 = EEPROM.read(9);
    nameChar2 = EEPROM.read(10);
    nameChar3 = EEPROM.read(11);

    EEPROM.update(9, EEPROM.read(5));
    EEPROM.update(10, EEPROM.read(6));
    EEPROM.update(11, EEPROM.read(7));

    EEPROM.update(5, nameChar1);
    EEPROM.update(6, nameChar2);
    EEPROM.update(7, nameChar3);
  }

  scoreEpr = EEPROM.read(4);
  scoreEprHigh = EEPROM.read(0);

  if (scoreEpr > scoreEprHigh) {
    EEPROM.update(4, scoreEprHigh);
    EEPROM.update(0, scoreEpr);
  
    nameChar1 = EEPROM.read(5);
    nameChar2 = EEPROM.read(6);
    nameChar3 = EEPROM.read(7);

    EEPROM.update(5, EEPROM.read(1));
    EEPROM.update(6, EEPROM.read(2));
    EEPROM.update(7, EEPROM.read(3));

    EEPROM.update(1, nameChar1);
    EEPROM.update(2, nameChar2);
    EEPROM.update(3, nameChar3);
  }
}

//----------------------------------------------------------------------------------------
//-inGame---------------------------------------------------------------------------------

void hereInGame() {
  if (inGame == true){

    if (endedGame == true) {
      
      if (firstTimeHere == true) {
        bool valueAct = false;
      
        for (int row = 0; row < matrixSize; row++) {
          for (int col = 0; col < matrixSize; col++) {
            valueAct = false; 
            if ((endedGameImage[row] & (1 << col)) != 0) {
              valueAct = true; 
            }
            lc.setLed(0, row, col, valueAct);
          }  
        }
        
        firstTimeHere = false; 
        lcd.setCursor(0, 0);
        lcd.print("Great job, ");
        lcd.print(nameChar1);
        lcd.print(nameChar2);
        lcd.print(nameChar3);
        lcd.print("! "); 

        if (score > EEPROM.read(0)) {
          lcd.setCursor(0, 0);
          lcd.print("                ");
          lcd.setCursor(0, 0);
          lcd.print(" New highscore! ");
        }

        updateHighscore(score); 
      }

      return; 
    }
    
    if (playing == false) {
      buttonObject.pauseGame(lc); 
        return; 
    }

    if (firstTimeHere == true) {
      gameObject.initGame();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Go, ");
      lcd.print(nameChar1);
      lcd.print(nameChar2);
      lcd.print(nameChar3);
      lcd.print("!");
      lcd.setCursor(14, 0);
      lcd.write(7);
      lcd.print(2);
      lcd.setCursor(11, 0);
      lcd.write(6);
      lcd.print(2);
      lcd.setCursor(2, 1);
      lcd.print("SCORE: 0000");
      playerObject.updatePlayerOnDisplay(xPos, yPos);
      firstTimeHere = false; 
    }
    updateDisplay();
    
    gameObject.runStep(ledPinBlue, ledPinGreen, lcd, firstTimeHere, onVolume);

    int timeScoreAccumulation = ((millis() - whenStartedGame) / 1000) - (stayedSeconds * 10 / 12);
    if (timeScoreAccumulation < 0) {
      timeScoreAccumulation = 0; 
    }
  
    score = (timeScoreAccumulation + collectedApples * 10) / 10; 

    if (score != prevScore) {
      prevScore = score; 
      lcd.setCursor(9, 1);
      byte digit1 = score % 10; 
      byte digit2 = score / 10 % 10;
      byte digit3 = score / 100 % 10; 
      byte digit4 = score / 1000 % 10; 
      lcd.print(digit4);
      lcd.print(digit3);
      lcd.print(digit2);
      lcd.print(digit1);
    }

    return;
  }
}

//----------------------------------------------------------------------------------------
//-change positions using the joystick----------------------------------------------------

void hereMoveJoyStick() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin); 

  if (xValue < minThreshold) {  
    if (millis() - lastTimeJoystickMove > joystickTime) {
      if (onVolume == true && inGame == false) {
        tone(buzzerPin, moveSound, toneTime);
      }
      --curOx;
      --dirOx;
      lastTimeJoystickMove = millis(); 
    }
  }
  
  else if (xValue > maxThreshold) {  
    if (millis() - lastTimeJoystickMove > joystickTime) {
      if (onVolume == true && inGame == false) {
        tone(buzzerPin, moveSound, toneTime);
      }
      ++curOx;
      ++dirOx; 
      lastTimeJoystickMove = millis(); 
    }
  }
    
  else if (yValue < minThreshold) {  
    if (millis() - lastTimeJoystickMove > joystickTime) {
      if (onVolume == true && inGame == false) {
        tone(buzzerPin, moveSound, toneTime);
      }
      --curOy;
      --dirOy;
      lastTimeJoystickMove = millis(); 
    }
  }
  
  else if (yValue > maxThreshold) {  
   if (millis() - lastTimeJoystickMove > joystickTime) {
      if (onVolume == true && inGame == false) {
        tone(buzzerPin, moveSound, toneTime);
      }
      ++curOy;
      ++dirOy;
      lastTimeJoystickMove = millis(); 
    }
  }
}

//----------------------------------------------------------------------------------------
//-change positions using the joystick----------------------------------------------------

void hereAtMenu() {
  if (atPromo == true) {
    if (millis() - lastTimeScrolledText > scrollInterval) {
      lastTimeScrolledText = millis(); 
      atPromo = menuObject.doPromo(lcd, actualPozPromo, promoText);
      atMainMenu = atPromo xor 1;  
    }
     
    buttonObject.promoLeds(lc);
    return; 
  }

  else if (atMainMenu == true) {
    menuObject.doMainMenu(lcd);
    buttonObject.promoLeds(lc);
    if (firstTimeHere == true) {
      digitalWrite(ledPinBlue, false);
      analogWrite(ledPinGreen, minValueLed);
      menuObject.init(lcd);
      firstTimeHere = false; 
    } 
    return; 
  }

  else if (atAbout == true) {
    if (firstTimeHere == true) {
      menuObject.doAbout(lcd); 
      printMatrix(aboutMenuImage);
      return;
    }
    if (millis() - lastTimeScrolledText > scrollInterval) {
      lastTimeScrolledText = millis(); 
      menuObject.doPromo(lcd, actualPozAbout, aboutText);
      actualPozAbout = actualPozAbout % aboutText.length();
    }
    return; 
  }

  else if (atScores == true) {
    if (firstTimeHere == true) {
      menuObject.doHighscore(lcd);
      printMatrix(scoresMenuImage);
      return; 
    } 
    menuObject.doHighscore(lcd);
    return; 
  }
  
  else if (atSettings == true) {
    if (firstTimeHere == true) {
      menuObject.doSettings(lcd, lc);
      printMatrix(settingsMenuImage);
      return; 
    }
    menuObject.doSettings(lcd, lc);
    return; 
  }

  else if (atSelectName == true) {
    if (firstTimeHere == true) {
      menuObject.doSelectName(lcd);
      printMatrix(modeMenuImage);
      return;
    }
    menuObject.doSelectName(lcd);
    return; 
  }

  else if (atTypeGame == true) {
    if (firstTimeHere == true) {
      menuObject.doTypeGame(lcd, displayMovementType); 
      firstTimeHere = false; 
      return;
    }
    menuObject.doTypeGame(lcd, displayMovementType);
    return; 
  }
}

void printMatrix(byte toPrint[]) {
  bool valueAct = false; 
  firstTimeHere = false;
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      valueAct = false; 
  
      if ((toPrint[row] & (1 << col)) != 0) {
        valueAct = true; 
      }
      
      lc.setLed(0, row, col, valueAct);
    }
  }
}





  
