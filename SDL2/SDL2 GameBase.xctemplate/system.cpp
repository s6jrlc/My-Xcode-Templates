// ============================================================================
// [Include Section]
// ============================================================================
#include"system.h"

// ============================================================================
// [Defines & Constants]
// ============================================================================

Sint8 Sys::commands;

Sys::Sys() {
  // Initialize the SDL library.
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    fprintf(stderr, "SDL_Init() failed: %s\n", SDL_GetError());
    phase = GAME_QUIT;
  }
  initState();
  window = SDL_CreateWindow(_MAIN_,
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            720, 900,
                            WINDOW_MODE);
  if (!window) {
    showSDLerror();
    phase = GAME_QUIT;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    showSDLerror();
    phase = GAME_QUIT;
  }
}
Sys::~Sys() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Sys::initState() {
  phase = GAME_TITLE;
  phopt = cursol = 0;
  v_brightness = 1.f;
}

void Sys::cvtHSVintoRGB(float h, float s, float v,
                        Uint8& r, Uint8& g, Uint8& b) {
  float rf = v;
  float gf = v;
  float bf = v;
  if (s > 0.0f) {
    h *= 6.0f;
    const int i = static_cast<int>(h);
    const float f = h - static_cast<float>(i);
    switch (i) {
      case 0:
        gf *= 1 - s * (1 - f); bf *= 1 - s;
        break;
      case 1:
        rf *= 1 - s * f; bf *= 1 - s;
        break;
      case 2:
        rf *= 1 - s; bf *= 1 - s * (1 - f);
        break;
      case 3:
        rf *= 1 - s; gf *= 1 - s * f;
        break;
      case 4:
        rf *= 1 - s * (1 - f); gf *= 1 - s;
        break;
      case 5:
        gf *= 1 - s; bf *= 1 - s * f;
        break;
    }
  }
  r = static_cast<Uint8>(rf*255.f);
  g = static_cast<Uint8>(gf*255.f);
  b = static_cast<Uint8>(bf*255.f);
}

void Sys::KeyPress() {
  switch (event.key.keysym.sym) {
    case SDLK_UP:    commands |= D_UP;
      if (phase == GAME_TITLE && --cursol < (char)OPTION_1) cursol = OPTION_2;
      break;
    case SDLK_DOWN:  commands |= D_DOWN;
      if (phase == GAME_TITLE && ++cursol > (char)OPTION_2) cursol = OPTION_1;
      break;
    case SDLK_LEFT:  commands |= D_LEFT;     break;
    case SDLK_RIGHT: commands |= D_RIGHT;    break;
    case SDLK_F1:
    case SDLK_SPACE: commands |= CHARGE;
    case SDLK_RETURN:
      if (phase == -1) {
        if (cursol == OPTION_1) {
          phopt |= GAME_FROM_PR;
        }else if (cursol == OPTION_2) {
          phase = GAME_QUIT;
        }
      }
      break;
      //↓ポーズへの移行か、ポーズからの復旧
    case SDLK_LSHIFT:
    case SDLK_RSHIFT:
      if (!(phopt & GAME_POSED) && phase != GAME_TITLE) {
        phopt |= GAME_POSED;
        //renderPose();
      } else if (phopt & GAME_POSED) {
        phopt &= ~GAME_POSED;
      }
      break;
      
    default: break;
  }
}

void Sys::KeyRelease() {
  switch (event.key.keysym.sym) {
    case SDLK_UP:     commands &= ~D_UP;     break;
    case SDLK_DOWN:   commands &= ~D_DOWN;   break;
    case SDLK_LEFT:   commands &= ~D_LEFT;   break;
    case SDLK_RIGHT:  commands &= ~D_RIGHT;  break;
    case SDLK_SPACE:
    case SDLK_F1:     commands &= ~CHARGE;      break;
      
    default: break;
  }
}

void Sys::ResetWindow() {
  switch (event.window.event) {
    case SDL_WINDOWEVENT_HIDDEN:
      if (phase != GAME_TITLE) {
        phopt |= GAME_POSED;
      }
      break;
      
    default: break;
  }
}

void Sys::OnEvent() {
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:     KeyPress();    break;
      case SDL_KEYUP:       KeyRelease();  break;
      case SDL_WINDOWEVENT: ResetWindow(); break;
      case SDL_QUIT:
        if (!SDL_GetWindowFromID(event.window.windowID))
          phase = GAME_QUIT;
        break;
        
      default: break;
    }
  }
}

void Sys::renderTitle() {
  Uint8 r, g, b;
  cvtHSVintoRGB(0.5, 1.f - cursol, v_brightness*0.8, r, g, b);
  SDL_SetRenderDrawColor(renderer, r, g, b, 0);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

Sint8 Sys::OnExecute(Sint8 fps) {
  // Initialize application.
  
  // Enter the SDL event loop.
  
  while (phase) {
    OnEvent();
    
    switch (phase) {
      case GAME_QUIT: break;
        
      case GAME_TITLE:
        renderTitle();
        break;
        
      default:
        
        if (!(phopt & GAME_POSED)) {
          
          if (!(phopt & GAME_CHPHASE)) {
            
            
            
          } ///if not GAME_TO_NEXT
          
          //////// Do your drawing here
          
          SDL_RenderClear(renderer);
          
          SDL_RenderPresent(renderer);
          
        }///if not GAME_POSED
        
        break;
    }
    
    if (phopt & GAME_FROM_PR) {
      if (v_brightness <= 0) {
        phopt |= GAME_TO_NEXT;
      } else v_brightness -= 0.125;
    } else if (phopt & GAME_TO_NEXT) {
      if (v_brightness >= 1) {
        phopt &= ~GAME_TO_NEXT;
      } else v_brightness += 0.125;
    }
    if ((phopt & GAME_FROM_PR) && (phopt & GAME_TO_NEXT)) {
      phopt &= ~GAME_FROM_PR;
      (phase == GAME_TITLE)? phase = 1 : ++phase;
    }
    
    SDL_Delay(static_cast<int>(1000.0/fps));
  }
  
  return 0;
}
