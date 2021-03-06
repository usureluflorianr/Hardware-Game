#ifndef MENU_H
#define MENU_H

#include "matrix_objects.h"

#define lastEndLCD 16
#define scrollInterval 700
#define backLedPin 5
#define contrastPin 3
#define contrastIncrement 33
#define brightIncrement 50
#define ledsIncrement 3
#define buzzerPin 7
#define promoOddTimeSound 500
#define promoEvenTimeSound 1000
#define settingsOxModulo 4
#define highscoreOyModulo 3 
#define highscoreOxModulo 2
#define typeGameOxModulo 2
#define typeGameOyModulo 2
#define selectNameOxModulo 2
#define selectNameOyModulo 4
#define menuOxModulo 2
#define menuOyModulo 4
#define toneTime 50
#define eprByteBright 12
#define eprByteContrast 13
#define eprByteLeds 14
#define eprByteVolume 15


byte actualPozPromo = 0;   
byte actualPozAbout = 0; 
short dirOx = 0;
short dirOy = 0; 

bool atPromo = true;
bool inGame = false;
bool atMainMenu = false;
bool atAbout = false; 
bool atScores = false; 
bool atTypeGame = false;
bool atSettings = false;
bool firstTimeHere = true;
bool onVolume = true;

int curOx = 1; 
int curOy = 0; 

byte lvlBright = 4;
byte lvlContrast = 3; 
byte lvlLeds = 2; 

char nameChar1 = 'U';
char nameChar2 = 'S';
char nameChar3 = 'U';

unsigned int long long lastTimeScrolledText = 0;

const String promoText = "Story: life is short. Collect stories to be happy and as fast as you can, because walls will appear and many to stop you!.....";
const String aboutText = "GitHub: usureluflorianr ";

class Menu {
  public:

  void init(LiquidCrystal &lcd) {
    lcd.createChar(1, arrowRight);
    lcd.createChar(2, arrowLeft);
    lcd.createChar(3, arrowUp);
    lcd.createChar(4, arrowDown);
    lcd.createChar(5, circle);
    lcd.createChar(6, heart);
    lcd.createChar(7, power);
    lcd.createChar(8, fullRect);
  }

  bool doPromo(LiquidCrystal &lcd, byte &actualPoz, String textToShow) {
    lcd.setCursor(0, 1);
    lcd.print(textToShow.substring(actualPoz, actualPoz + lastEndLCD));
    
    actualPoz++;

    if (onVolume == true) {
      if (actualPoz % 2) {
        tone(buzzerPin, promoOddTimeSound, toneTime);
      }

      else {
        tone(buzzerPin, promoEvenTimeSound, toneTime);
      }
    }
    
    if ((actualPoz + lastEndLCD - 1) == (textToShow.length() - 1)) {
      return false;
    }
    return true;
  }

  void doMainMenu(LiquidCrystal &lcd) {
    if (firstTimeHere == true) {
      curOx = 1; 
      curOy = 0;
      lcd.clear(); 
      lcd.setCursor(1, 0); 
      lcd.print("MENU:");
      lcd.setCursor(14, 1);
      lcd.print("GO");
      lcd.setCursor(1, 1);
      lcd.write(2);
      lcd.setCursor(2, 1);
      lcd.write(1);
      lcd.setCursor(6, 1);
      lcd.print("Play");
      return;
    }

    if (curOx == -1) {
      curOx = menuOxModulo - 1; 
    }
    curOx %= menuOxModulo;
    if (curOy == -1) {
      curOy = menuOyModulo - 1; 
    }
    curOy %= menuOyModulo; 

    if (curOy == 0) {
      lcd.setCursor(4, 1);
      lcd.print("  Play   ");
    }

    else if (curOy == 1) {
      lcd.setCursor(4, 1); 
      lcd.print("Highscore");
    }

    else if (curOy == 2) {
      lcd.setCursor(4, 1); 
      lcd.print("Settings "); 
    }

    else if (curOy == 3) {
      lcd.setCursor(4, 1);
      lcd.print("  About  ");
    }
  }

  void doAbout(LiquidCrystal &lcd) {
    if (firstTimeHere == true) {
      firstTimeHere = false; 
      lcd.clear();
      lcd.setCursor(1, 0); 
      lcd.print("ABOUT:");
      lcd.setCursor(12, 0);
      lcd.print("BACK");
      lcd.setCursor(1, 1);
      actualPozPromo = 0; 
      //lcd.print("GIT: usureluflorianr");
    }
  }

  void doSelectName(LiquidCrystal &lcd) {

    if (curOx == -1) {
      curOx = selectNameOxModulo - 1; 
    }
    curOx %= selectNameOxModulo;
    if (curOy == -1) {
      curOy = selectNameOyModulo - 1; 
    }
    curOy %= selectNameOyModulo;
    
    if (firstTimeHere == true) {
      curOx = 1; 
      curOy = 3;
      dirOx = 0;
      dirOy = 0; 
      nameChar1 = 'U';
      nameChar2 = 'S';
      nameChar3 = 'U';
      lcd.clear(); 
      lcd.setCursor(14, 1);
      lcd.print("GO");
      lcd.setCursor(0, 0);
      lcd.write(3);
      lcd.setCursor(0, 1);
      lcd.write(4);
      lcd.setCursor(1, 1);
      lcd.write(2);
      lcd.setCursor(2, 1);
      lcd.write(1);
      lcd.setCursor(12, 0);
      lcd.print("back");
      lcd.setCursor(3, 1);
      lcd.print("    USU    ");
      lcd.setCursor(1, 0);
      lcd.print("NAME:");
      return; 
    }

    if (curOy == 0) {
      if (dirOy != 0) {
        dirOy = 0;
        
        lcd.setCursor(7, 0); 
        lcd.print("   ");
        lcd.setCursor(7, 0);
        lcd.write(4);
      }
    }

    else if (curOy == 1) {
      if (dirOy != 0) {
        dirOy = 0;
      
        lcd.setCursor(7, 0); 
        lcd.print("   ");
        lcd.setCursor(8, 0);
        lcd.write(4);
      }
    }

    else if (curOy == 2) {
      if (dirOy != 0) {
        dirOy = 0;
      
        lcd.setCursor(7, 0); 
        lcd.print("   ");
        lcd.setCursor(9, 0);
        lcd.write(4);
      }
    }

    else if (curOy == 3 && dirOy != 0) {
      dirOy = 0;
      curOx = 1; 
      lcd.setCursor(14, 1);
      lcd.print("GO");
      lcd.setCursor(12, 0);
      lcd.print("back");
      lcd.setCursor(7, 0);
      lcd.print("   ");    
    }


    if (dirOx != 0) {
      if (curOy == 3) {
        dirOx = 0; 
        if (curOx == 0) {
          lcd.setCursor(12, 0);
          lcd.print("BACK");
          
          lcd.setCursor(14, 1);
          lcd.print("go");
        }
        else {
          lcd.setCursor(12, 0);
          lcd.print("back");
          
          lcd.setCursor(14, 1);
          lcd.print("GO");   
        }
      }

      else if (curOy == 0) {
        if (dirOx == 1) {
          if (nameChar1 == 'Z') {
            nameChar1 = 'A';
          }
          else {
            nameChar1++;    
          }
        }
        
        else if (dirOx == -1) {
          if (nameChar1 == 'A') {
            nameChar1 = 'Z'; 
          }
          else {
            nameChar1--;
          }
        }
        dirOx = 0; 
        lcd.setCursor(7, 1); 
        lcd.print(nameChar1);
      }

      else if (curOy == 1) {
        if (dirOx == 1) {
          if (nameChar2 == 'Z') {
            nameChar2 = 'A';
          }
          else {
            nameChar2++;    
          }
        }
        
        else if (dirOx == -1) {
          if (nameChar2 == 'A') {
            nameChar2 = 'Z'; 
          }
          else {
            nameChar2--;
          }
        }
        dirOx = 0; 
        lcd.setCursor(8, 1); 
        lcd.print(nameChar2);
      }

      else if (curOy == 2) {
        if (dirOx == 1) {
          if (nameChar3 == 'Z') {
            nameChar3 = 'A';
          }
          else {
            nameChar3++;    
          }
        }
        
        else if (dirOx == -1) {
          if (nameChar3 == 'A') {
            nameChar3 = 'Z'; 
          }
          else {
            nameChar3--;
          }
        }
        dirOx = 0; 
        lcd.setCursor(9, 1); 
        lcd.print(nameChar3);
      }
      
    }
  }

  void printScoresFromEEPROM(LiquidCrystal &lcd, byte startPoz) {
    lcd.setCursor(4, 1);
    
    if (startPoz == 0)
      lcd.print(1); 
    else if (startPoz == 4) 
      lcd.print(2);
    else 
      lcd.print(3); 
    
    lcd.print(": ");
    lcd.print((char)EEPROM.read(startPoz + 1));
    lcd.print((char)EEPROM.read(startPoz + 2));
    lcd.print((char)EEPROM.read(startPoz + 3));
    lcd.print(" "); 
    byte topScore = EEPROM.read(startPoz); 

    // we extract each digit of the highscore with leading 0s
    lcd.print(topScore / 100 % 10);
    lcd.print(topScore / 10 % 10);
    lcd.print(topScore  % 10);
  }

  void doTypeGame(LiquidCrystal &lcd, byte &displayMovementType) {

    if (curOx == -1) {
      curOx = typeGameOxModulo - 1; 
    }
    curOx %= typeGameOxModulo;
    if (curOy == -1) {
      curOy = typeGameOyModulo - 1; 
    }
    curOy %= typeGameOyModulo;
    
    if (firstTimeHere == true) {
      curOx = 1; 
      curOy = 0; 
      lcd.clear(); 
      lcd.setCursor(14, 1);
      lcd.print("GO");
      lcd.setCursor(0, 0);
      lcd.write(3);
      lcd.setCursor(0, 1);
      lcd.write(4);
      lcd.setCursor(1, 1);
      lcd.write(2);
      lcd.setCursor(2, 1);
      lcd.write(1);
      lcd.setCursor(12, 0);
      lcd.print("back");
      lcd.setCursor(3, 1);
      lcd.print("   ROOMS   ");
      lcd.setCursor(1, 0);
      lcd.print("GAME MODE:");
      firstTimeHere = false;
      return; 
    }

    if (dirOx != 0) {
      dirOx = 0; 
      if (curOx == 0) {
        lcd.setCursor(12, 0);
        lcd.print("BACK");
          
        lcd.setCursor(14, 1);
        lcd.print("go");
      }
      else {
        lcd.setCursor(12, 0);
        lcd.print("back");
          
        lcd.setCursor(14, 1);
        lcd.print("GO");   
      }
    }

    if (dirOy != 0) {
      dirOy = 0;

      if (curOy == 0) {
        lcd.setCursor(3, 1);
        lcd.print("   ROOMS   ");
      }
      else {
        lcd.setCursor(3, 1);      
        lcd.print("  FOLLOW   ");
      }
    }
  }

  void doHighscore(LiquidCrystal &lcd) {
    
    if (curOy == -1) {
      curOy = highscoreOyModulo - 1; 
    }
    curOy %= highscoreOyModulo;
    if (curOx == -1) {
      curOx = highscoreOxModulo - 1; 
    }
    curOx %= highscoreOxModulo;

    if (firstTimeHere == true) {
      curOy = 0; 
      lcd.clear(); 
      lcd.setCursor(12, 0);
      lcd.print("BACK");
      lcd.setCursor(0, 0);
      lcd.write(3);
      lcd.setCursor(0, 1);
      lcd.write(4);
      lcd.setCursor(1, 1);
      lcd.write(2);
      lcd.setCursor(2, 1);
      lcd.write(1);
      lcd.setCursor(1, 0);
      lcd.print("clear-all");
      firstTimeHere = false;         
      printScoresFromEEPROM(lcd, 0);
      return; 
    }

    if (dirOy != 0) {
      dirOy = 0;

      if (curOx == 1) {
        if (curOy == 0) {
          printScoresFromEEPROM(lcd, 0);
        }
        else if (curOy == 1) {
          printScoresFromEEPROM(lcd, 4);
        }
        else {
          printScoresFromEEPROM(lcd, 8);
        }
      }

      if (curOx == 0) {
        if (curOy == 2) {
          curOy = 1; 
        }
        if (curOy == 0) {
          lcd.setCursor(1, 0);
          lcd.print("CLEAR-ALL");
          lcd.setCursor(12, 0);
          lcd.print("back");
        }
        else if (curOy == 1) {
          lcd.setCursor(1, 0);
          lcd.print("clear-all");
          lcd.setCursor(12, 0);
          lcd.print("BACK");
        }
      }
    }
  }

  void doSettings(LiquidCrystal &lcd, LedControl &lc) {
    if (firstTimeHere == true) {
      curOx = 0;
      dirOx = 0;
      dirOy = 0;
      lcd.clear(); 
      lcd.setCursor(13, 1);
      lcd.print("SET");
      lcd.setCursor(0, 0);
      lcd.write(3);
      lcd.setCursor(0, 1);
      lcd.write(4);
      lcd.setCursor(1, 1);
      lcd.write(2);
      lcd.setCursor(2, 1);
      lcd.write(1);
      lcd.setCursor(3, 1);
      lcd.print(" Bright. ");
      lcd.setCursor(4, 0);
      lcd.print("<");
      for (byte i = 0; i < lvlBright; ++i) {
        lcd.write(8);
      }
      lcd.setCursor(10, 0);
      lcd.print(">"); 
      firstTimeHere = false; 
    }

    if (dirOy != 0) {
      if (curOx == 0) {
        if (dirOy == -1) {
          lvlBright = max(0, lvlBright - 1);
        }
        else {
          lvlBright = min(5, lvlBright + 1); 
        }
        EEPROM.update(eprByteBright, lvlBright);
        analogWrite(backLedPin, lvlBright * brightIncrement);
        lcd.setCursor(5, 0);
        lcd.print("     ");
        lcd.setCursor(5, 0);
        for (byte i = 0; i < lvlBright; ++i) {
          lcd.write(8);
        }
      }

      else if (curOx == 1) {
        if (dirOy == -1) {
          lvlContrast = max(0, lvlContrast - 1);
        }
        else {
          lvlContrast = min(5, lvlContrast + 1); 
        }
        EEPROM.update(eprByteContrast, lvlContrast);
        analogWrite(contrastPin, lvlContrast * contrastIncrement);
        lcd.setCursor(5, 0);
        lcd.print("     ");
        lcd.setCursor(5, 0);
        for (byte i = 0; i < lvlContrast; ++i) {
          lcd.write(8);
        }
      }

      else if (curOx == 2) {
        if (dirOy == -1) {
          lvlLeds = max(0, lvlLeds - 1);
        }
        else {
          lvlLeds = min(5, lvlLeds + 1); 
        }
        EEPROM.update(eprByteLeds, lvlLeds);
        lc.setIntensity(0, lvlLeds * ledsIncrement);
        lcd.setCursor(5, 0);
        lcd.print("     ");
        lcd.setCursor(5, 0);
        for (byte i = 0; i < lvlLeds; ++i) {
          lcd.write(8);
        } 
      }

      else if (curOx == 3) {
        onVolume = !onVolume;
        EEPROM.update(eprByteVolume, onVolume); 
        lcd.setCursor(4, 0);
        lcd.print("        ");
        lcd.setCursor(4, 0); 
        if (onVolume == true) {
          lcd.print(" ON  off ");
        }
        else {
          lcd.print(" on  OFF ");
        }
      }

      dirOy = 0;
    }

    if (dirOx != 0) {
      curOx = (curOx + dirOx + (settingsOxModulo - 1)) % settingsOxModulo; 
      dirOx = 0; 
      if (curOx == 0) {
        lcd.setCursor(3, 1);
        lcd.print(" Bright. ");
        lcd.setCursor(4, 0);
        lcd.print("<     >  ");
        lcd.setCursor(5, 0);
        for (byte i = 0; i < lvlBright; ++i) {
          lcd.write(8);
        }
      }

      else if (curOx == 1) {
        lcd.setCursor(3, 1);
        lcd.print(" Contrast");
        lcd.setCursor(4, 0);
        lcd.print("<     >  ");
        lcd.setCursor(5, 0);
        for (byte i = 0; i < lvlContrast; ++i) {
          lcd.write(8);
        }
      }

      else if (curOx == 2) {
        lcd.setCursor(3, 1); 
        lcd.print(" 8x8 leds");
        lcd.setCursor(4, 0);
        lcd.print("<     >  ");
        lcd.setCursor(5, 0);
        for (byte i = 0; i < lvlLeds; ++i) {
          lcd.write(8);
        }
      }

      else if (curOx == 3) {
        lcd.setCursor(3, 1); 
        lcd.print("  Volume  ");
        lcd.setCursor(4, 0);
        lcd.print("       ");
        lcd.setCursor(4, 0); 
        if (onVolume == true) {
          lcd.print(" ON  off ");
        }
        else {
          lcd.print(" on  OFF ");
        }
      }
    }
  }
  
} menuObject; 


#endif
