#include <16F877.h>
#fuses brownout,hs,nowdt,nolvp

#define LED pin_b0
#define SW pin_d0

void main()
{
 output_b(0b00000000);
 
 while(1)
 {
 if(input(SW)==0)
 {
 output_high(LED);
 }
 else
 {
 output_low(LED);
 }
 }
}