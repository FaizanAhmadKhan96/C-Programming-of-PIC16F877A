#include <16F877.h>
#device ADC = 10
#fuses HS,BROWNOUT,NOWDT,NOLVP
#use delay(clock=20M )

unsigned int16 i, j ;
void main(){
  setup_adc(ADC_CLOCK_DIV_8);            // Set ADC conversion time to 8Tosc
  setup_adc_ports(ALL_ANALOG);// Configure AN0 as analog input 
  setup_ccp1(CCP_PWM);// Configure CCP1 as standard PWM
  delay_ms(1);
  setup_ccp2(CCP_PWM);                   // Configure CCP2 as standard PWM
  setup_timer_2(T2_DIV_BY_16, 250, 1);   // Set PWM frequency to 500Hz
  delay_ms(100);                         // Wait 100ms
  while(TRUE){
    set_adc_channel(0);                  // Select channel AN0
    delay_ms(1);                         // Wait 1ms
    i = read_adc();                      // Read from AN0 and store in i
    delay_ms(1);                         // Wait 1ms
    set_adc_channel(1);                  // Select channel AN1
    delay_ms(1);                         // Wait 1ms
    j = read_adc();                      // Read from AN1 and store in j
    set_pwm1_duty(i/2);// Set pwm1 duty cycle to i
    delay_ms(1);
    set_pwm2_duty(j/2);                    // Set pwm2 duty cycle to j
    delay_ms(1);                         // Wait 1ms
   }
}