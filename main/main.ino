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

unsigned int long long lastTimeJoystickMove = 0;
bool didJoystickMove = false;

const String gameTitle = "Usu The Game";

LiquidCrystal lcd(13, 9, A2, 4, A3, 8);
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
  
  randomSeed(analogRead(0));
  gameObject.initGame();
  
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, lvlLeds * ledsIncrement); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  
  
  change(xPos, yPos, 1);

  analogWrite(contrastPin, lvlContrast * contrastIncrement);
  analogWrite(backLedPin, lvlBright * brightIncrement);
}

void loop() {
 
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
  if (nameChar1 == EEPROM.read(4) && nameChar2 == EEPROM.read(5) && nameChar3 == EEPROM.read(6)) {
    scoreEpr = EEPROM.read(0) * 1000 + EEPROM.read(1) * 100 + EEPROM.read(2) * 10 + EEPROM.read(3);
    if (score > scoreEpr) {
      EEPROM.write(0, score / 1000 % 10);
      EEPROM.write(1, score / 100 % 10);
      EEPROM.write(2, score / 10 % 10);
      EEPROM.write(3, score % 10);
    }
    else return;
    existed = true; 
  }

  else if (nameChar1 == EEPROM.read(11) && nameChar2 == EEPROM.read(12) && nameChar3 == EEPROM.read(13)) {
    scoreEpr = EEPROM.read(7) * 1000 + EEPROM.read(8) * 100 + EEPROM.read(9) * 10 + EEPROM.read(10);
    if (score > scoreEpr) {
      EEPROM.write(7, score / 1000 % 10);
      EEPROM.write(8, score / 100 % 10);
      EEPROM.write(9, score / 10 % 10);
      EEPROM.write(10, score % 10);
    }
    else return;
    existed = true; 
  }

  else if (nameChar1 == EEPROM.read(18) && nameChar2 == EEPROM.read(19) && nameChar3 == EEPROM.read(20)) {
    scoreEpr = EEPROM.read(14) * 1000 + EEPROM.read(15) * 100 + EEPROM.read(16) * 10 + EEPROM.read(17);
    if (score > scoreEpr) {
      EEPROM.write(14, score / 1000 % 10);
      EEPROM.write(15, score / 100 % 10);
      EEPROM.write(16, score / 10 % 10);
      EEPROM.write(17, score % 10);
    }
    else return;
    existed = true; 
  }

  scoreEpr = EEPROM.read(14) * 1000 + EEPROM.read(15) * 100 + EEPROM.read(16) * 10 + EEPROM.read(17);
  if (score > scoreEpr && existed == false) {
      EEPROM.write(14, score / 1000 % 10);
      EEPROM.write(15, score / 100 % 10);
      EEPROM.write(16, score / 10 % 10);
      EEPROM.write(17, score % 10);

      EEPROM.write(18, nameChar1);
      EEPROM.write(19, nameChar2); 
      EEPROM.write(20, nameChar3);  
  }

  int scoreEprHigh = 0;

  scoreEpr = EEPROM.read(14) * 1000 + EEPROM.read(15) * 100 + EEPROM.read(16) * 10 + EEPROM.read(17);
  scoreEprHigh = EEPROM.read(7) * 1000 + EEPROM.read(8) * 100 + EEPROM.read(9) * 10 + EEPROM.read(10);

  if (scoreEpr > scoreEprHigh) {
    EEPROM.write(14, scoreEprHigh / 1000 % 10);
    EEPROM.write(15, scoreEprHigh / 100 % 10);
    EEPROM.write(16, scoreEprHigh / 10 % 10);
    EEPROM.write(17, scoreEprHigh % 10);

    EEPROM.write(7, scoreEpr / 1000 % 10);
    EEPROM.write(8, scoreEpr / 100 % 10);
    EEPROM.write(9, scoreEpr / 10 % 10);
    EEPROM.write(10, scoreEpr % 10);
  
    nameChar1 = EEPROM.read(18);
    nameChar2 = EEPROM.read(19);
    nameChar3 = EEPROM.read(20);

    EEPROM.write(18, EEPROM.read(11));
    EEPROM.write(19, EEPROM.read(12));
    EEPROM.write(20, EEPROM.read(13));

    EEPROM.write(11, nameChar1);
    EEPROM.write(12, nameChar2);
    EEPROM.write(13, nameChar3);
  }

  scoreEpr = EEPROM.read(7) * 1000 + EEPROM.read(8) * 100 + EEPROM.read(9) * 10 + EEPROM.read(10);
  scoreEprHigh = EEPROM.read(0) * 1000 + EEPROM.read(1) * 100 + EEPROM.read(2) * 10 + EEPROM.read(3);

  if (scoreEpr > scoreEprHigh) {
    EEPROM.write(7, scoreEprHigh / 1000 % 10);
    EEPROM.write(8, scoreEprHigh / 100 % 10);
    EEPROM.write(9, scoreEprHigh / 10 % 10);
    EEPROM.write(10, scoreEprHigh % 10);

    EEPROM.write(0, scoreEpr / 1000 % 10);
    EEPROM.write(1, scoreEpr / 100 % 10);
    EEPROM.write(2, scoreEpr / 10 % 10);
    EEPROM.write(3, scoreEpr % 10);
  
    nameChar1 = EEPROM.read(11);
    nameChar2 = EEPROM.read(12);
    nameChar3 = EEPROM.read(13);

    EEPROM.write(11, EEPROM.read(4));
    EEPROM.write(12, EEPROM.read(5));
    EEPROM.write(13, EEPROM.read(6));

    EEPROM.write(4, nameChar1);
    EEPROM.write(5, nameChar2);
    EEPROM.write(6, nameChar3);
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

        if (score == EEPROM.read(0) * 1000 + EEPROM.read(1) * 100 + EEPROM.read(2) * 10 + EEPROM.read(3)) {
          if (nameChar1 == EEPROM.read(4) && nameChar2 == EEPROM.read(5) && nameChar3 == EEPROM.read(6)) {
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print(" New highscore! ");
          }
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
        tone(buzzerPin, moveSound, 50);
      }
      --curOx;
      --dirOx;
      lastTimeJoystickMove = millis(); 
    }
  }
  
  else if (xValue > maxThreshold) {  
    if (millis() - lastTimeJoystickMove > joystickTime) {
      if (onVolume == true && inGame == false) {
        tone(buzzerPin, moveSound, 50);
      }
      ++curOx;
      ++dirOx; 
      lastTimeJoystickMove = millis(); 
    }
  }
    
  else if (yValue < minThreshold) {  
    if (millis() - lastTimeJoystickMove > joystickTime) {
      if (onVolume == true && inGame == false) {
        tone(buzzerPin, moveSound, 50);
      }
      --curOy;
      --dirOy;
      lastTimeJoystickMove = millis(); 
    }
  }
  
  else if (yValue > maxThreshold) {  
   if (millis() - lastTimeJoystickMove > joystickTime) {
      if (onVolume == true && inGame == false) {
        tone(buzzerPin, moveSound, 50);
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
      atPromo = menuObject.doPromo(lcd);
      atMainMenu = atPromo xor 1;  
    }
     
    buttonObject.promoLeds(lc);
    return; 
  }

  else if (atMainMenu == true) {
    menuObject.doMainMenu(lcd);
    buttonObject.promoLeds(lc);
    if (firstTimeHere == true) {
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





  
