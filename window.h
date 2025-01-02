#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>

typedef u_int32_t u32;

typedef struct {
  SDL_Window *win;
  int width, height;
  int running;
  SDL_Renderer *ren;
  SDL_Texture *tex;
  u32 *pixels;
  size_t pSize;
} WinMan;

int initWindow(WinMan *wm) {
  if (wm->width == NULL)
    wm->width = 800;
  if (wm->height == NULL)
    wm->height = 800;
  SDL_Init(SDL_INIT_VIDEO);
  wm->win =
      SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       wm->width, wm->height, SDL_WINDOW_SHOWN);
  wm->ren = SDL_CreateRenderer(wm->win, -1, SDL_RENDERER_PRESENTVSYNC);
  wm->tex =
      SDL_CreateTexture(wm->ren, SDL_PIXELFORMAT_ABGR8888,
                        SDL_TEXTUREACCESS_STREAMING, wm->width, wm->height);
  // allocate memory for pixels
  wm->pSize = wm->width * wm->height;
  wm->pixels = (u32 *)malloc(sizeof(u32) * wm->pSize);
  return 0;
}

int freeWinManager(WinMan *wm) {
  SDL_DestroyTexture(wm->tex);
  wm->tex = NULL;
  SDL_DestroyRenderer(wm->ren);
  wm->ren = NULL;
  SDL_DestroyWindow(wm->win);
  wm->win = NULL;
  wm->width = NULL;
  wm->height = NULL;
  free(wm->pixels);
  wm->pSize = NULL;
  return 0;
}

#endif