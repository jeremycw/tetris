#include "events.h"

#define NULL 0

event_hdl* g_event_handlers[14] = {0};

unsigned int hash_event_name(const char* event);

int register_event_handler(const char* event, void (*f)(void*))
{
    int hash = hash_event_name(event);
    event_hdl* hdl = malloc(sizeof(event_hdl));
    if(hdl == NULL)
        return -1;
    if(g_event_handlers[hash] != NULL)
        hdl->next = g_event_handlers[hash];
    else
        hdl->next = NULL;
    hdl->callback = f;
    g_event_handlers[hash] = hdl;
    return 0;
}

void trigger_event(const char* event, void* data)
{
    int hash = hash_event_name(event);
    event_hdl* hdl = g_event_handlers[hash];
    while(hdl != NULL) {
        hdl->callback(data);
        hdl = hdl->next;
    }
}

unsigned int hash_event_name(const char* event)
{
    unsigned int highorder, h = 0, i = 0;
    while(event[i] != 0) {
        highorder = h & 0xf8000000;    // extract high-order 5 bits from h
        h = h << 5;                    // shift h left by 5 bits
        h = h ^ (highorder >> 27);     // move the highorder 5 bits to the low-order
        h = h ^ event[i];                    // XOR h and ki
        i++;
    }
    switch(h)
    {
    case 0x594dfe68: //AttemptMoveLeft
        return 0;
    case 0x2859caff: //AttemptMoveRight
        return 1;
    case 0x5949d452: //AttemptMoveDown
        return 2;
    case 0x5199df7a: //AttemptBlockRotate
        return 3;
    case 0xe637ec70: //MoveSucceeded
        return 4;
    case 0xd804af5f: //BlockHit
        return 5;
    case 0x9384eb: //BlockDrop
        return 6;
    case 0x1f5c09fc: //LineCleared
        return 7;
    case 0xef990ea4: //DoneClearing
        return 8;
    case 0xaa17b262: //NewBlock
        return 9;
    case 0xab00304c: //NewScore
        return 10;
    case 0xaaf356a5: //NewLevel
        return 11;
    case 0x831ae4db: //LogicReady
        return 12;
    case 0x9c7656f2: //GameOver
        return 13;

    }
}
