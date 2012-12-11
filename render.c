#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "events.h"
#include "tetris.h"

static int grid[20][14] = {0};

static int type = 0;
static int next_type = 0;
static int row = 0;
static int col = 6;
static int rot = 0;

static SDL_Surface* screen = 0;
static TTF_Font* font;
static TTF_Font* over_font;
static SDL_Color text_colour = { 255, 255, 255 }; 

static void draw_block(int type, int row, int col)
{
    SDL_Rect rect;
    rect.x = col*BLOCK_WIDTH;
    rect.y = row*BLOCK_HEIGHT;
    rect.w = BLOCK_WIDTH;
    rect.h = BLOCK_HEIGHT;

    switch(type)
    {
    case 0:
        SDL_FillRect(screen, &rect, 0x00000000);
        break;
    case 1:
        SDL_FillRect(screen, &rect, 0x0000FF00);
        break;
    case 2:
        SDL_FillRect(screen, &rect, 0x00FF0000);
        break;
    case 3:
        SDL_FillRect(screen, &rect, 0x000000FF);
        break;
    case 4:
        SDL_FillRect(screen, &rect, 0x00FFFF00);
        break;
    case 5:
        SDL_FillRect(screen, &rect, 0x00FFFFFF);
        break;
    case 6:
        SDL_FillRect(screen, &rect, 0x00FF00FF);
        break;
    case 7:
        SDL_FillRect(screen, &rect, 0x0000FFFF);
        break;
    }
}

static void render_grid()
{
    int i,j;
    int data[14];
    for(i = 0; i < 20; i++)
    {
        if(i >= row && i < row+4)
            get_block_seg(i-row, col, rot, type, data);
        for(j = 0; j < 14; j++)
        {
            if(i >= row && i < row+4 && j >= col && j < col+4)
            {
                if(data[j])
                    draw_block(type+1, i, j);
                else
                    draw_block(grid[i][j], i, j);
            }
            else
                draw_block(grid[i][j], i, j);
        }
    }
    SDL_Flip(screen);
}

void render_next_block()
{
    int i,j;
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            if(blocks[next_type][0][i][j])
                draw_block(next_type+1, 2+i, 14+j);
            else
                draw_block(0, 2+i, 14+j);
        }
    }
    SDL_Flip(screen);
}

static void cement_block()
{
    int i,j;
    int data[14];
    for(i = 0; i < 4; i++)
    {
        get_block_seg(i, col, rot, type, data);
        for(j = 0; j < 14; j++)
        {
            if(row+i < 20)
                grid[row+i][j] = data[j] ? type+1 : grid[row+i][j];
        }
    }
}

void init_renderer()
{
    register_event_handler("MoveSucceeded", renderer_MoveSucceeded);
    register_event_handler("NewBlock", renderer_NewBlock);
    register_event_handler("LogicReady", renderer_LogicReady);
    register_event_handler("LineCleared", renderer_LineCleared);
    register_event_handler("DoneClearing", renderer_DoneClearing);
    register_event_handler("NewScore", renderer_NewScore);
    register_event_handler("BlockHit", renderer_BlockHit);
    register_event_handler("NewLevel", renderer_NewLevel);
    register_event_handler("GameOver", renderer_GameOver);
    screen = SDL_SetVideoMode( 436, 480, 32, SDL_SWSURFACE );
    if(screen == NULL)
    {
        //cont = 0;
        printf("Failed to init screen!\n");
    }
    TTF_Init();
    font = TTF_OpenFont("font.ttf", 14);
    over_font = TTF_OpenFont("font.ttf", 48);
    if(font == NULL)
    {
        //cont = 0;
        printf("Failed to load font!\n");
    }
    SDL_Rect rect = {336, 0, 100, 480};
    SDL_FillRect(screen, &rect, 0x00555753);
    SDL_Rect rect2 = {15, 96, 300, 80};
    SDL_FillRect(screen, &rect2, 0x00555753);
    SDL_Surface* splash = TTF_RenderText_Solid( over_font, "Tetris", text_colour);
    SDL_Rect rect3 = {75, 100};
    SDL_BlitSurface(splash, NULL, screen, &rect3); 
    SDL_FreeSurface(splash);
    splash = TTF_RenderText_Solid( font, "Press Enter to play.", text_colour);
    SDL_Rect rect4 = {80, 150};
    SDL_BlitSurface(splash, NULL, screen, &rect4); 
    SDL_FreeSurface(splash);
}

void renderer_MoveSucceeded(void* data)
{
    switch((long)data)
    {
    case LEFT:
        col--;
        break;
    case RIGHT:
        col++;
        break;
    case DOWN:
        row++;
        break;
    case ROTATE:
        rot = (rot+1)%4;
        break;
    }
    render_grid();
}

void renderer_NewBlock(void* data)
{
    type = next_type;
    next_type = (long)data;
    row = 0;
    col = 6;
    rot = 0;
    render_next_block();
    render_grid();
}

void renderer_BlockHit(void* data)
{
    cement_block();
}

void renderer_LineCleared(void* data)
{
    copy_down((int)data, grid);
}

void renderer_DoneClearing(void* data)
{
    render_grid();
}

void renderer_LogicReady(void* data)
{
    int* types = (int*)data;
    type = types[0];
    next_type = types[1];
    render_next_block();
    //render_grid();
}

void renderer_NewScore(void* data)
{
    char str[13];
    SDL_Rect offset;
    SDL_Surface* score;
    offset.x = 336;
    offset.y = 0;
    sprintf(str, "Score: %d", (int)data);
    score = TTF_RenderText_Solid( font, str, text_colour);
    SDL_Rect rect = {336, 0, 100, 15};
    SDL_FillRect(screen, &rect, 0x00555753);
    SDL_BlitSurface(score, NULL, screen, &offset); 
    SDL_FreeSurface(score);
    SDL_Flip(screen);
}

void renderer_NewLevel(void* data)
{
    char str[13];
    SDL_Rect offset;
    SDL_Surface* level;
    offset.x = 336;
    offset.y = 15;
    sprintf(str, "Level: %d", (int)data);
    level = TTF_RenderText_Solid( font, str, text_colour);
    SDL_Rect rect = {336, 15, 100, 15};
    SDL_FillRect(screen, &rect, 0x00555753);
    SDL_BlitSurface(level, NULL, screen, &offset); 
    SDL_FreeSurface(level);
    SDL_Flip(screen);
}

void renderer_GameOver(void* data)
{
    SDL_Surface* game_over;
    TTF_Font* over_font = TTF_OpenFont("font.ttf", 48);
    game_over = TTF_RenderText_Solid( over_font, "Game Over!", text_colour);
    SDL_Rect rect = {20, 150};
    SDL_BlitSurface(game_over, NULL, screen, &rect); 
    SDL_FreeSurface(game_over);
    SDL_Flip(screen);
}

