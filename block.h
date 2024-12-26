#ifndef BLOCK_H
#define BLOCK_H

// rotation information for blocks | could use 2nd index bit to dictate +/-
// and 1st to dictate up or down
// could be more efficient
#define b_UP 0b00    // 0
#define b_DOWN 0b01  // 1
#define b_LEFT 0b10  // 2
#define b_RIGHT 0b11 // 3
// lookup table for directions. with enum?
enum BlockDirection { bUP, bDOWN, bLEFT, bRIGHT };
const enum BlockDirection blockRotationLookup[4] = {bUP, bDOWN, bLEFT, bRIGHT};
// instead of up down left right could do hFlip and vFlip bools
// makes no difference except how its implemented and which is more efficient
// most likely fliping but current rotations can be interpreted that way

// bitmasks for block values
// shape variable
#define BITMASK_SHAPE_0 0b00000001
#define BITMASK_SHAPE_1 0b00000010
#define BITMASK_SHAPE_2 0b00000100
#define BITMASK_SHAPE_3 0b00001000
#define BITMASK_SHAPE_4 0b00010000
#define BITMASK_SHAPE_5 0b00100000
#define BITMASK_SHAPE_ALL 0b00111111
#define BITMASK_ROTATION 0b11000000
#define BLOCK_WIDTH 3
#define BLOCK_HEIGHT 2
const int blockShapeLookup[6] = {BITMASK_SHAPE_0, BITMASK_SHAPE_1,
                                 BITMASK_SHAPE_2, BITMASK_SHAPE_3,
                                 BITMASK_SHAPE_4, BITMASK_SHAPE_5};
// visual variable
#define BITMASK_COLOUR_RED 0b00110000
#define BITMASK_COLOUR_GREEN 0b00001100
#define BITMASK_COLOUR_BLUE 0b00000011
// utility lookup table to get color values
const int blockColourLookup[4] = {0, 85, 170, 255};
// position variable
#define BITMASK_POS_X 0b00001111
#define BITMASK_POS_Y 0b11110000

// declare unsigned 8 bit int type
typedef unsigned char uint8;

// bool type to store true/false in one byte
typedef uint8 b_bool;

// struct to store block/tetroid data
// if more variables are added to struct could store it all in an int
typedef struct {
  /*
  store blocks in 2x3 grid -- if rotation > 1 becomes 3x2
  _ # _   _ _ #   # # _   0 1 2
  # # #   # # #   _ # #   3 4 5

  can store each block in 6 bits 543210 | 0 or 1 for true or false
  - center point could always be location 4
  remaining 2 bits can store rotaion
    - four possibilies: up, down, left, right
                        00  01    10    11
  each block fits in unsigned char (uint8) or 1 8-bit byte

  stored as rr543210
  */
  uint8 shape;

  /*
  store colour and pos maybe center index
  r g b x 2 bits = 6 bits for 100% or 66% or 33% or 0% colour
  11 00 01        255(red) 0(green) 85(blue) = reddy purple
  11 00 11        255(red) 0(green) 255(blue) = 50/50 purple
  10 11 00        170(red) 255(gren) 0%(blue) = yellowy orange

  2 bits left !!!!!!

  store as 00rrggbb
  */
  uint8 visual;

  /*
  store position data
  15x6 grid | max 15x15
  1 2 4 8  - 4 bits needed for y pos
  1 2 4 8  - 4 bits needed for x pos

  store as yyyyxxxx
  */
  uint8 pos;

} block_t;

// functions to get and set each value from class values

// Position variables
// ----------------------------------------------------------
uint8 bGetPosY(uint8 blockPos) { return (blockPos & BITMASK_POS_Y) >> 4; }
uint8 bGetPosX(uint8 blockPos) { return (blockPos & BITMASK_POS_X); }
void bSetPosY(block_t *block, uint8 value) {
  if (value > 15)
    value = 15;
  // right shift and or into pos
  value = value << 4;
  // mask to only get x values to add y values without changing them
  block->pos = (block->pos & BITMASK_POS_X) | value;
}
void bSetPosX(block_t *block, uint8 value) {
  if (value > 15)
    value = 15;
  // no need to bit shift due to being first 4 bits
  // mask to only get y values to add x values without changing them
  block->pos = (block->pos & BITMASK_POS_Y) | value;
}

// Colour variables
// ----------------------------------------------------------
uint8 bGetColRed(uint8 blockVisual) {
  return (blockVisual >> 4);
} // no bitmask because first two bits should be zero
uint8 bGetColGreen(uint8 blockVisual) {
  return (blockVisual & BITMASK_COLOUR_GREEN) >> 2;
}
uint8 bGetColBlue(uint8 blockVisual) {
  return (blockVisual & BITMASK_COLOUR_BLUE);
} // no bitshift because blue is first two bits
void bSetColRed(block_t *block, uint8 value) {
  if (value > 3)
    value = 3; // clamp value to 2
  // shift bits into correct position
  value = value << 4;
  // invert red bitmask to save all values except red and or in new value
  block->visual = (block->visual & ~BITMASK_COLOUR_RED) | value;
}
void bSetColGreen(block_t *block, uint8 value) {
  if (value > 3)
    value = 3; // clamp value to 2
  // shift bits into correct position
  value = value << 2;
  // invert green bitmask to save all values except green and or in new value
  block->visual = (block->visual & ~BITMASK_COLOUR_GREEN) | value;
}
void bSetColBlue(block_t *block, uint8 value) {
  if (value > 3)
    value = 3; // clamp value to 2
  // no bitshift, digits already in place
  // invert blue bitmask to save all values except blue and or in new value
  block->visual = (block->visual & ~BITMASK_COLOUR_BLUE) | value;
}

// Rotation variable
// --------------------------------------------------------
enum BlockDirection bGetRotation(uint8 blockShape) {
  return blockRotationLookup[(blockShape & BITMASK_ROTATION) >> 6];
} // bitmask and  shift to isolate rotation and lookup direction
void bSetRotation(block_t *block, enum BlockDirection direction) {
  // utilise int value of enum to convert to binary
  uint8 intDirection = direction;
  // bitshift direction to position
  intDirection = intDirection << 6;
  // invert rotation bitmask and or in new rotation
  block->shape = (block->shape & ~BITMASK_ROTATION) | intDirection;
}

// Shape variable
// -------------------------------------------------------
// load shape into array to iterate through
void *bGetShape(uint8 blockShape, uint8 returnArray[6]) {
  // array to store output
  // uint8 shape[(BLOCK_WIDTH * BLOCK_HEIGHT)];
  // mask bits to isolate shape - most likely unneccessary
  blockShape = blockShape & BITMASK_SHAPE_ALL;
  for (int i = 0; i < (BLOCK_WIDTH * BLOCK_HEIGHT); i++) {
    returnArray[i] = (blockShape & blockShapeLookup[i]) >> i;
  }
  return returnArray;
}
// function to convert each shape index into one uint8 excluding rotation
// zero or ones only
// can use this function to update shape by converting indivial values
// and or rotation
uint8 bConvShape(b_bool shapeValues[6]) {
  uint8 collatedShape = 0;
  // could use lookup and iterate through to add together
  for (int i = 0; i < (BLOCK_WIDTH * BLOCK_HEIGHT); i++) {
    if (shapeValues[i] > 1)
      shapeValues[i] = 1; // clamp to 1 or 0
    // bitshift by iteration and or into output
    uint8 shiftedValue = shapeValues[i] << i;
    collatedShape = collatedShape | shiftedValue;
  }
  return collatedShape;
}
// use convshape function to update shape values and set them in block struct
void bUpdateShape(block_t *block, b_bool shapeValues[6]) {
  // mask out shape bools from shape variable
  block->shape = block->shape & ~BITMASK_SHAPE_ALL;
  // or in new shape values
  block->shape = block->shape | bConvShape(shapeValues);
}

#endif
