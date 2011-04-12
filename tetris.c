#include <stdlib.h>
#include <time.h>
#include "events.h"
#include "tetris.h"

static int grid[20][14] = {0};   

int blocks[7][4][4][4] = {{{{0,1,0,0},
                      {0,1,0,0},
                      {1,1,0,0},
                      {0,0,0,0}},
                     {{1,0,0,0},
                      {1,1,1,0},
                      {0,0,0,0},
                      {0,0,0,0}},
                     {{0,1,1,0},
                      {0,1,0,0},
                      {0,1,0,0},
                      {0,0,0,0}},
                     {{0,0,0,0},
                      {1,1,1,0},
                      {0,0,1,0},
                      {0,0,0,0}}},

                     {{{0,1,0,0},
                      {0,1,0,0},
                      {0,1,0,0},
                      {0,1,0,0}},
                     {{0,0,0,0},
                      {1,1,1,1},
                      {0,0,0,0},
                      {0,0,0,0}},
                     {{0,1,0,0},
                      {0,1,0,0},
                      {0,1,0,0},
                      {0,1,0,0}},
                     {{0,0,0,0},
                      {1,1,1,1},
                      {0,0,0,0},
                      {0,0,0,0}}},

                    {{{1,1,0,0},
                      {0,1,1,0},
                      {0,0,0,0},
                      {0,0,0,0}},
                     {{0,0,1,0},
                      {0,1,1,0},
                      {0,1,0,0},
                      {0,0,0,0}},
                     {{1,1,0,0},
                      {0,1,1,0},
                      {0,0,0,0},
                      {0,0,0,0}},
                     {{0,0,1,0},
                      {0,1,1,0},
                      {0,1,0,0},
                      {0,0,0,0}}},

                    {{{0,1,0,0},
                      {0,1,0,0},
                      {0,1,1,0},
                      {0,0,0,0}},
                     {{0,0,0,0},
                      {1,1,1,0},
                      {1,0,0,0},
                      {0,0,0,0}},
                     {{1,1,0,0},
                      {0,1,0,0},
                      {0,1,0,0},
                      {0,0,0,0}},
                     {{0,0,1,0},
                      {1,1,1,0},
                      {0,0,0,0},
                      {0,0,0,0}}},

                    {{{1,1,0,0},
                      {1,1,0,0},
                      {0,0,0,0},
                      {0,0,0,0}},
                     {{1,1,0,0},
                      {1,1,0,0},
                      {0,0,0,0},
                      {0,0,0,0}},
                     {{1,1,0,0},
                      {1,1,0,0},
                      {0,0,0,0},
                      {0,0,0,0}},
                     {{1,1,0,0},
                      {1,1,0,0},
                      {0,0,0,0},
                      {0,0,0,0}}},

                    {{{1,1,1,0},
                      {0,1,0,0},
                      {0,0,0,0},
                      {0,0,0,0}},
                     {{0,0,1,0},
                      {0,1,1,0},
                      {0,0,1,0},
                      {0,0,0,0}},
                     {{0,0,0,0},
                      {0,1,0,0},
                      {1,1,1,0},
                      {0,0,0,0}},
                     {{1,0,0,0},
                      {1,1,0,0},
                      {1,0,0,0},
                      {0,0,0,0}}},

                    {{{0,1,1,0},
                      {1,1,0,0},
                      {0,0,0,0},
                      {0,0,0,0}},
                     {{0,1,0,0},
                      {0,1,1,0},
                      {0,0,1,0},
                      {0,0,0,0}},
                     {{0,1,1,0},
                      {1,1,0,0},
                      {0,0,0,0},
                      {0,0,0,0}},
                     {{0,1,0,0},
                      {0,1,1,0},
                      {0,0,1,0},
                      {0,0,0,0}}}};
static int type = 0;
static int next_type = 0;
static int row = 0;
static int col = 6;
static int rot = 0;
static int score = 0;
static int total_lines = 0;
static int level = 1;

void init_logic()
{
    srand(time(NULL));
    type = rand()%7;
    next_type = rand()%7;
    int types[2];
    types[0] = type;
    types[1] = next_type;
    register_event_handler("AttemptMoveLeft", logic_AttemptMoveLeft);
    register_event_handler("AttemptMoveRight", logic_AttemptMoveRight);
    register_event_handler("AttemptMoveDown", logic_AttemptMoveDown);
    register_event_handler("BlockDrop", logic_BlockDrop);
    register_event_handler("AttemptBlockRotate", logic_AttemptBlockRotate);
    trigger_event("LogicReady", (void*)types);
    trigger_event("NewScore", 0);
    trigger_event("NewLevel", 1);
}
    

int get_block_seg(int seg, int col, int rot, int type, int* data)
{
    int i;
    for(i = 0; i < 14; i++)
        data[i] = 0;
    for(i = 0; i < 4; i++)
    {
        if(col+i < 14 && col+i >= 0)
            data[col+i] = blocks[type][rot][seg][i];
        else if(blocks[type][rot][seg][i] == 1)
            return -1;
    }       
    return 0;
}

static int move(int dir)
{
    switch(dir)
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
    int rc = check_move();
    if(rc == 1)
    {
        switch(dir)
        {
        case LEFT:
            col++;
            break;
        case RIGHT:
            col--;
            break;
        case DOWN:
            row--;
            break;
        case ROTATE:
            rot = rot == 0 ? 3 : rot-1;
            break;
        }
        if(dir == DOWN)
        {
            cement_block();
            trigger_event("BlockHit", NULL);
            int lines = check_lines();
            if(lines)
                calc_score(lines);
            new_block();
       }
    }
    else
    {
        trigger_event("MoveSucceeded", (void*)dir);
    }
    return rc;
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
                grid[row+i][j] = data[j] | grid[row+i][j];
        }
    }
}

static int check_lines()
{
    int i,j;
    int total = 0;
    int lines = 0;
    for(i = 0; i < 4; i++)
    {
        if(row+i < 20)
        {
            for(j = 0; j < 14; j++)
            {
                total += grid[row+i][j];
            }
        }
        if(total == 14)
        {
            lines++;
            copy_down(row+i, grid);
            trigger_event("LineCleared", (void*)row+i);
        }
        total = 0;
    }
    trigger_event("DoneClearing", (void*)row+i);
    return lines;
}

void copy_down(int row, int grid[20][14])
{
    while(row > 0)
    {
        int i;
        for(i = 0; i < 14; i++)
            grid[row][i] = grid[row-1][i];
        row--;
    }
}

static int check_move()
{
    int i,j;
    int data[14];
    for(i = 0; i < 4; i++)
    {
        if(get_block_seg(i, col, rot, type, data) == -1)
            return 1;
        for(j = 0; j < 14; j++)
        {
            if(row+i >= 20 && data[j])
                return 1;
            if(data[j] & grid[row+i][j])
                return 1;
        }
    }
    return 0;
}

static void new_block()
{
    row = 0;
    col = 6;
    rot = 0;
    type = next_type;
    next_type = rand()%7;
    trigger_event("NewBlock", (void*)next_type);
    if(check_move())
        trigger_event("GameOver", NULL);
}

static void calc_score(int lines)
{
    score = score + lines*10 * level;
    type = rand()%7;
    trigger_event("NewScore", (void*)score);
    total_lines += lines;
    if(total_lines >= 10)
    {
        level++;
        total_lines = 0;
        trigger_event("NewLevel", (void*)level);
    }
}

void logic_AttemptMoveLeft(void* data)
{
    move(LEFT);
}

void logic_AttemptMoveRight(void* data)
{
    move(RIGHT);
}

void logic_AttemptMoveDown(void* data)
{
    move(DOWN);
}

void logic_BlockDrop(void* data)
{
    while(move(DOWN) != 1);
}

void logic_AttemptBlockRotate(void* data)
{
    move(ROTATE);
}


