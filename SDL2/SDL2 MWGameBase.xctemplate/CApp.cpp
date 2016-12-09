// ============================================================================
// [Include Section]
// ============================================================================
#include"CApp.h"

// ============================================================================
// [Defines & Constants]
// ============================================================================

char CApp::gphase = 0, CApp::gfphase = 0, CApp::commands = 0, CApp::cursol = 0, CApp::count = 0;
SDL_Event CApp::event;

CApp::CApp() {
    // Initialize the SDL library.
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "SDL_Init() failed: %s\n", SDL_GetError());
        gphase = GAME_QUIT;
    }
    if (addRWToBimap(_MAIN_,
                     SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                     480, 800,
                     SDL_WINDOW_SHOWN)
        ) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "warning!", SDL_GetError(), 0);
        gphase = GAME_QUIT;
    }
}
CApp::~CApp() {
    if (!bi_sw.empty()) bi_sw.clear();
    if (!bi_sr.empty()) bi_sr.clear();
    SDL_Quit();
}

int CApp::OnExecute(int f) {
    // Initialize application.
    bimap_sr::iterator srit;
    
    // Enter the SDL event loop.
    
    while (gphase != GAME_QUIT) {
        OnEvent();
        
        switch (gphase) {
            case TITLE:

                break;
            case GAME_PHASE_1:
                
                // Update your game logic here

                for (srit = bi_sr.begin(); srit != bi_sr.end(); ++srit) {
                    SDL_RenderClear(srit->right);
                }
                
                // Do your drawing here
                
                for (srit = bi_sr.begin(); srit != bi_sr.end(); ++srit) {
                    SDL_RenderPresent(srit->right);
                }
                
            default:
                break;
        }
        
        SDL_Delay(static_cast<int>(1000.0/f));
    }
    
    return 0;
}

int CApp::addRWToBimap(string sk, int x, int y, int w, int h, unsigned f) {
    bi_sw.insert(valt_sw(sk, SDL_CreateWindow(sk.c_str(), x, y, w, h, f)));
    if (!bi_sw.left.count(sk)) {
        return -1;
    } else {
        bi_sr.insert(valt_sr(sk, SDL_CreateRenderer(bi_sw.left.at(sk), -1,
                                                    SDL_RENDERER_PRESENTVSYNC)));
    }
    if (!bi_sr.left.count(sk)) {
        return -1;
    }
    return 0;
}

void CApp::removeRWFromBimap(string sk) {
    SDL_DestroyRenderer(bi_sr.left.at(sk));
    SDL_DestroyWindow(bi_sw.left.at(sk));
    bi_sr.left.erase(sk);
    bi_sw.left.erase(sk);
}
void CApp::removeRWFromBimap(SDL_Window* wk) {
    SDL_DestroyRenderer(bi_sr.left.at(bi_sw.right.at(wk)));
    SDL_DestroyWindow(wk);
    bi_sr.left.erase(bi_sw.right.at(wk));
    bi_sw.right.erase(wk);
}

void CApp::KeyPress() {
    switch (event.key.keysym.sym) {
        case SDLK_UP:    commands |= MOVE_UP;
            if (gphase == TITLE && --cursol < (char)OPTION_1)
                cursol = OPTION_2;
            break;
        case SDLK_DOWN:  commands |= MOVE_DOWN;
            if (gphase == TITLE && ++cursol > (char)OPTION_2)
                cursol = OPTION_1;
            break;
        case SDLK_LEFT:  commands |= MOVE_LEFT;     break;
        case SDLK_RIGHT: commands |= MOVE_RIGHT;    break;
        case SDLK_F1:
        case SDLK_SPACE: commands |= CHARGE;        break;
            //↓ポーズへの移行か、ポーズからの復旧
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            if (gphase > TITLE) {
                gfphase = gphase;
                gphase = GAME_POSED;
                //showPose();
            } else if (SDL_GetWindowFlags(bi_sw.left.at(_MAIN_)) & SDL_WINDOW_SHOWN &&
                       gphase == GAME_POSED)
                gphase = gfphase;
            break;
        case SDLK_RETURN:
            if (gphase == TITLE) {
                if (cursol == OPTION_1) {
                    gphase = GAME_PHASE_1;
                }else if (cursol == OPTION_2) {
                    gphase = GAME_QUIT;
                }
            }
            break;
        case SDLK_s:
            if (!bi_sr.left.count("status")) {
                int y, h; SDL_Window* wpt = bi_sw.left.at(_MAIN_);
                SDL_GetWindowPosition(wpt, 0, &y);
                SDL_GetWindowSize(wpt, 0, &h);
                addRWToBimap("status",
                             SDL_WINDOWPOS_CENTERED, y+h,
                             480, 40,
                             SDL_WINDOW_SHOWN// | SDL_WINDOW_BORDERLESS
                             );
            } else {
                removeRWFromBimap("status");
            }
            
        default: break;
    }
}

void CApp::KeyRelease() {
    switch (event.key.keysym.sym) {
        case SDLK_UP:     commands &= ~MOVE_UP;     break;
        case SDLK_DOWN:   commands &= ~MOVE_DOWN;   break;
        case SDLK_LEFT:   commands &= ~MOVE_LEFT;   break;
        case SDLK_RIGHT:  commands &= ~MOVE_RIGHT;  break;
        case SDLK_SPACE:
        case SDLK_F1:     commands &= ~CHARGE;      break;
            
        default: break;
    }
}

void CApp::ResetWindow() {
    switch (event.window.event) {
        case SDL_WINDOWEVENT_HIDDEN:
            if (SDL_GetWindowFromID(event.window.windowID) == bi_sw.left.at(_MAIN_) &&
                gphase > TITLE) {
                gfphase = gphase;
                gphase = GAME_POSED;
            }
            break;
        case SDL_WINDOWEVENT_SHOWN:
            if (SDL_GetWindowFromID(event.window.windowID) == bi_sw.left.at(_MAIN_) &&
                gphase == GAME_POSED)
                gphase = gfphase;
            break;
        case SDL_WINDOWEVENT_CLOSE:
            if (SDL_GetWindowFromID(event.window.windowID) == bi_sw.left.at(_MAIN_))
                gphase = GAME_QUIT;
            else removeRWFromBimap(SDL_GetWindowFromID(event.window.windowID));
            break;
            
        default: break;
    }
}

void CApp::OnEvent() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:     KeyPress();    break;
            case SDL_KEYUP:       KeyRelease();  break;
            case SDL_WINDOWEVENT: ResetWindow(); break;
            case SDL_QUIT:
                if (!SDL_GetWindowFromID(event.window.windowID))
                    gphase = GAME_QUIT;
                break;
                
            default: break;
        }
    }
}
