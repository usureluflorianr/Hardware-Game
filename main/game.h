#ifndef GAME_H
#define GAME_H

//-VARIABLES--------------------------------------------------------------------------------------------------------

const int xPin = A0;
const int yPin = A1;

#define minThreshold 200
#define maxThreshold 600
#define blinkIntervalStaying 3000
#define moveInterval 100
#define generateIntervalKiller 15000 
#define generateIntervalWall 15000  
#define powerDuration 5000 
#define powerChangeColorInterval 5000 
#define matrixSize 8 
#define maximumKillerSpeed 60
#define maximumNumberOfKillers 10
#define matrixBigSize 24
#define maximumNumberOfWalls 100
#define buzzerPin 7
#define eatSound 3000
#define dieSound 100

byte xApple = 0;
byte yApple = 0; 
byte displayMovementType = 0;
byte xPos = 1;
byte yPos = 1;
byte xLastPos = 1;
byte yLastPos = 1;
byte xStartDisplay = 0;
byte xEndDisplay = matrixSize - 1; 
byte yStartDisplay = 0;
byte yEndDisplay = matrixSize - 1;
byte numberOfPowers = 2; 
byte numberOfLifes = 2; 

unsigned int long long lastKillerUpdateTime = 0;
unsigned int long long lastKillerGenerateTime = 0;
unsigned int long long lastTimeUpdatePlayer = 0; 
unsigned int long long myLastMove = 0; 
unsigned int long long lastMoved = 0;
unsigned int long long lastWallGeneratedTime = 0; 
unsigned int long long startPowerTime = 0; 
unsigned int long long lastColorPowerChange = 0;
unsigned int long long whenStartedGame = 0;

byte numberOfKillers = 0;
byte stayedSeconds = 0; 
byte collectedApples = 0;
byte numberOfWalls = 0;
int score = 0;
int prevScore = 0; 

bool toGenerateKiller = false;
bool toUpdateApple = false;
bool endedGame = false;
bool toGenerateWall = false; 
bool doButtonGame = false;
bool isGreenToLightPower = false; 
bool atSelectName = false;

struct {
  byte x; 
  byte y;   
} killers[maximumNumberOfKillers], walls[maximumNumberOfWalls];

long matrix[24] = {0ll, 0ll, 0ll, 0ll, 0ll, 0ll, 0ll, 0ll, 
                   0ll, 0ll, 0ll, 0ll, 0ll, 0ll, 0ll, 0ll, 
                   0ll, 0ll, 0ll, 0ll, 0ll, 0ll, 0ll, 0ll}; 

void change(byte x, byte y, byte what) {
  if ((matrix[x] & (1ll << y)) == 0 && what == 0) {
    return;  
  }

  if ((matrix[x] & (1ll << y)) != 0 && what == 1) {
    return;  
  }

  if ((matrix[x] & (1ll << y)) == 0) {
    matrix[x] += (1ll << y);  
    return;
  }

  if ((matrix[x] & (1ll << y)) != 0) {
    matrix[x] -= (1ll << y);
    return; 
  }
}

//------------------------------------------------------------------------------------------------------------------

//-UTILS------------------------------------------------------------------------------------------------------------

bool checkNotObstacle(byte x, byte y) {
  for (byte i = 0; i < numberOfWalls; ++i) {
    if (walls[i].x == x && walls[i].y == y) {
      return false;
    }
  }
  return x > 0 && x < matrixBigSize - 1 && y > 0 && y < matrixBigSize - 1; 
}


//------------------------------------------------------------------------------------------------------------------

//-APPLE CLASS------------------------------------------------------------------------------------------------------

class Apple {
  public:

  byte appleLedDistance() {
    // these are not magic numbers, 42 = 21 + 21, biggest distance between apple and player
    byte dist = abs(xApple - xPos) + abs(yApple - yPos);
    return 250 / 42 * (42 - dist);
  }
  
  void blinkApple() {
    if (millis() % 100 < 60) {
      change(xApple, yApple, 0);
    }
  
    else {
      change(xApple, yApple, 1);
    }
  }

  void tryRespawnApple() {
    byte xNewApple = random(matrixBigSize - 2) + 1; 
    byte yNewApple = random(matrixBigSize - 2) + 1; 
  
    if (xNewApple == xApple && yNewApple == yApple) {
      return;
    } 
  
    if (xNewApple == xPos && yNewApple == yPos) {
      return;
    }
  
    if (checkNotObstacle(xNewApple, yNewApple) == false) {
      return;
    }
    
    xApple = xNewApple; 
    yApple = yNewApple; 

    change(xApple, yApple, 1);
  
    toUpdateApple = false; 
  }

} appleObject;

//------------------------------------------------------------------------------------------------------------------

//-PLAYER CLASS-----------------------------------------------------------------------------------------------------

class Player {
  public:

  void blinkPlayer() {

    change(xPos, yPos, 1);
    if (millis() - myLastMove > blinkIntervalStaying) {
      if (millis() % 500 < 50) {
        change(xPos, yPos, 0);
      }
  
      else {
        change(xPos, yPos, 1);
      }
    }
  }

  void updatePlayerOnDisplay(byte &x, byte &y) {

    if (millis() - lastTimeUpdatePlayer > blinkIntervalStaying) {
      stayedSeconds += (millis() - lastTimeUpdatePlayer) / 1000; 
    }
  
    lastTimeUpdatePlayer = millis();
    
    if (displayMovementType == 0) {
      xStartDisplay = (byte) (x / matrixSize) * matrixSize;  
      xEndDisplay = ((byte) (x / matrixSize) + 1) * matrixSize - 1;
      yStartDisplay = (byte) (y / matrixSize) * matrixSize;  
      yEndDisplay = ((byte) (y / matrixSize) + 1) * matrixSize - 1;
    }
    
    else {
      if (x + 4 < matrixBigSize) {
        xStartDisplay = max(0, x - 3);
        xEndDisplay = xStartDisplay + 7; 
      }
  
      if (y + 4 < matrixBigSize) {
        yStartDisplay = max(0, y - 3);
        yEndDisplay = yStartDisplay + 7;
      }
    }  
  }

  void updatePlayer(LiquidCrystal &lcd, bool &firstTimeHere, bool &onVolume) {
    int xValue = analogRead(xPin);
    int yValue = analogRead(yPin);
    
    xLastPos = xPos;
    yLastPos = yPos;
    
    if (xValue < minThreshold) {
      if (checkNotObstacle(xPos + 1, yPos)) {
        xPos++;
        updatePlayerOnDisplay(xPos, yPos);
      }
    }
  
    if (xValue > maxThreshold) {
      if (checkNotObstacle(xPos - 1, yPos)) {
        xPos--;
        updatePlayerOnDisplay(xPos, yPos);
      }
    }
    
    if (yValue > maxThreshold) {
      if (checkNotObstacle(xPos, yPos + 1)) {
        yPos++;
        updatePlayerOnDisplay(xPos, yPos);
      }
    }
      
    if (yValue < minThreshold) {
      if (checkNotObstacle(xPos, yPos - 1)) {
        yPos--;
        updatePlayerOnDisplay(xPos, yPos);
      }
    }
    
    if (xLastPos != xPos || yLastPos != yPos) {
      if (xPos == xApple && yPos == yApple) {
        ++collectedApples; 
        if (onVolume == true) {
          tone(buzzerPin, eatSound, 50);
        }
        toUpdateApple = true; 
        change(xApple, yApple, 0);
        appleObject.tryRespawnApple();
      }
  
      for (int i = 0; i < numberOfKillers; ++i) {
        if (killers[i].x == xPos && killers[i].y == yPos) {
          --numberOfLifes;
          if (onVolume == true) {
            tone(buzzerPin, dieSound, 50);
          }
          if (numberOfLifes == 0) {
            endedGame = true; 
            firstTimeHere = true; 
          }
          lcd.setCursor(12, 0);
          lcd.print(numberOfLifes);
          if (numberOfLifes == 1) {
            ++numberOfPowers;
            startPowerTime = millis();
            doButtonGame = true; 
            lastColorPowerChange = millis();
            xPos = 1; 
            yPos = 1;
            updatePlayerOnDisplay(xPos, yPos);
          }
        }
      }
      
      myLastMove = millis();  

      change(xPos, yPos, 1);
      change(xLastPos, yLastPos, 0);
    }
  }
  
} playerObject; 

//------------------------------------------------------------------------------------------------------------------

//-WALL CLASS-------------------------------------------------------------------------------------------------------

class Wall {
  public:

  void tryGenerateWall() {
    byte xNewWall = random(matrixBigSize - 2) + 1;
    byte yNewWall = random(matrixBigSize - 2) + 1;
  
    if (xNewWall == xPos && yNewWall == yPos) {
      return; 
    }
  
    if (xNewWall == xApple && yNewWall == yApple) {
      return;
    }

    if (xNewWall + yNewWall == 2) {
      return; 
    }
  
    for (int i = 0; i < numberOfKillers; ++i) {
      if (walls[i].x == killers[i].x && walls[i].y == killers[i].y) {
        return;
      }
    }
    
    for (int i = 0; i < numberOfWalls; ++i) {
      if (walls[i].x == xNewWall && walls[i].y == yNewWall) {
        return;
      }
    }
  
    ++numberOfWalls; 
    
    change(xNewWall, yNewWall, 1); 
    
    walls[numberOfWalls - 1].x = xNewWall; 
    walls[numberOfWalls - 1].y = yNewWall; 
    toGenerateWall = false; 
    lastWallGeneratedTime = millis();
  }
} wallObject;

//------------------------------------------------------------------------------------------------------------------

//-KILLER CLASS-----------------------------------------------------------------------------------------------------

class Killer {
  public: 
  
  byte killerDistance() {
    // aici am nevoie de o functie f(x) cu x din millis() care da valori de la 5 la 1 
    // cam naspa de facut, deci hardcodez functia dupa bunul plac 
    if (millis() <= 60000) {
      return 5; 
    }
    else if (millis() <= 100000) {
      return 4; 
    }
    else if (millis() <= 200000) {
      return 3; 
    }
    else if (millis() <= 1000000) {
      return 2; 
    }
    else {
      return 1; 
    }
  }

  void tryGenerateKiller() {
    byte xNewKiller = xNewKiller = random(matrixBigSize - 2) + 1;
    byte yNewKiller = yNewKiller = random(matrixBigSize - 2) + 1;
  
    if (xNewKiller == xApple && yNewKiller == yApple) {
      return;
    } 
  
    if (abs(xNewKiller - xPos) + abs(yNewKiller -  yPos) < killerDistance()) {
      return; 
    }

    if (xNewKiller + yNewKiller == 2) {
      return;
    }
  
    for (byte i = 0; i < numberOfKillers; ++i) {
      if (xNewKiller == killers[i].x && yNewKiller == killers[i].y) {
        return; 
      }
    }
  
    if (checkNotObstacle(xNewKiller, yNewKiller) == false) {
      return;
    }
  
    ++numberOfKillers; 
  
    killers[numberOfKillers - 1].x = xNewKiller;
    killers[numberOfKillers - 1].y = yNewKiller;

    change(xNewKiller, yNewKiller, 1);

    Serial.println(xNewKiller); 
    toGenerateKiller = false; 
  
    lastKillerGenerateTime = millis();
  }

  void blinkKillers() {
    if (millis() % 500 < 50) {
      for (byte i = 0; i < numberOfKillers; ++i) {
        change(killers[i].x, killers[i].y, 0);
      }
    }
    else {
      for (byte i = 0; i < numberOfKillers; ++i) {
        change(killers[i].x, killers[i].y, 1);
      }
    }
  }

  void updateKillers(LiquidCrystal &lcd, bool &firstTimeHere, bool &onVolume) {
    for (byte i = 0; i < numberOfKillers; ++i) {
      byte directionOx = random(3) - 1;
      byte directionOy = random(3) - 1;
  
      byte xNewKiller = killers[i].x + directionOx; 
      byte yNewKiller = killers[i].y + directionOy; 
  
      if (checkNotObstacle(xNewKiller, yNewKiller) == true && !(xNewKiller == xApple && yNewKiller == yApple)) {

        change(xNewKiller, yNewKiller, 1);
        change(killers[i].x, killers[i].y, 0);
        
        killers[i].x = xNewKiller; 
        killers[i].y = yNewKiller;
        if (killers[i].x == xPos && killers[i].y == yPos) {
          --numberOfLifes;
          if (onVolume == true) {
            tone(buzzerPin, dieSound, 50);
          }
          if (numberOfLifes == 0) {
            endedGame = true; 
            firstTimeHere = true;
          }
          lcd.setCursor(12, 0);
          lcd.print(numberOfLifes);
          if (numberOfLifes == 1) {
            ++numberOfPowers;
            startPowerTime = millis();
            doButtonGame = true; 
            lastColorPowerChange = millis();
            xPos = 1; 
            yPos = 1;
            playerObject.updatePlayerOnDisplay(xPos, yPos);
          }
        }
      }
    }
  }

  void checkIfNeedUpdateKillers(LiquidCrystal &lcd, bool &firstTimeHere, bool &onVolume) {
    int speedKiller = min(millis() / 20000 + 1, maximumKillerSpeed); 
  
    if (millis() - lastKillerUpdateTime > (120 - speedKiller) * 4) {
      updateKillers(lcd, firstTimeHere, onVolume);
      lastKillerUpdateTime = millis();
    }
  }

} killerObject;

class Game {
  public: 
  
  void boardMatrix() {
    for (int i  = 0; i < matrixBigSize; ++i) {

      change(0, i, 1);
      change(matrixBigSize - 1, i, 1);
      change(i, 0, 1);
      change(i, matrixBigSize - 1, 1);
    }
  }

  void initGame() {
    xPos = 1;
    yPos = 1;
    xLastPos = 1;
    yLastPos = 1;
    numberOfPowers = 2; 
    numberOfLifes = 2;
    numberOfKillers = 0;
    collectedApples = 0;
    stayedSeconds = 0; 
    numberOfWalls = 0;
    score = 0;
    prevScore = 0; 
    lastKillerUpdateTime = millis();
    lastKillerGenerateTime = millis();
    lastTimeUpdatePlayer = millis(); 
    myLastMove = millis(); 
    lastMoved = millis();
    lastWallGeneratedTime = millis(); 
    startPowerTime = millis(); 
    lastColorPowerChange = millis();
    whenStartedGame = millis(); 
    toGenerateKiller = false;
    toUpdateApple = false;
    toGenerateWall = false; 
    
    for (byte i = 0; i < 24; ++i) {
      matrix[i] = 0ll;
    }
    boardMatrix();
    
    xApple = random(5) + 2; 
    yApple = random(5) + 1;

    change(xApple, yApple, 1);
  }

  void runStep(const int &ledPinBlue, const int &ledPinGreen, LiquidCrystal &lcd, bool &firstTimeHere, bool &onVolume) {

    if (doButtonGame == true && numberOfPowers > 0) {
      if (millis() - startPowerTime > powerDuration) {
        doButtonGame = false; 
        --numberOfPowers;
      }

      else {
        if (millis() - lastColorPowerChange <= powerChangeColorInterval) {
          if (isGreenToLightPower == false) {
            isGreenToLightPower = true; 
            digitalWrite(ledPinBlue, false);
            analogWrite(ledPinGreen, 250);
          }
          else {
            isGreenToLightPower = false; 
            analogWrite(ledPinGreen, 0);
            digitalWrite(ledPinBlue, true);
          }
        }
        
        if (millis() - lastMoved > moveInterval) {
          playerObject.updatePlayer(lcd, firstTimeHere, onVolume);
          lastMoved = millis();
        }

        if (toUpdateApple == true) {
          appleObject.tryRespawnApple();
        }

        return;
      }
    }

    analogWrite(ledPinGreen, 0);
    digitalWrite(ledPinBlue, false);

    analogWrite(ledPinGreen, appleObject.appleLedDistance());
    
    killerObject.checkIfNeedUpdateKillers(lcd, firstTimeHere, onVolume);

    if (millis() - lastWallGeneratedTime > generateIntervalWall) {
      toGenerateWall = true;
    }
  
    if (millis() - lastMoved > moveInterval) {
      playerObject.updatePlayer(lcd, firstTimeHere, onVolume);
      lastMoved = millis();
    }

    if (millis() - lastKillerGenerateTime > generateIntervalKiller) {
      toGenerateKiller = true; 
    }

    if (toUpdateApple == true) {
      appleObject.tryRespawnApple();
    }

    if (toGenerateKiller == true && numberOfKillers < maximumNumberOfKillers) {
      killerObject.tryGenerateKiller(); 
    }

    if (toGenerateWall == true) {
      wallObject.tryGenerateWall(); 
    }
  }
  
} gameObject;

#endif
