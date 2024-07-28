#if defined(__PCM__)
#include <16F877A.h>
#device adc=10
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)
#endif

#include <string.h>
#include <stdlib.h>

#define lcd_rs pin_d0
#define lcd_en pin_d1

#define disp_clear 0b00000001
#define disp_control 0b00001110
#define func_set 0b00111000
#define lscrol 0b00011000
#define rscrol 0b00011100
#define line1 0b10000000
#define line2 0b11000000

float adc_value,adc_value1;
float Vsolar,Vsolar1;
float Isolar,Isolar1;
float Psolar,Psolar1;
float Vbatt;
float Ibatt;
float Pbatt;
float PrevVsolar=0;
float PrevIsolar=0;
float PrevPsolar=0;
int pwm;
int pwm_start=10;
int pwm_min=6;
int pwm_max=14;
int delta=1;
char charger_mode;                  

void lcd_init (void);
void lcd_data (unsigned char);
void lcd_cmd (unsigned char);
void lcd_lscrol (unsigned char);
void lcd_rscrol (unsigned char);
void lcd_line1 (unsigned char);
void lcd_line2 (unsigned char);

void lcd_init (void)
{
delay_ms(300);
lcd_cmd (func_set);
lcd_cmd (disp_control);
lcd_cmd(disp_clear);
 delay_ms(50);
}

void lcd_cmd (unsigned char i)
{
output_low(lcd_rs);
output_b(i);
output_high(lcd_en);
delay_us(100);
output_low(lcd_en);
}

void lcd_data (unsigned char i)
{
output_high(lcd_rs);
output_b(i);
output_high(lcd_en);
delay_us(100);
output_low(lcd_en);
}

void lcd_lscrol (unsigned char i)
{
unsigned int8 count;
for(count=0;count<i;count++)
{
lcd_cmd(lscrol);
delay_ms(300);
}
}

void lcd_rscrol (unsigned char i)
{
unsigned int8 count;
for(count=0;count<i;count++)
{
lcd_cmd(rscrol);
delay_ms(300);
}
}

void lcd_line1 (unsigned char position)
{
lcd_cmd(line1 + position);
}

void lcd_line2 (unsigned char position)
{
lcd_cmd(line2 + position);
}

void main()
{

pwm=pwm_start;

SETUP_ADC_PORTS(ALL_ANALOG);
setup_adc(ADC_CLOCK_DIV_8);

output_low(PIN_C1);           

setup_ccp1(CCP_PWM);         

setup_timer_2(T2_DIV_BY_1,19,1);

int i=0;

do
{
lcd_line1 (3);
lcd_data ("Made by:");
lcd_line2 (0);
lcd_data ("Faizan,Shakir");
delay_ms(1000);
lcd_rscrol(20);

lcd_cmd(disp_clear);
 delay_ms(50);

lcd_line1 (3);
lcd_data ("Made by:");
lcd_line2 (2);
lcd_data ("And Hamza");
delay_ms(1000);
lcd_rscrol(20);
i++;
}
while(i==0);

lcd_cmd(disp_clear);
 delay_ms(50);
 
while(1)
{
set_adc_channel(0);
delay_us(100);

adc_value = read_adc();

Vsolar = (5.0*adc_value/1023.0)/0.111;

delay_us(200);

adc_value1 = read_adc();

Vsolar1 = (5.0*adc_value/1023.0)/0.111;

 set_adc_channel(1);
delay_us(100);

adc_value = read_adc();

Isolar = ((5.0/1023.0*adc_value)-2.5)/0.066;

delay_us(200);

adc_value1 = read_adc();

Isolar1 = ((5.0/1023.0*adc_value)-2.5)/0.066;

set_adc_channel(2);
delay_us(100);

adc_value = read_adc();

Vbatt = (5.0*adc_value/1023.0)/0.333;

set_adc_channel(3);
delay_us(100);

adc_value = read_adc();

Ibatt = ((5.0/1023.0*adc_value)-2.5)/0.066;

lcd_line1(0);

printf(lcd_data,"VOLTAGE:%f V",Vsolar);
delay_ms(1000);
lcd_rscrol(20);
delay_ms(1000);

lcd_cmd(disp_clear);
 delay_ms(50);
 
lcd_line1(0);

 printf(lcd_data,"CURRENT:%f A",Isolar);
delay_ms(1000);
lcd_rscrol(20);

lcd_cmd(disp_clear);
 delay_ms(50);
 
 Psolar=Vsolar*Isolar;
 Psolar1=Vsolar1*Isolar1;
 lcd_line1(0);
 lcd_data("INPUT POWER:");
 
 lcd_line2(3);
 printf(lcd_data,"%f W",Psolar);
 
delay_ms(1000);
lcd_rscrol(20);

lcd_cmd(disp_clear);
 delay_ms(50);

lcd_line1(0);

printf(lcd_data,"VOLTAGE: %f V",Vbatt);
delay_ms(1000);
lcd_rscrol(20);

lcd_cmd(disp_clear);
 delay_ms(50);

lcd_line1(0);

printf(lcd_data,"CURRENT: %f A",Ibatt);
delay_ms(1000);
lcd_rscrol(20);

lcd_cmd(disp_clear);
 delay_ms(50);
 
  Pbatt=Vbatt*Ibatt;
 lcd_line1(0);
 lcd_data("OUTPUT POWER:");
 
 lcd_line2(3);
 printf(lcd_data,"%f W",Pbatt);
 
delay_ms(1000);
lcd_rscrol(20);

lcd_cmd(disp_clear);
 delay_ms(50);

 switch(charger_mode)
 {     
 case 'off':     
 MOSFET_OFF;    
 delay(1500);    
 if(((Vbatt>12)&&(Vsolar>Vbatt+0.5)||(Vbatt<=12)&&(Vsolar>=12.5)))
 {       
 charger_mode=initialize;    
 }    
 break;
 
    case initialize:     
 if(((Vbatt>12)&&(Vsolar<Vbatt+0.5)||(Vbatt<=12)&&(Vsolar<12.5)||(Isolar<0.1)||
 (Ibatt>Imax) ||(Vbatt>Vboost+0.15)||Psolar<Pleast||Tbatt>Tmax))
 {    
 charger_mode=off;     
 MOSFET_OFF;    
 break;     
 }     
 else 
 {
 if(boost_time>boost_settime)
 {      
 charger_mode=float_mode;
 }      
 else
 {
 if(Vbatt>Vboost)
 {        
 charger_mode=absorp;       
 MOSFET_ON;        
 boost_millis=millis();       
 break;      
 }      
 else 
 {        
 charger_mode=bulk; 
 MOSFET_ON;        
 break;       
 }     
 }    
 }   
 break;       
 
 case 'bulk':       
 if(((Vbatt>12)&&(Vsolar<Vbatt+0.5)||(Vbatt<=12)&&(Vsolar<12.5)||(Isolar<0.1)||
 (Ibatt>Imax) ||(Vbatt>Vboost+0.15)||Psolar<Pleast||Tbatt>Tmax))
 {        
 charger_mode=off;        
 MOSFET_OFF;         
 break;      
 }      
 else 
 {
 if (Vbatt>Vboost+0.01)
 {        
 if (boost_time>boost_settime)
 {           
 charger_mode=float_mode;          
 MOSFET_ON;           
 break;        
 }
else 
{ 
charger_mode=boost;  
MOSFET_ON;        
boost_time=millis();   
break;       
}     
}     
else 
{ 
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
PrevPsolar=Psolar1;
PrevVsolar=Vsolar1; 
PrevIsolar=Isolar1; 
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
PrevPsolar=Psolar1; 
PrevVsolar=Vsolar1; 
PrevIsolar=Isolar1; 
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
PrevPsolar=Psolar1; 
PrevVsolar=Vsolar1; 
PrevIsolar=Isolar1;
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
PrevPsolar=Psolar1; 
PrevVsolar=Vsolar1; 
PrevIsolar=Isolar1;
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
if(Ibatt>Imax-1)
{         
error3=Ibatt-(Imax-1);         
inc=300*delta*error3;         
pwm-=inc;        
set_pwmdutycycle(pwm);     
}     
break; 
} 
}            

case absorp:        
boost_time=boost_time+millis()-boost_millis;   
boost_millis=millis();         
if (((Vbatt>12)&&(Vsolar<Vbatt+0.5)||(Vbatt<=12)&&(Vsolar<12.5)||(Ibatt>Imax)||
(Isolar<0.1) ||(Vbatt>Vboost+0.15)||Psolar<Pleast))
{         
charger_mode=off;         
MOSFET_OFF;         
break;      
}   
else 
{
if (Vbatt< Vboost-0.25)
{    
charger_mode=bulk;       
MOSFET_ON;     
break;  
}     
else
{  
if(boost_time>boost_settime)
{       
charger_mode=float_mode;
MOSFET_ON;
break;
}       
else
{
if(Vbatt>Vboost+0.02)
{         
error1=Vbatt-(Vboost+0.02);         
inc=delta*500*error1;        
pwm-=inc;         
set_pwmdutycycle(pwm);           
boost_time=boost_time+millis()-boost_millis;      
}            
if(Ibatt>Imax-1)
{         
error3=Ibatt-(Imax-1);         
inc=300*delta*error3;       
pwm-=inc;         
set_pwmdutycycle(pwm);          
}     
break;   
}  
}  
}

case float_mode:     
if (((Vbatt>12)&&(Vsolar<Vbatt+0.5)||(Vbatt<=12)&&(Vsolar<12.5)||(Isolar<0.1)||(Ibatt>Imax) ||(Vbatt>Vboost+0.15)||Psolar<Pleast||Tbatt>Tmax))
{         
charger_mode=off;        
MOSFET_OFF;         
break;      
}    
else 
{
if(Vbatt<(Vfloat-0.1))
{     
charger_mode=bulk;    
MOSFET_ON;    
break;    
}   
else
{
if (Vbatt>(Vfloat+0.02))
{     
error2=Vbatt-(Vfloat+0.02);
inc=delta*500*error2;   
pwm-=inc;   
set_pwmdutycycle(pwm);      
if(Ibatt>Imax-1)
{         
error3=Ibatt-(Imax-1);        
inc=300*delta*error3;
pwm-=inc;
set_pwmdutycycle(pwm);       
}      
break;   
}  
}

default:  
charger_mode=off;   
MOSFET_OFF;  
break;  
}
}
}
}
}

