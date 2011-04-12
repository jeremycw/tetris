#ifndef TETRIS_H
#define TETRIS_H

#define BLOCK_WIDTH 24
#define BLOCK_HEIGHT 24

#define LEFT 0
#define RIGHT 1
#define DOWN 2
#define ROTATE 3

#define PLAY 0
#define GAME_OVER 1
#define SPLASH 2

#ifndef NULL
#define NULL 0
#endif

#define FPS 15

void init_logic();
int get_block_seg(int seg, int col, int rot, int type, int* data);
static int move(int dir);
static void cement_block();
static int check_lines();
static int check_move();
void copy_down(int row, int grid[20][14]);
static void new_block();
static void calc_score(int lines);
void logic_AttemptMoveLeft(void* data);
void logic_AttemptMoveRight(void* data);
void logic_AttemptMoveDown(void* data);
void logic_BlockDrop(void* data);
void logic_AttemptBlockRotate(void* data);

void renderer_NewBlock(void* data);
void renderer_NewScore(void* data);
void renderer_BlockHit(void* data);
void renderer_MoveSucceeded(void* data);
void renderer_LogicReady(void* data);
void renderer_LineCleared(void* data);
void renderer_DoneClearing(void* data);
void renderer_NewLevel(void* data);
void renderer_GameOver(void* data);
static void draw_block(int type, int row, int col);
static void render_grid();
void init_renderer();

void main_NewLevel(void* data);

void init_renderer();
void init_logic();

extern blocks[7][4][4][4];

#endif
