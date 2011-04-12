#ifndef EVENTS_H
#define EVENTS_H

typedef struct event_hdl {
    struct event_hdl* next;
    void (*callback)(void*);
} event_hdl;

int register_event_handler(const char* event, void (*f)(void*));
void trigger_event(const char* event, void* data);

#endif
