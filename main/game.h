#ifndef GAME_H
#define GAME_H

//-VARIABLES--------------------------------------------------------------------------------------------------------

const int xPin = A0;
const int yPin = A1;

const int minThreshold = 200;
const int maxThreshold = 600;
const int blinkIntervalStaying = 3000;
const int moveInterval = 100;
const int generateIntervalKiller = 15000; 
const int generateIntervalWall = 15000;  
const int powerDuration = 5000; 
const int powerChangeColorInterval = 5000; 

const byte matrixSize = 8; 
const byte maximumKillerSpeed = 60;
const byte maximumNumberOfKillers = 10;
const byte matrixBigSize = 16;
const byte maximumNumberOfWalls = 100;

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

unsigned int long long lastKillerUpdateTime = 0;
unsigned int long long lastKillerGenerateTime = 0;
unsigned int long long lastTimeUpdatePlayer = 0; 
unsigned int long long myLastMove = 0; 
unsigned int long long lastMoved = 0;
unsigned int long long lastWallGeneratedTime = 0; 
unsigned int long long startPowerTime = 0; 
unsigned int long long lastColorPowerChange = 0;

int numberOfKillers = 0;
int stayedSeconds = 0; 
int collectedApples = 0;
int numberOfWalls = 0;
int score = 0;

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

bool matrix[matrixBigSize][matrixBigSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0},
  //{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  //{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  //{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  //{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  //{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  //{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  //{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  //{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //0, 0, 0, 0, 0, 0, 0, 0}
};

//------------------------------------------------------------------------------------------------------------------

//-UTILS------------------------------------------------------------------------------------------------------------

bool checkNotObstacle(int x, int y) {
  for (int i = 0; i < numberOfWalls; ++i) {
    if (walls[i].x == x && walls[i].y == y) {
      return false;
    }
  }
  return x > 0 && x < matrixBigSize - 1 && y > 0 && y < matrixBigSize - 1; 
}

//------------------------------------------------------------------------------------------------------------------

//-KILLER CLASS-----------------------------------------------------------------------------------------------------

class Killer {
  public: 
  
  int killerDistance() {
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
    int xNewKiller = xNewKiller = random(matrixBigSize - 2) + 1;
    int yNewKiller = yNewKiller = random(matrixBigSize - 2) + 1;
  
    if (xNewKiller == xApple && yNewKiller == yApple) {
      return;
    } 
  
    if (abs(xNewKiller - xPos) + abs(yNewKiller -  yPos) < killerDistance()) {
      return; 
    }
  
    for (int i = 0; i < numberOfKillers; ++i) {
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
  
    matrix[xNewKiller][yNewKiller] = 1; 
  
    toGenerateKiller = false; 
  
    lastKillerGenerateTime = millis();
  }

  void blinkKillers() {
    if (millis() % 500 < 50) {
      for (int i = 0; i < numberOfKillers; ++i) {
        matrix[killers[i].x][killers[i].y] = 0; 
      }
    }
    else {
      for (int i = 0; i < numberOfKillers; ++i) {
        matrix[killers[i].x][killers[i].y] = 1; 
      }
    }
  }

  void updateKillers() {
    for (int i = 0; i < numberOfKillers; ++i) {
      int directionOx = random(3) - 1;
      int directionOy = random(3) - 1;
  
      int xNewKiller = killers[i].x + directionOx; 
      int yNewKiller = killers[i].y + directionOy; 
  
      if (checkNotObstacle(xNewKiller, yNewKiller) == true && !(xNewKiller == xApple && yNewKiller == yApple)) {
        matrix[xNewKiller][yNewKiller] = 1;
        matrix[killers[i].x][killers[i].y] = 0; 
        killers[i].x = xNewKiller; 
        killers[i].y = yNewKiller;
        if (killers[i].x == xPos && killers[i].y == yPos) {
          endedGame = true; 
        }
      }
    }
  }

  void checkIfNeedUpdateKillers() {
    int speedKiller = min(millis() / 20000 + 1, maximumKillerSpeed); 
  
    if (millis() - lastKillerUpdateTime > (120 - speedKiller) * 10) {
      updateKillers();
      lastKillerUpdateTime = millis();
    }
  }

} killerObject;

//------------------------------------------------------------------------------------------------------------------

//-APPLE CLASS------------------------------------------------------------------------------------------------------

class Apple {
  public:

  int appleLedDistance() {
    int dist = abs(xApple - xPos) + abs(yApple - yPos);
    return 250 / 31 * (31 - dist);
  }
  
  void blinkApple() {
    if (millis() % 100 < 60) {
      matrix[xApple][yApple] = 0; 
    }
  
    else {
      matrix[xApple][yApple] = 1; 
    }
  }

  void tryRespawnApple() {
    int xNewApple = random(matrixBigSize - 2) + 1; 
    int yNewApple = random(matrixBigSize - 2) + 1; 
  
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
  
    matrix[xApple][yApple] = 1; 
  
    toUpdateApple = false; 
  }

} appleObject;

//------------------------------------------------------------------------------------------------------------------

//-PLAYER CLASS-----------------------------------------------------------------------------------------------------

class Player {
  public:

  void blinkPlayer() {
  
    matrix[xPos][yPos] = 1; 
    if (millis() - myLastMove > blinkIntervalStaying) {
      if (millis() % 500 < 50) {
        matrix[xPos][yPos] = 0; 
      }
  
      else {
        matrix[xPos][yPos] = 1;
      }
    }
  }

  void updatePlayerOnDisplay(int x, int y) {

    if (millis() - lastTimeUpdatePlayer > blinkIntervalStaying) {
      stayedSeconds += (millis() - lastTimeUpdatePlayer) / 1000; 
    }
  
    lastTimeUpdatePlayer = millis();
    
    if (displayMovementType == 0) {
      xStartDisplay = (int) (x / matrixSize) * matrixSize;  
      xEndDisplay = ((int) (x / matrixSize) + 1) * matrixSize - 1;
      yStartDisplay = (int) (y / matrixSize) * matrixSize;  
      yEndDisplay = ((int) (y / matrixSize) + 1) * matrixSize - 1;
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

  void updatePlayer() {
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
        toUpdateApple = true; 
        matrix[xApple][yApple] = 0;
        appleObject.tryRespawnApple();
      }
  
      for (int i = 0; i < numberOfKillers; ++i) {
        if (killers[i].x == xPos && killers[i].y == yPos) {
          endedGame = true; 
        }
      }
      
      myLastMove = millis();  
      matrix[xPos][yPos] = 1;
      matrix[xLastPos][yLastPos] = 0;
    }
  }
  
} playerObject; 

//------------------------------------------------------------------------------------------------------------------

//-WALL CLASS-------------------------------------------------------------------------------------------------------

class Wall {
  public:

  void tryGenerateWall() {
    int xNewWall = random(matrixBigSize - 2) + 1;
    int yNewWall = random(matrixBigSize - 2) + 1;
  
    if (xNewWall == xPos && yNewWall == yPos) {
      return; 
    }
  
    if (xNewWall == xApple && yNewWall == yApple) {
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
    matrix[xNewWall][yNewWall] = 1; 
    walls[numberOfWalls - 1].x = xNewWall; 
    walls[numberOfWalls - 1].y = yNewWall; 
    toGenerateWall = false; 
    lastWallGeneratedTime = millis();
  }
} wallObject;

//------------------------------------------------------------------------------------------------------------------

class Game {
  public: 
  
  void boardMatrix() {
    for (int i  = 0; i < matrixBigSize; ++i) {
      matrix[0][i] = 1;
      matrix[matrixBigSize - 1][i] = 1; 
      matrix[i][0] = 1;
      matrix[i][matrixBigSize - 1] = 1;
    }
  }

  void initGame() {
    boardMatrix();
    
    xApple = random(5) + 2; 
    yApple = random(5) + 1;
    matrix[xApple][yApple] = 1;
  }

  void runStep(const int &ledPinBlue, const int &ledPinGreen) {

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
          playerObject.updatePlayer();
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
    
    killerObject.checkIfNeedUpdateKillers();

    if (millis() - lastWallGeneratedTime > generateIntervalWall) {
      toGenerateWall = true;
    }
  
    if (millis() - lastMoved > moveInterval) {
      playerObject.updatePlayer();
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
