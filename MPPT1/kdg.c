#if defined(__PCM__)
#include <16F877.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)
#endif

#include <string.h>
#include <stdlib.h>

float adc_value;
float Vsolar;
float Isolar;
float Psolar;
float PrevVsolar=0;
float PrevIsolar=0;
float PrevPsolar=0;
int pwm;
int pwm_start=10;
int pwm_min=6;
int pwm_max=14;
int delta=1;

void main()
{

pwm=pwm_start;

SETUP_ADC_PORTS(ALL_ANALOG);
setup_adc(ADC_CLOCK_DIV_8);

output_low(PIN_C1);           /* set CCP1 output low */

setup_ccp1(CCP_PWM);          /* setup pin CCP1 (RC2) to do PWM */

setup_timer_2(T2_DIV_BY_1,19,1);  /* 50 KHZ */

delay_ms(1000);         
set_adc_channel(0);     /* select RA */
delay_ms(20);            /* wait to read ADC */
Vsolar =read_adc();   /* Read the voltage input from ADC channel 0 */

set_adc_channel(1);           /* select RA1 */
delay_ms(20);                 /* wait to read ADC */
Isolar =read_adc();

float delta_P=Psolar-PrevPsolar; 
float delta_V=Vsolar-PrevVsolar; 
float delta_I=Isolar-PrevIsolar;
float gradient=delta_P/delta_V; 
int pwm;

if(delta_V!=0)
{ 
if(gradient>0)
{ 
pwm+=delta; 
PrevPsolar=Psolar;
PrevVsolar=Vsolar; 
PrevIsolar=Isolar; 
if (pwm<pwm_min)
{
pwm=pwm_min;
set_pwm1_duty(pwm);
}
else if (pwm>pwm_max || pwm==pwm_max)
{
pwm=pwm_max;
set_pwm1_duty(pwm);
}
else if (pwm<pwm_max && pwm>pwm_min)
{
pwm=pwm;
set_pwm1_duty(pwm);
}
}
else if(gradient<0)
{ 
pwm-=delta; 
PrevPsolar=Psolar; 
PrevVsolar=Vsolar; 
PrevIsolar=Isolar; 
if (pwm<pwm_min)
{
pwm=pwm_min;
set_pwm1_duty(pwm);
}
else if (pwm>pwm_max || pwm==pwm_max)
{
pwm=pwm_max;
set_pwm1_duty(pwm);
}
else if (pwm<pwm_max && pwm>pwm_min)
{
pwm=pwm;
set_pwm1_duty(pwm);
}
} 

else 
{
if(delta_I>0)
{ 
pwm+=delta; 
PrevPsolar=Psolar; 
PrevVsolar=Vsolar; 
PrevIsolar=Isolar;
if (pwm<pwm_min)
{
pwm=pwm_min;
set_pwm1_duty(pwm);
}
else if (pwm>pwm_max || pwm==pwm_max)
{
pwm=pwm_max;
set_pwm1_duty(pwm);
}
else if (pwm<pwm_max && pwm>pwm_min)
{
pwm=pwm;
set_pwm1_duty(pwm);
}
}

else if(delta_I<0)
{ 
pwm-=delta; 
PrevPsolar=Psolar; 
PrevVsolar=Vsolar; 
PrevIsolar=Isolar;
if (pwm<pwm_min)
{
pwm=pwm_min;
set_pwm1_duty(pwm);
}
else if (pwm>pwm_max || pwm==pwm_max)
{
pwm=pwm_max;
set_pwm1_duty(pwm);
}
else if (pwm<pwm_max && pwm>pwm_min)
{
pwm=pwm;
set_pwm1_duty(pwm);
}
}
}
}
}

