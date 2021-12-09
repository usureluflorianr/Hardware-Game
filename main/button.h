#ifndef BUTTON_H
#define BUTTON_H

#include "menu.h"

#define buttonPin A5 
#define SwPin 2
#define ledPinBlue A4
#define ledPinGreen 6
#define universalMatrixSize 8
#define moveSpeedLcdImages 400

bool buttonState = HIGH;
bool previousState = HIGH; 
bool buttonStateSW = HIGH;
bool previousStateSW = HIGH; 
bool playing = false;

int reading = HIGH;
int previousReading = HIGH;
int readingSW = HIGH;
int previousReadingSW = HIGH;
int debounceDelay = 50;

int movesPause = 0; 
int movesPromo = 0; 

unsigned int long long lastDebounceTime = 0;
unsigned int long long lastDebounceTimeSW = 0;
unsigned int long long lastPauseMove = 0; 
unsigned int long long lastPromoMove = 0; 

const bool pauseMatrix[universalMatrixSize][universalMatrixSize] = {
  {1, 0, 1, 0, 0, 0, 0, 0}, 
  {1, 0, 1, 0, 0, 0, 0, 0},
  {1, 0, 1, 0, 0, 0, 0, 0},
  {1, 0, 1, 0, 1, 1, 1, 0},
  {1, 1, 1, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 1, 1, 1, 0},
};

const bool promoMatrix [universalMatrixSize][universalMatrixSize] = {
  {1, 0, 0, 0, 1, 0, 0, 0}, 
  {0, 0, 0, 1, 0, 0, 0, 1},
  {0, 0, 1, 0, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 1, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 1},
  {0, 0, 1, 0, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 1, 0, 0},
};

class Button {
  public:

  bool checkPushed() {
    reading = digitalRead(buttonPin); 
      
      if (reading != previousReading) {
        lastDebounceTime = millis(); 
      }
    
      if (millis() - lastDebounceTime > debounceDelay) {
        if (reading != buttonState) {
          buttonState = reading; 
          if (buttonState == HIGH) {
            previousReading = reading;
            return true; 
          }
        }
      }
    
      previousReading = reading;
      return false;
  }

  bool checkSwPushed() {
    readingSW = digitalRead(SwPin); 
      
      if (readingSW != previousReadingSW) {
        lastDebounceTimeSW = millis(); 
      }
    
      if (millis() - lastDebounceTimeSW > debounceDelay) {
        if (readingSW != buttonStateSW) {
          buttonStateSW = readingSW; 
          if (buttonStateSW == HIGH) {
            previousReadingSW = readingSW;
            return true; 
          }
        }
      }
    
      previousReadingSW = readingSW;
      return false;
  }

  void pauseGame(LedControl &lc) {
    if (millis() - lastPauseMove > moveSpeedLcdImages) {
      lastPauseMove = millis(); 
    
      for (int row = 0; row < universalMatrixSize; row++) {
        for (int col = 0; col < universalMatrixSize; col++) {
          lc.setLed(0, row, col, pauseMatrix[row][(col + movesPause) % universalMatrixSize]);
        }
      }
      
      ++movesPause; 
    }
  }

  void promoLeds(LedControl &lc) {
    if (millis() - lastPromoMove > moveSpeedLcdImages) {
      lastPromoMove = millis(); 
    
      for (int row = 0; row < universalMatrixSize; row++) {
        for (int col = 0; col < universalMatrixSize; col++) {
          lc.setLed(0, row, col, promoMatrix[(row + movesPause) % universalMatrixSize][(col + movesPause) % universalMatrixSize]);
        }
      }
      
      ++movesPause; 
    }
  }

  void doButtons(LedControl &lc, bool &firstTimeHere) {
    if (checkSwPushed()) {
      if (playing == true) {
        movesPause = 0; 
        pauseGame(lc); 
        playing = false; 
        return;
      }
      else {
        playing = true; 
        return;
      }
    }
  
    if (checkPushed()) {
      
      if (atPromo == true) {
        atPromo = false; 
        atMainMenu = true;
        firstTimeHere = true;  
        return;
      }

      else if (atMainMenu == true) {
        atMainMenu = false;
        if (curOy == 0) { 
          firstTimeHere = true;
          atSelectName = true;
          dirOx = 0; 
          dirOy = 0;
          nameChar1 = 'U';
          nameChar2 = 'S';
          nameChar3 = 'U'; 
        }

        else if (curOy == 1) {
          firstTimeHere = true;
          atScores = true;
        }
          
        else if (curOy == 2) {
          firstTimeHere = true;
          atSettings = true; 
        }

        else if (curOy == 3) {
          firstTimeHere = true;
          atAbout = true;
        }
        
      }

      else if (atAbout == true) {
        firstTimeHere = true; 
        atAbout = false; 
        atMainMenu = true; 
      }

      else if (atSelectName == true) {
        if (curOy == 3 && curOx == 0) {
          firstTimeHere = true; 
          atMainMenu = true; 
          atSelectName = false; 
        }
        
        if (curOy == 3 && curOx == 1) {
          atTypeGame = true;
          atSelectName = false; 
          firstTimeHere = true;  
        }
      }

      else if (atTypeGame == true) {
        if (curOx == 0) {
          firstTimeHere = true; 
          atSelectName = true;
          atTypeGame = false;
        }
        else {
          firstTimeHere = true; 
          inGame = true; 
          playing = true;
          atTypeGame = false;
        }
      }

      else if (atScores == true) {
        atMainMenu = true; 
        atScores = false; 
        firstTimeHere = true; 
      }

      else if (atSettings == true) {
        atMainMenu = true; 
        atSettings = false; 
        firstTimeHere = true; 
      }
      
      else if (inGame == true && playing == true) {
        startPowerTime = millis();
        doButtonGame = true; 
        digitalWrite(ledPinBlue, true);
        lastColorPowerChange = millis();
      }
    }
  }

} buttonObject;

#endif
