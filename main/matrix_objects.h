#ifndef MATRIX_OBJECTS_H
#define MATRIX_OBJECTS_H

byte pauseMatrix[matrixSize] = {
  B10100000, 
  B10100000,
  B10100000,
  B10101110,
  B11101000,
  B00001110,
  B00000010,
  B00001110
};

byte promoMatrix[matrixSize] = {
  B10001000, 
  B00010001,
  B00100010,
  B01000100,
  B10001000,
  B00010001,
  B00100010,
  B01000100
};

byte modeMenuImage[matrixSize] = {
  B00011000,
  B00111100,
  B01111110,
  B11011011,
  B11111111,
  B00100100,
  B01011010,
  B10100101
};

byte aboutMenuImage[matrixSize] = {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100
};

byte settingsMenuImage[matrixSize] = {
  B11000011,
  B10011001,
  B00011000,
  B11111111,
  B11111111,
  B00011000,
  B10011001,
  B11000011
};

byte scoresMenuImage[matrixSize] = {
  B01111110,
  B11111111,
  B11111111,
  B01111110,
  B00111100,
  B00011000,
  B10011001,
  B11111111
};

byte endedGameImage[matrixSize] = {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10011001,
  B10100101,
  B01000010,
  B00111100
};

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

byte heart[8] = {
  B00000, 
  B11011, 
  B11111,
  B01110, 
  B01110, 
  B00100, 
  B00000, 
  B00000 
};

byte power[8] = {
  B00000, 
  B01001, 
  B10101,
  B10101, 
  B01110, 
  B11111, 
  B11111, 
  B01110 
};

byte fullRect[8] = {
  B11111, 
  B11111, 
  B11111,
  B11111, 
  B11111, 
  B11111, 
  B11111, 
  B11111 
};

byte setRight[8] = {
  B11000, 
  B11100, 
  B11110,
  B11111, 
  B11110, 
  B11100, 
  B11000, 
  B10000 
};

byte setLeft[8] = {
  B00011, 
  B00111, 
  B01111,
  B11111, 
  B01111, 
  B00111, 
  B00011, 
  B00001 
};

#endif
