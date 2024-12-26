#include "block.h"

#include <unistd.h>
#include <stdio.h>

int main() {
  /*
  setup game window
  print game layout/grid
    - render colours shape text ect...
  draw block
    - block object
      - class to store variables
        - how to store block info?
      - generic block object and different types
    - movement and rotation
    - shape and colour
  game logic
    - score
    - next block
    - clearing lines
    - game loop
  game ui
    - render text from font
  Sound?

  */

  block_t myBlock = {.visual = 0b00010011, .shape = 0b1111001}; // 1 0 3
  // printf("red: %d\n", bGetColRed(myBlock.visual));
  // printf("green: %d\n", bGetColGreen(myBlock.visual));
  // printf("blue: %d\n", bGetColBlue(myBlock.visual));

  // bSetColRed(&myBlock, 2);
  // printf("new value: %d\n", myBlock.visual);
  // printf("red: %d\n", bGetColRed(myBlock.visual));
  // printf("green: %d\n", bGetColGreen(myBlock.visual));
  // printf("blue: %d\n", bGetColBlue(myBlock.visual));
  // printf("actual red: %d\n", blockColourLookup[bGetColRed(myBlock.visual)]);

  // printf("direction index: %d\n", bGetRotation(myBlock.shape));
  // bSetRotation(&myBlock, bDOWN);
  // printf("direction index: %d\n", bGetRotation(myBlock.shape));
  printf("sizeof(bool): %lu\n", sizeof(b_bool));

  // convert shape to list
  uint8 shapelist[6];
  bGetShape(myBlock.shape, shapelist);
  for (int y = 0; y < BLOCK_HEIGHT; y++) {
    for (int x = 0; x < BLOCK_WIDTH; x++) {
      if (shapelist[(BLOCK_WIDTH * y) + x] == 1) {
        printf("#");
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }

  printf("Converted values: %u\n", bConvShape(shapelist));

  return 0;
}