/**
@file blinker.c
@brief LED blinker demo for light52 core.

Does nothing but send a greeting string to the console and do a binary count
on port P0.
Should fit in 4K of ROM and use no XRAM.
This demo may come in handy to try the core on boards with no RS232 port or
display.
*/
#include <stdio.h>
#include <string.h>
#include "../../include/light52.h"
#include "../../common/soc.h"


/*-- Local function prototypes -----------------------------------------------*/
// char *gets(char *p,int s);
int xtoi(char *s1);
int atoi(char *s1);
// char *strtok(char *str,char *dptr);
void delay();

volatile uint8_t seconds;
uint8_t LCD_talbe[11] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6, 0x01};
uint8_t enlcd = 0;

/*-- Public functions --------------------------------------------------------*/
// int i;
void main(void)
{

    // uint8_t num;
    // uint16_t i,j = 0;

    /* Initialize the support code: timer0 set to count seconds. */
    /* The UART is left in its default reset state: 115200-8-N-1 */
    soc_init();

    /* Send a banner to the serial port, in case it is connected. */


    char  buffer[16];
    int c;
    char *p; 

    int s = 0;
    char *argv[4];
    int   argc;
    p = buffer;

    printf("\n");
    printf("> C52 on FPGA project -- F520 cjh\n");
    printf("> valid commands: led <hex>, sw, seconds, \n");
    printf("> valid commands: enlcd <hex>, P0 <hex>, P1 <hex>\n");
    while(1)
    {
        if(RXRDY)
        {
            c = SBUF;
            printf("> rx = 0x%02x num = %d\n", c, s);
            if ((c=='\n'||c=='\r') && (s == 0)) // if get CR or LF before command
            {
                // reset rx buffer
                memset(buffer,0,16);
                memset(argv, 0, 4);
                argc = 0;
                p = buffer; 
                s = 0;
            }
            else if ((c=='\n'||c=='\r') && (s > 1)) // deal with command 
            {
                *p = 0;

                for(argc=0;argc<4 && (argv[argc]=strtok(argc==0?buffer:NULL," "));argc++) 
                    ; // split command by space "arg[0] arg[1]"

                if(argv[0])
                {
                    printf("> argv[0] = %s\n",argv[0]);
                    printf("> argv[1] = %s\n",argv[1]);
                    if(!strcmp(argv[0],"led"))
                    {
                        if(argv[1]) P0 = (xtoi(argv[1]) & 0x0F) | (P0 & 0xf0);
                        printf("> led = %x\n",P0 & 0x0F);
                    }
                    else if(!strcmp(argv[0],"sw"))
                    {
                        printf("> sw = %x\n",P2 & 0x3);
                    }
                    else if(!strcmp(argv[0],"seconds"))
                    {
                        printf("> seconds = %d\n",seconds);
                    }
                    else if(!strcmp(argv[0],"P0"))
                    {
                        if(argv[1]) P0 = xtoi(argv[1]);
                        printf("> P0 = %d\n",P0);
                    }
                    else if(!strcmp(argv[0],"P1"))
                    {
                        if(argv[1]) P1 = xtoi(argv[1]);
                        printf("> P1 = %d\n",P1);
                    }
                    else if(!strcmp(argv[0],"enlcd"))
                    {
                        if(atoi(argv[1]) == 0x01)
                            enlcd = 1;
                        else if (atoi(argv[1]) == 0x00)
                            enlcd = 0;
                        printf("> enlcd = %d\n",enlcd);
                    }
                    else if(argv[0][0])
                    {
                            printf("> valid commands: led <hex>, sw, seconds, \n");
                            printf("> valid commands: enlcd <hex>, P0 <hex>, P1 <hex>\n");
                    } 
                }
                // reset rx buffer
                memset(buffer,0,16);
                memset(argv, 0, 4);
                argc = 0;
                p = buffer; 
                s = 0;
            }
            else if(c=='\b') // backspace!
            {
                printf("> backspace\n");
                if(p!=buffer) 
                {
                    *--p = 0;
                }
            }
            else
            { 
                *p++ = c;   
                s++; 
            }
        }
        else
        {
            if (enlcd == 1)
            {
                if (seconds < 10)
                {
                    // enable tens lCD
                    P0 = 0x40 | (P0 & 0x3F);
                    P1 = LCD_talbe[0];
                    // remove ghost
                    delay(); P0 = 0xC0 | (P0 & 0x3F);
                    P0 = 0x80 | (P0 & 0x3F);
                    // enable ones lCD
                    P1 = LCD_talbe[seconds];
                    delay();P0 = 0xC0 | (P0 & 0x3F);
                }
                else if ((seconds >= 10) && (seconds < 20))
                {
                    P0 = 0x40 | (P0 & 0x3F);
                    P1 = LCD_talbe[1];
                    delay();P0 = 0xC0 | (P0 & 0x3F);
                    P0 = 0x80 | (P0 & 0x3F);
                    P1 = LCD_talbe[seconds-10];
                    delay();P0 = 0xC0 | (P0 & 0x3F);
                }
                else if ((seconds >= 20) && (seconds < 30))
                {
                    P0 = 0x40 | (P0 & 0x3F);
                    P1 = LCD_talbe[2];
                    delay();P0 = 0xC0 | (P0 & 0x3F);
                    P0 = 0x80 | (P0 & 0x3F);
                    P1 = LCD_talbe[seconds-20];
                    delay();P0 = 0xC0 | (P0 & 0x3F);
                }
                else if ((seconds >= 30) && (seconds < 40))
                {
                    P0 = 0x40 | (P0 & 0x3F);
                    P1 = LCD_talbe[3];
                    delay();P0 = 0xC0 | (P0 & 0x3F);
                    P0 = 0x80 | (P0 & 0x3F);
                    P1 = LCD_talbe[seconds-30];
                    delay();P0 = 0xC0 | (P0 & 0x3F);
                }
                else if ((seconds >= 40) && (seconds < 50))
                {
                    P0 = 0x40 | (P0 & 0x3F);
                    P1 = LCD_talbe[4];
                    delay();P0 = 0xC0 | (P0 & 0x3F);
                    P0 = 0x80 | (P0 & 0x3F);
                    P1 = LCD_talbe[seconds-40];
                    delay();P0 = 0xC0 | (P0 & 0x3F);
                }
                else if ((seconds >= 50) && (seconds < 60))
                {
                    P0 = 0x40 | (P0 & 0x3F);
                    P1 = LCD_talbe[5];
                    delay();P0 = 0xC0 | (P0 & 0x3F);
                    P0 = 0x80 | (P0 & 0x3F);
                    P1 = LCD_talbe[seconds-50];
                    delay();P0 = 0xC0 | (P0 & 0x3F);
                }
            }
            else
            {
                P0 = P0 & 0x3F;
                P1 = 0x00;
            }
        }
    }
}

/*-- Local functions ---------------------------------------------------------*/

// char *gets(char *p,int s)
// {
//   char *ret = p;
//   int c;

//   while(--s)
//   {
//     c=getchar();

//     if(c=='\n'||c=='\r') break;
//     if(c=='\b') // backspace!
//     {
//         if(p!=ret) 
//         {
//             *--p = 0;
//             s++;
//         }
//     }
//     else
//         *p++ = c;
//   }
//   *p=0;

//   return p==ret ? NULL : ret;
// }

int xtoi(char *s1)
{
    int ret;

    for(ret=0;s1&&*s1;s1++) 
    {
        if(*s1<='9')
            ret = *s1-'0'+(ret<<4); // val = val*16+int(*s1)
        else
            ret = 10+(*s1&0x5f)-'A'+(ret<<4); // val = val*16+int(toupper(*s1))
    }

    return ret;
}
int atoi(char *s1)
{
    int ret,sig;

    for(sig=ret=0;s1&&*s1;s1++) 
    {
        if(*s1=='-') 
            sig=1;
        else 
            ret = *s1-'0'+(ret<<3)+(ret<<1); // val = val*10+int(*s1)
    }

    return sig ? -ret : ret;
}

// char *strtok(char *str,char *dptr)
// {
//     static char *nxt = NULL;

//     int dlen = strlen(dptr);
//     char *tmp;

//          if(str) tmp=str;
//     else if(nxt) tmp=nxt;
//     else return NULL;

//     char *ret=tmp;

//     while(*tmp)
//     {
//         if(strncmp(tmp,dptr,dlen)==0)
//         {
//             *tmp=NULL;
//             nxt = tmp+1;
//             return ret;
//         }
//         tmp++;
//     }
//     nxt = NULL;
//     return ret;
// }
void delay()
{
    uint16_t i;
    for (i = 0; i < 5000; ++i);
}
