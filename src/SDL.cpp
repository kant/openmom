//
//  SDL.cpp
//  OpenMoM
//
//  Created by Jack on 7/19/14.
//  Copyright (c) 2014 Jack. All rights reserved.
//

#include "SDL.h"

#include "Font.h"
#include "Gfx.h"

#include "ViewManager.h"

SDL_Window* SDL::window = nullptr;
SDL_Renderer* SDL::renderer = nullptr;
SDL_Texture* SDL::screen = nullptr;

bool SDL::willQuit = false;
u32 SDL::ticks = 0;

ViewManager* SDL::gvm = new ViewManager();

bool SDL::init()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    return false;
  
  window = SDL_CreateWindow("OpenMoM v0.01a",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 400, SDL_WINDOW_OPENGL);
  renderer = SDL_CreateRenderer(window, -1, 0);
  
  screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 320, 200);
  
  return true;
}

void SDL::loop()
{
  while (!willQuit)
  {
    render();
    handleEvents();
    
    capFPS();
  }
}

void SDL::capFPS()
{
  u32 ticks = SDL_GetTicks();
  u32 elapsed = ticks - SDL::ticks;
  
  if (elapsed < TICKS_PER_FRAME)
    SDL_Delay(TICKS_PER_FRAME - elapsed);
  
  SDL::ticks = SDL_GetTicks();
}

void SDL::deinit()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyTexture(screen);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

bool buttonDown = false;
void SDL::handleEvents()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      case SDL_QUIT: willQuit = true; break;
      
      case SDL_MOUSEBUTTONDOWN:
      {
        gvm->mousePressed(event.button.x/2, event.button.y/2, static_cast<MouseButton>(event.button.button));
        buttonDown = true;
        break;
      }
      case SDL_MOUSEBUTTONUP:
      {
        gvm->mouseReleased(event.button.x/2, event.button.y/2, static_cast<MouseButton>(event.button.button));
        buttonDown = false;
        break;
      }
      case SDL_MOUSEMOTION:
      {
        if (buttonDown)
          gvm->mouseDragged(event.button.x/2, event.button.y/2, BUTTON_LEFT);
        else
          gvm->mouseMoved(event.button.x/2, event.button.y/2, BUTTON_LEFT);
        break;
      }
      
      case SDL_KEYDOWN:
      {
        gvm->keyPressed(event.key.keysym.scancode, static_cast<KeyboardMod>(event.key.keysym.mod));
        break;
      }
        
      case SDL_KEYUP:
      {
        gvm->keyReleased(event.key.keysym.scancode, static_cast<KeyboardMod>(event.key.keysym.mod));
        break;
      }
    }
  }
}

void SDL::render()
{
  gvm->draw();
  
  SDL_Surface *canvas = Gfx::getCanvas();
  SDL_UpdateTexture(screen, nullptr, canvas->pixels, canvas->pitch);
  
  
  
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, screen, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}