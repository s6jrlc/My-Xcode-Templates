#ifndef GSYSF_H
#define GSYSF_H

// ============================================================================
// [Include Section]
// ============================================================================

#define _USE_MATH_DEFINES
#include<cstdlib>
#include<ctime>
#include<cmath>

#if defined(__APPLE__)
# include<SDL2/SDL.h>
# include<SDL2_mixer/SDL_mixer.h>
# include<SDL2_image/SDL_image.h>
# include<SDL2_ttf/SDL_ttf.h>
#else
# include<SDL2/SDL.h>
# include<SDL2/SDL_mixer.h>
# include<SDL2/SDL_image.h>
# include<SDL2/SDL_ttf.h>
#endif

// ============================================================================
// [Definition Section]
// ============================================================================

#define _MAIN_ "TITLE"

#if defined(__APPLE__)
constexpr unsigned WINDOW_MODE = (SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
#else
constexpr unsigned WINDOW_MODE = SDL_WINDOW_SHOWN;
#endif

// ============================================================================
// [System class]
// ============================================================================

class Sys {
  // Whether the application is running.
  Sint8               phase, phopt, cursol;
  float               v_brightness;
  static Sint8        commands;
  
  SDL_Window*         window;
  SDL_Renderer*       renderer;
  SDL_Event           event;
  
  void                cvtHSVintoRGB(float, float, float, Uint8&, Uint8&, Uint8&);
  void                initState();
  
  // Called to process SDL event.
  void                KeyPress();
  void                KeyRelease();
  void                ResetWindow();
  void                OnEvent();
  
  void                renderTitle();
  
public:
  inline static Sint8 getCommand() { return commands; }
  inline static void  showSDLerror() {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "warning!", SDL_GetError(), 0);
  }
  
  // Run application, called by your code.
  Sint8               OnExecute(Sint8);
  Sys();
  ~Sys();
  
  // Application state
  enum {
    GAME_TITLE = -1, GAME_QUIT = 0,
    GAME_PHASE_LAST = 1,
    
    GAME_POSED      = 0b1,
    GAME_BOSS       = 0b10,
    GAME_BEATED     = 0b100,
    GAME_FROM_PR    = 0b1000,
    GAME_TO_NEXT    = 0b10000,
    
    GAME_CHPHASE    = GAME_FROM_PR | GAME_TO_NEXT,
    GAME_TO_LAST    = GAME_BOSS | GAME_TO_NEXT,
  };
  
  enum GSelect {
    OPTION_1, OPTION_2,
    OPTTOP = OPTION_1,
    OPTBTM = OPTION_2,
  };
  
  enum {
    D_UP = 0b1, D_DOWN = 0b10, D_LEFT = 0b100, D_RIGHT = 0b1000,
    CHARGE = 0b10000
  };
};

#endif //GSYSF_H