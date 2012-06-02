#include "events.h"

#define NULL 0

event_hdl* g_event_handlers[NUM_EVENTS] = {0};
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
      HASHES
    }
}
