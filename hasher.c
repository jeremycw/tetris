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
    while(c != EOF)
    {
        if(c == '\n')
        {
            string[pos] = 0;
            printf("    case 0x%x: //%s\n", hash(string), string);
            printf("        return %d;\n", count);
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
}

