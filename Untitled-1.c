#include <stdio.h>

int main()
{
    unsigned char mask;
    unsigned char data = 0x31;
    unsigned char result;

    for (mask = 0x01; mask; mask <<= 1) {
        result |= (data & mask);
    }
    printf("Result: 0x%x\n", result);
    return 0;
}
