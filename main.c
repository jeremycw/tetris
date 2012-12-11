#include "SDL/SDL.h"
#include "events.h"
#include "tetris.h"

static int cont = 1;
static int count = 0;
static int level = 1;
static int state = SPLASH;

void handle_input()
{
    SDL_Event event;
    if( SDL_PollEvent(&event) )
    {
        if( event.type == SDL_KEYDOWN )
        { 
            switch( event.key.keysym.sym )
            {
            case SDLK_DOWN:
                if(state == PLAY)
                    trigger_event("BlockDrop", NULL);
                break;
            case SDLK_LEFT:
                if(state == PLAY)
                    trigger_event("AttemptMoveLeft", NULL);
                break;
            case SDLK_RIGHT:
                if(state == PLAY)
                    trigger_event("AttemptMoveRight", NULL);
                break;
            case SDLK_SPACE:
                if(state == PLAY)
                    trigger_event("AttemptBlockRotate", NULL);
                break;
            case SDLK_RETURN:
                if(state == SPLASH)
                    state = PLAY;
                break;
            case SDLK_ESCAPE:
                cont = 0;
                break;
            }
        }
        else if( event.type == SDL_QUIT ) 
        {
            cont = 0;
        } 
    } 
}

void main_GameOver(void* data)
{
    state = GAME_OVER;
}

void main_NewLevel(void* data)
{
    level = (int)data;
}

void init_main()
{
    register_event_handler("GameOver", main_GameOver);
    register_event_handler("NewLevel", main_NewLevel);
}

int main(int argc, char* argv[])
{
    int frame_start = 0;
    int frame_end = 0;
    int frame_time = 0;
    SDL_Init( SDL_INIT_EVERYTHING );
    init_main();
    init_renderer();
    init_logic();
    while(cont)
    {
        frame_start = SDL_GetTicks();
        handle_input();
        count++;
        if(count >= 15 - level-1 && state == PLAY)
        {
            count = 0;
            trigger_event("AttemptMoveDown", NULL);
        }
        frame_end = SDL_GetTicks();
        frame_time = frame_end - frame_start;
        if(frame_time < 1000/FPS)
            SDL_Delay(1000/FPS - frame_time);
    }
    SDL_Quit();
    return 0; 
}
