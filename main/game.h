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
#define maxValueLed 250 
#define minValueLed 0 
#define applePozAddition 2
#define whereMaxSpawnApple 5
#define maxDetectDistance 42
#define howManyBorders 2
#define checkBlinkApple 100
#define makeBlinkTimeApple 60
#define checkBlinkKiller 500
#define makeBlinkTimeKiller 50
#define checkBlinkPlayer 500
#define makeBlinkTimePlayer 50
#define oneSecondMillis 1000
#define distanceOneFollow 4
#define distanceTwoFollow 3
#define intervalShiftFollow 7
#define toneTime 50 
#define stageOneKillerTime 60000
#define stageOneKillerDistance 5
#define stageTwoKillerTime 100000
#define stageTwoKillerDistance 4
#define stageThreeKillerTime 200000
#define stageThreeKillerDistance 3
#define stageFourKillerTime 1000000
#define stageFourKillerDistance 2
#define stageFiveKillerDistance 1
#define randomForDirections 3
#define dividerForKillerUpdate 20000
#define killerSpeedMultiplier 4
#define speedToTakeKiller 120
#define whenBeFollowedKillers 2

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

long matrix[matrixBigSize] = {0ll, 0ll, 0ll, 0ll, 0ll, 0ll, 0ll, 0ll, 
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
    byte dist = abs(xApple - xPos) + abs(yApple - yPos);
    return maxValueLed / (maxDetectDistance * (maxDetectDistance - dist));
  }
  
  void blinkApple() {
    
    if (millis() % checkBlinkApple < makeBlinkTimeApple) {
      change(xApple, yApple, 0);
    }
  
    else {
      change(xApple, yApple, 1);
    }
  }

  void tryRespawnApple() {
    byte xNewApple = random(matrixBigSize - howManyBorders) + 1; 
    byte yNewApple = random(matrixBigSize - howManyBorders) + 1; 
  
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
      if (millis() % checkBlinkPlayer < makeBlinkTimePlayer) {
        change(xPos, yPos, 0);
      }
  
      else {
        change(xPos, yPos, 1);
      }
    }
  }

  void updatePlayerOnDisplay(byte &x, byte &y) {

    if (millis() - lastTimeUpdatePlayer > blinkIntervalStaying) {
      stayedSeconds += (millis() - lastTimeUpdatePlayer) / oneSecondMillis; 
    }
  
    lastTimeUpdatePlayer = millis();
    
    if (displayMovementType == 0) {
      xStartDisplay = (byte) (x / matrixSize) * matrixSize;  
      xEndDisplay = ((byte) (x / matrixSize) + 1) * matrixSize - 1;
      yStartDisplay = (byte) (y / matrixSize) * matrixSize;  
      yEndDisplay = ((byte) (y / matrixSize) + 1) * matrixSize - 1;
    }
    
    else {
      if (x + distanceOneFollow < matrixBigSize) {
        xStartDisplay = max(0, x - distanceTwoFollow);
        xEndDisplay = xStartDisplay + intervalShiftFollow; 
      }
  
      if (y + distanceOneFollow < matrixBigSize) {
        yStartDisplay = max(0, y - distanceTwoFollow);
        yEndDisplay = yStartDisplay + intervalShiftFollow;
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
          tone(buzzerPin, eatSound, toneTime);
        }
        toUpdateApple = true; 
        change(xApple, yApple, 0);
        appleObject.tryRespawnApple();
      }
  
      for (int i = 0; i < numberOfKillers; ++i) {
        if (killers[i].x == xPos && killers[i].y == yPos) {
          --numberOfLifes;
          if (onVolume == true) {
            tone(buzzerPin, dieSound, toneTime);
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
    byte xNewWall = random(matrixBigSize - howManyBorders) + 1;
    byte yNewWall = random(matrixBigSize - howManyBorders) + 1;
  
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
     
    if (millis() <= stageOneKillerTime) {
      return stageOneKillerDistance; 
    }
    else if (millis() <= stageTwoKillerTime) {
      return stageTwoKillerDistance; 
    }
    else if (millis() <= stageThreeKillerTime) {
      return stageThreeKillerDistance; 
    }
    else if (millis() <= stageFourKillerTime) {
      return stageFourKillerDistance; 
    }
    else {
      return stageFiveKillerDistance; 
    }
  }

  void tryGenerateKiller() {
    
    byte xNewKiller = xNewKiller = random(matrixBigSize - howManyBorders) + 1;
    byte yNewKiller = yNewKiller = random(matrixBigSize - howManyBorders) + 1;
  
    if (xNewKiller == xApple && yNewKiller == yApple) {
      return;
    } 
  
    if (abs(xNewKiller - xPos) + abs(yNewKiller -  yPos) < killerDistance()) {
      return; 
    }

    if (xNewKiller == 1 && yNewKiller == 1) {
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
    if (millis() % checkBlinkKiller < makeBlinkTimeKiller) {
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

      int xNewKiller; 
      int yNewKiller;

      int actX = -1; 
      int actY = -1;

      int whoNextX = - 1;
      int whoNextY = - 1; 
          
      if (numberOfKillers >= whenBeFollowedKillers) {
        // initialized by random big value
        int maxToMe = 200; 
        
        for (int p1 = -1; p1 <= 1; ++p1) {
          for (int p2 = -1; p2 <= 1; ++p2) {
            actX = killers[i].x + p1; 
            actY = killers[i].y + p2;
            Serial.println(actX); 
            if (checkNotObstacle(actX, actY) == true && !(actX == xApple && actY == yApple)) {
              if (abs(actX - xPos) + abs(actY - yPos) < maxToMe) {
                maxToMe = abs(actX - xPos) + abs(actY - yPos); 
                whoNextX = actX; 
                whoNextY = actY;
              }
            }
          }
        }
      }

      if (whoNextX == -1 && whoNextY == -1) {
        int directionOx = random(randomForDirections) - 1;
        int directionOy = random(randomForDirections) - 1;
        xNewKiller = killers[i].x + directionOx; 
        yNewKiller = killers[i].y + directionOy; 
      }

      else {
        xNewKiller = whoNextX; 
        yNewKiller = whoNextY;
      }
      
      if (checkNotObstacle(xNewKiller, yNewKiller) == true && !(xNewKiller == xApple && yNewKiller == yApple)) {
        change(xNewKiller, yNewKiller, 1);
        change(killers[i].x, killers[i].y, 0);
        
        killers[i].x = xNewKiller; 
        killers[i].y = yNewKiller;
        if (killers[i].x == xPos && killers[i].y == yPos) {
          --numberOfLifes;
          if (onVolume == true) {
            tone(buzzerPin, dieSound, toneTime);
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
    int speedKiller = min(millis() / dividerForKillerUpdate + 1, maximumKillerSpeed); 
  
    if (millis() - lastKillerUpdateTime > (speedToTakeKiller - speedKiller) * killerSpeedMultiplier) {
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
    
    for (byte i = 0; i < matrixBigSize; ++i) {
      matrix[i] = 0ll;
    }
    boardMatrix();
    
    xApple = random(whereMaxSpawnApple) + applePozAddition; 
    yApple = random(whereMaxSpawnApple) + applePozAddition;

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
            analogWrite(ledPinGreen, maxValueLed);
          }
          else {
            isGreenToLightPower = false; 
            analogWrite(ledPinGreen, minValueLed);
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

    analogWrite(ledPinGreen, minValueLed);
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
