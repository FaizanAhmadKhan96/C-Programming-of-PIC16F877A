#include<16F877A.h>
#fuses HS,NOWDT,NOLVP,BROWNOUT

#define LED1 PIN_B0
#define LED2 PIN_B1
#define SW1 PIN_D0
#define SW2 PIN_D1

void main()
{
output_b(0b00000000);
while(1)
{

if(input(SW1)==0)
{
output_high(LED1);
}
else
{
output_low(LED1);
}

if(input(SW2)==0)
{
output_high(LED2);
}
else
{
output_low(LED2);
}
}
}
