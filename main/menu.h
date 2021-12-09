#ifndef MENU_H
#define MENU_H

#define lastEndLCD 16
#define scrollInterval 400

byte arrowRight[8] = {
  B00000, 
  B00100, 
  B00110,
  B11111, 
  B00110, 
  B00100, 
  B00000, 
  B00000 
};

byte arrowLeft[8] = {
  B00000, 
  B00100, 
  B01100,
  B11111, 
  B01100, 
  B00100, 
  B00000, 
  B00000 
};

byte arrowDown[8] = {
  B00000, 
  B00100, 
  B00100,
  B11111, 
  B01110, 
  B00100, 
  B00000, 
  B00000 
};

byte arrowUp[8] = {
  B00000, 
  B00100, 
  B01110,
  B11111, 
  B00100, 
  B00100, 
  B00000, 
  B00000 
};

byte circle[8] = {
  B00000, 
  B00100, 
  B01110,
  B11111, 
  B01110, 
  B00100, 
  B00000, 
  B00000 
};

byte actualPoz = 0;   
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

int curOx = 1; 
int curOy = 0; 

char nameChar1 = 'U';
char nameChar2 = 'S';
char nameChar3 = 'U';

unsigned int long long lastTimeScrolledText = 0;

const String promoText = "Story: life is short. Collect stories to be happy and as fast as you can, because walls will appear and many to stop you!.....     ";

class Menu {
  public:

  void init(LiquidCrystal lcd) {
    lcd.createChar(1, arrowRight);
    lcd.createChar(2, arrowLeft);
    lcd.createChar(3, arrowUp);
    lcd.createChar(4, arrowDown);
    lcd.createChar(5, circle);
  }

  bool doPromo(LiquidCrystal lcd) {
    lcd.setCursor(0, 1);
    lcd.print(promoText.substring(actualPoz, actualPoz + lastEndLCD));
    
    actualPoz++;
    
    if ((actualPoz + lastEndLCD - 1) == (promoText.length() - 1)) {
      return false;
    }
    return true;
  }

  void doMainMenu(LiquidCrystal lcd) {
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
      curOx = 1; 
    }
    curOx %= 2;
    if (curOy == -1) {
      curOy = 3; 
    }
    curOy %= 4; 

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

  void doAbout(LiquidCrystal lcd) {
    if (firstTimeHere == true) {
      firstTimeHere = false; 
      lcd.clear();
      lcd.setCursor(1, 0); 
      lcd.print("ABOUT:");
      lcd.setCursor(12, 0);
      lcd.print("BACK");
      lcd.setCursor(1, 1);
      lcd.print("YT: Usu The Man");
    }
  }

  void doSelectName(LiquidCrystal lcd) {
    if (curOx == -1) {
      curOx = 1; 
    }
    curOx %= 2;
    if (curOy == -1) {
      curOy = 3; 
    }
    curOy %= 4;
    
    if (firstTimeHere == true) {
      curOx = 1; 
      curOy = 3;
      dirOx = 0;
      dirOy = 0; 
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

  void doTypeGame(LiquidCrystal lcd, byte &displayMovementType) {
    if (curOx == -1) {
      curOx = 1; 
    }
    curOx %= 2;
    if (curOy == -1) {
      curOy = 1; 
    }
    curOy %= 2;
    
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
        displayMovementType = 0;
      }
      else {
        displayMovementType = 1;
        lcd.setCursor(3, 1);      
        lcd.print("  FOLLOW   ");
      }
    }
  }
  
} menuObject; 


#endif
