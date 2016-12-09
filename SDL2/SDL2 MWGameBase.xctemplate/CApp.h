#ifndef CAPP_H
#define CAPP_H

// ============================================================================
// [Include Section]
// ============================================================================

#include<string>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<boost/bimap/bimap.hpp>

#if defined(_WIN32) | defined(_WIN64)

#elif defined(__APPLE__)

#include<SDL2/SDL.h>
#include<SDL2_mixer/SDL_mixer.h>
#include<SDL2_image/SDL_image.h>
#include<SDL2_ttf/SDL_ttf.h>

#elif defined(__linux__)

#include<SDL2/SDL.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

#endif

#define _MAIN_ "TITLE"

using std::string;
typedef boost::bimaps::bimap<string, SDL_Window*>   bimap_sw;
typedef boost::bimaps::bimap<string, SDL_Renderer*> bimap_sr;
typedef bimap_sw::value_type valt_sw;
typedef bimap_sr::value_type valt_sr;

// ============================================================================
// [CApp]
// ============================================================================
// This is just a sample SDL application class to test that the app works.

class CApp {
    // Whether the application is running.
    string              title;
    static char         commands, gphase, gfphase, cursol, count;
    
    bimap_sw            bi_sw;
    bimap_sr            bi_sr;
    static SDL_Event    event;
    
    // Called to process SDL event.
    void                KeyPress();
    void                KeyRelease();
    void                ResetWindow();
    void                OnEvent();
    
public:
    inline static char  getCommand() {return commands;}
    int                 addRWToBimap(string, int, int, int, int, unsigned);
    void                removeRWFromBimap(string);
    void                removeRWFromBimap(SDL_Window*);
    
    // Run application, called by your code.
    int                 OnExecute(int);
    CApp();
    ~CApp();
    
    // Application state
    enum APP_STATE {
        APP_OK = 0,
        APP_FAILED = 1
    };
    enum GPhase {GAME_POSED = -4,
        GAME_READY_TO_END, GAME_END, GAME_QUIT, TITLE, GAME_PHASE_1, GAME_PHASE_2};
    enum GSelect {OPTION_1, OPTION_2};
};

enum ACTION {
    MOVE_UP = 0b1, MOVE_DOWN = 0b10, MOVE_LEFT = 0b100, MOVE_RIGHT = 0b1000,
    CHARGE = 0b10000
};

#endif //CAPP_H