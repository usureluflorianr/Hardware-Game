#ifndef BUTTON_H
#define BUTTON_H

#include "menu.h"

#define buttonPinMain A5 
#define buttonPinSW 2
#define ledPinBlue A4
#define ledPinGreen 6
#define universalMatrixSize 8
#define moveSpeedLcdImages 400
#define debounceDelay 50
#define buzzerPin 7
#define buttonSound 1500

bool buttonState = HIGH;
bool previousState = HIGH; 
bool buttonStateSW = HIGH;
bool previousStateSW = HIGH; 
bool playing = false;
bool reading = HIGH;
bool previousReading = HIGH;
bool readingSW = HIGH;
bool previousReadingSW = HIGH;

byte movesPause = 0; 

unsigned int long long lastDebounceTime = 0;
unsigned int long long lastDebounceTimeSW = 0;
unsigned int long long lastPauseOrPromoMove = 0;  

class Button {
  public:

  // function to check if I pushed a button (main or SW)
  bool checkPushed(bool &reading, const byte &buttonPin, unsigned int long long &lastDebounceTime, 
                    bool &previousReading, bool &buttonState) {
    reading = digitalRead(buttonPin); 
      
      if (reading != previousReading) {
        lastDebounceTime = millis(); 
      }
    
      if (millis() - lastDebounceTime > debounceDelay) {
        if (reading != buttonState) {
          buttonState = reading; 
          if (buttonState == HIGH) {
            previousReading = reading;
            if (onVolume == true) {
              tone(buzzerPin, buttonSound, 50);
            }
            return true; 
          }
        }
      }
    
      previousReading = reading;
      return false;
  }

  // do the "USU" move when pausing the game
  void pauseGame(LedControl &lc) {
    if (millis() - lastPauseOrPromoMove > moveSpeedLcdImages) {
      lastPauseOrPromoMove = millis(); 

      bool valueAct = false; 
      for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
          valueAct = false; 
          if ((pauseMatrix[row] & (1 << (universalMatrixSize - 1 - (col + movesPause) % universalMatrixSize))) != 0) {
            valueAct = true; 
          }
          lc.setLed(0, row, col, valueAct);
        }
      }
      
      ++movesPause; 
      if (movesPause == 8) {
        movesPause = 0; 
      }
    }
  }

  // moves the 8x8 leds when in menu or promo
  void promoLeds(LedControl &lc) {
    if (millis() - lastPauseOrPromoMove > moveSpeedLcdImages) {
      lastPauseOrPromoMove = millis(); 

      bool valueAct = false; 
      for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
          valueAct = false; 
          if ((promoMatrix[(row + movesPause) % universalMatrixSize] & (1 << (universalMatrixSize - 1 - (col + movesPause) % universalMatrixSize))) != 0) {
            valueAct = true; 
          }
          lc.setLed(0, row, col, valueAct);
        }
      }
      
      ++movesPause; 
    }
  }

  // check and do the actions for buttons
  void doButtons(LedControl &lc, bool &firstTimeHere, LiquidCrystal &lcd) {
    if (checkPushed(readingSW, (byte) buttonPinSW, lastDebounceTimeSW, previousReadingSW, buttonStateSW)) {
      if (inGame == false) {
        return;
      }
      if (playing == true) {
        movesPause = 0; 
        pauseGame(lc); 
        playing = false; 
      }
      
      else {
        playing = true; 
      }

      return;
    }
    
    if (checkPushed(reading, (byte) buttonPinMain, lastDebounceTime, previousReading, buttonState)) {
      
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
          if (curOy == 0) {
            displayMovementType = 0;
          }
          else {
            displayMovementType = 1;
          }
          
          firstTimeHere = true; 
          atTypeGame = false;
          inGame = true; 
          endedGame = false; 
          playing = true; 
        }
      }

      else if (atScores == true) {
        if (curOy == 0 && curOx == 0) {
           EEPROM.write(0, 0);
           EEPROM.write(1, 0);
           EEPROM.write(2, 0);
           EEPROM.write(3, 0);
           EEPROM.write(4, '-');
           EEPROM.write(5, '-');
           EEPROM.write(6, '-');

           EEPROM.write(7, 0);
           EEPROM.write(8, 0);
           EEPROM.write(9, 0);
           EEPROM.write(10, 0);
           EEPROM.write(11, '-');
           EEPROM.write(12, '-');
           EEPROM.write(13, '-');

           EEPROM.write(14, 0);
           EEPROM.write(15, 0);
           EEPROM.write(16, 0);
           EEPROM.write(17, 0);
           EEPROM.write(18, '-');
           EEPROM.write(19, '-');
           EEPROM.write(20, '-');
        }
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
        if (endedGame == true) {
          atMainMenu = true; 
          inGame = false;
          firstTimeHere = true; 
          return;
        }
        startPowerTime = millis();
        doButtonGame = true;  
        if (numberOfPowers != 0) {
          lcd.setCursor(15, 0);
          lcd.print((byte)(numberOfPowers - 1));
        }
        digitalWrite(ledPinBlue, true);
        lastColorPowerChange = millis();
      }
    }
  }

} buttonObject;

#endif
