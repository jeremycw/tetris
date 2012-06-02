#include <stdio.h>

unsigned int hash(char* string)
{
    unsigned int highorder, h = 0, i = 0;
    while(string[i] != 0) {
        highorder = h & 0xf8000000;    // extract high-order 5 bits from h
        h = h << 5;                    // shift h left by 5 bits
        h = h ^ (highorder >> 27);     // move the highorder 5 bits to the low-order
        h = h ^ string[i];                  // XOR h and ki
        i++;
    }
    return h;
}

int main()
{
    char string[128];
    char c = getchar();
    int pos = 0;
    int count = 0;

    FILE* eventsh = fopen("events.h", "w");

    const char* head = 
        "#ifndef EVENTS_H\n"
        "#define EVENTS_H\n\n"
        "typedef struct event_hdl {\n"
        "    struct event_hdl* next;\n"
        "    void (*callback)(void*);\n"
        "} event_hdl;\n\n"
        "int register_event_handler(const char* event, void (*f)(void*));\n"
        "void trigger_event(const char* event, void* data);\n\n"
        "#define HASHES ";
    fprintf(eventsh, head);
    while(c != EOF)
    {
        if(c == '\n')
        {
            string[pos] = 0;
            fprintf(eventsh, "case 0x%x: /*%s*/ \\\n", hash(string), string);
            fprintf(eventsh, "    return %d; \\\n", count);
            pos = 0;
            count++;
        }
        else
        {
            string[pos] = c;
            pos++;
        }
        c = getchar();
    }
    fprintf(eventsh, "\n#define NUM_EVENTS %d\n\n", count-1);
    fprintf(eventsh, "#endif\n");
    fclose(eventsh);
}

