#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define dot "/dev/dot"

int main()
{
    int dot_d, i;
    unsigned char c[26][8] = {{0x18, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42},   //A
                {0x3c, 0x22, 0x22, 0x3c, 0x22, 0x22, 0x22, 0x3c},   //B
                {0x1C, 0x22, 0x20, 0x20, 0x20, 0x20, 0x22, 0x1C},   //C
                {0x38, 0x44, 0x42, 0x42, 0x42, 0x42, 0x44, 0x38},   //D
                {0x3E, 0x20, 0x20, 0x3E, 0x20, 0x20, 0x20, 0x3E},   //E
                {0x3E, 0x20, 0x20, 0x3E, 0x20, 0x20, 0x20, 0x20},   //F
                {0x1C, 0x22, 0x42, 0x40, 0x40, 0x47, 0x42, 0x3C},   //G
                {0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42},   //H
                {0x1C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C},   //I
                {0x1C, 0x08, 0x08, 0x08, 0x08, 0x48, 0x48, 0x30},   //J
                {0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x44},   //K
                {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3E},   //L
                {0x81, 0xC3, 0xA5, 0x99, 0x81, 0x81, 0x81, 0x81},   //M
                {0x42, 0x62, 0x52, 0x4A, 0x46, 0x42, 0x42, 0x42},   //N
                {0x3C, 0x42, 0x81, 0x81, 0x81, 0x81, 0x42, 0x3C},   //O
                {0x7C, 0x42, 0x42, 0x42, 0x7C, 0x40, 0x40, 0x40},   //P
                {0x38, 0x44, 0x82, 0x82, 0x82, 0x8A, 0x44, 0x3A},   //Q
                {0x7C, 0x42, 0x42, 0x42, 0x7C, 0x48, 0x44, 0x42},   //R
                {0x3C, 0x42, 0x40, 0x3C, 0x02, 0x02, 0x42, 0x3C},   //S
                {0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08},   //T
                {0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C},   //U
                {0x81, 0x42, 0x42, 0x42, 0x24, 0x24, 0x24, 0x18},   //V
                {0x81, 0x99, 0x99, 0x99, 0x99, 0x99, 0x5A, 0x24},   //W
                {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81},   //X
                {0x81, 0x81, 0x42, 0x42, 0x3C, 0x18, 0x18, 0x18},   //Y
                {0xFF, 0x03, 0x02, 0x04, 0x08, 0x10, 0x60, 0xFF}    //Z
                };
    if((dot_d = open(dot, O_RDWR)) < 0)
    {
        printf("Can't Open\n");
        exit(0);
    }

    for(i=0;i<26;i++)
    {
        write(dot_d,&c[i],sizeof(c[i]));
        sleep(1);
    }
    close(dot_d);

    return 0;
}