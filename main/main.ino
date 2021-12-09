#include "LedControl.h"
#include <LiquidCrystal.h>

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

const bool modeMenuImage[matrixSize][matrixSize] = {
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 0, 1, 1, 0, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 1, 0, 1, 1, 0, 1, 0},
  {1, 0, 1, 0, 0, 1, 0, 1}
};

const bool aboutMenuImage[matrixSize][matrixSize] = {
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 0, 1, 1, 0, 0, 1},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0}
};

const bool settingsMenuImage[matrixSize][matrixSize] = {
  {1, 1, 0, 0, 0, 0, 1, 1},
  {1, 0, 0, 1, 1, 0, 0, 1},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1},
  {1, 1, 0, 0, 0, 0, 1, 1}
};

const bool scoresMenuImage[matrixSize][matrixSize] = {
  {0, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1}
};


unsigned int long long lastTimeJoystickMove = 0;
bool didJoystickMove = false;

const String gameTitle = "Usu The Game";

byte matrixBrightness = 2;

LiquidCrystal lcd(13, 9, 5, 4, 3, 8);
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER 

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(SwPin, INPUT_PULLUP);

  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinBlue, OUTPUT);

  lcd.begin(lcdOy, lcdOx);
  lcd.setCursor((lcdOy - gameTitle.length()) / 2, 0); 
  lcd.print("Usu The Game");
  
  randomSeed(analogRead(0));
  gameObject.initGame();
  
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  matrix[xPos][yPos] = 1;
}

void loop() {
  buttonObject.doButtons(lc, firstTimeHere);
  
  if (inGame == true){
    lcd.clear();
    if (playing == false) {
      buttonObject.pauseGame(lc); 
        return; 
    }
    
    gameObject.runStep(ledPinBlue, ledPinGreen);

    updateDisplay();

    int timeScoreAccumulation = (millis() / 1000) - (stayedSeconds * 10 / 12);
    if (timeScoreAccumulation < 0) {
      timeScoreAccumulation = 0; 
    }
  
    score = (timeScoreAccumulation + collectedApples * 10) / 10; 

    return;
  }

  //-----------------------------------------------------------------------------------------

  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin); 

  if (xValue < minThreshold) {  
    if (millis() - lastTimeJoystickMove > joystickTime) {
      --curOx;
      --dirOx;
      lastTimeJoystickMove = millis(); 
    }
  }
  
  else if (xValue > maxThreshold) {  
    if (millis() - lastTimeJoystickMove > joystickTime) {
      ++curOx;
      ++dirOx; 
      lastTimeJoystickMove = millis(); 
    }
  }
    
  else if (yValue < minThreshold) {  
    if (millis() - lastTimeJoystickMove > joystickTime) {
      --curOy;
      --dirOy;
      lastTimeJoystickMove = millis(); 
    }
  }
  
  else if (yValue > maxThreshold) {  
   if (millis() - lastTimeJoystickMove > joystickTime) {
      ++curOy;
      ++dirOy;
      lastTimeJoystickMove = millis(); 
    }
  }
  
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
      firstTimeHere = false;
      for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
          lc.setLed(0, row, col, aboutMenuImage[row][col]);
        }
      }
    }
    return; 
  }

  else if (atScores == true) {
    if (firstTimeHere == true) {
      for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
          lc.setLed(0, row, col, scoresMenuImage[row][col]);
        }
      }
      firstTimeHere = false; 
      return; 
    }
    return; 
  }
  
  else if (atSettings == true) {
    if (firstTimeHere == true) {
      for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
          lc.setLed(0, row, col, settingsMenuImage[row][col]);
        }
      }
      firstTimeHere = false; 
      return; 
    }
    return; 
  }

  else if (atSelectName == true) {
    if (firstTimeHere == true) {
      menuObject.doSelectName(lcd);
      for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
          lc.setLed(0, row, col, modeMenuImage[row][col]);
        }
      }
      firstTimeHere = false;
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

void updateDisplay() {
  
  appleObject.blinkApple();
  playerObject.blinkPlayer(); 
  killerObject.blinkKillers(); 
  
  for (int row = xStartDisplay; row <= xEndDisplay; row++) {
    for (int col = yStartDisplay; col <= yEndDisplay; col++) {
      lc.setLed(0, row - xStartDisplay, col - yStartDisplay, matrix[row][col]);
    }
  }
}







  
