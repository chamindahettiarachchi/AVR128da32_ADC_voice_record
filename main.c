/*
 * File:   main.c
 * Author: chami
 *
 * Created on May 7, 2026, 8:37 AM
 */

#define F_CPU 4000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
//#include "voice/welcome_to_the_show_8k_u8.h"
#include "welcome_to_the_show_8k_u8.h"

#define DAC_VALUE 512   // Half of 10-bit range: 0 to 1023
#define SAMPLE_COUNT WELCOME_TO_THE_SHOW_SAMPLE_COUNT


volatile uint16_t sample_index = 0;
volatile uint8_t playing = 1;
volatile uint16_t wave_cycles = 0;
static void TCB0_Init_8kHz(void)

{
    // F_CPU = 4 MHz
    // TCB clock = F_CPU / 2 = 2 MHz
    // 2 MHz / 8000 Hz = 250 counts
    // So CCMP = 249

    TCB0.CTRLA = 0;                  // stop timer first
    TCB0.CTRLB = TCB_CNTMODE_INT_gc; // periodic interrupt mode
    TCB0.CCMP = 249;                 // 8000 interrupts per second
    TCB0.INTCTRL = TCB_CAPT_bm;      // enable interrupt
    TCB0.CTRLA = TCB_CLKSEL_DIV2_gc | TCB_ENABLE_bm;
}

static void DAC0_Write(uint16_t value)
{
    value &= 0x03FF;   // keep only 10 bits

    // DAC data is split:
    // lower 2 bits go to DATAL bits 7:6
    // upper 8 bits go to DATAH
    DAC0.DATAL = (uint8_t)((value & 0x03) << 6);
    DAC0.DATAH = (uint8_t)(value >> 2);
    
}



static void Audio_Play(void)
{
    uint8_t oldSREG = SREG;  // save interrupt state
    cli();                   // temporarily stop interrupts

    sample_index = 0;
    wave_cycles = 0;
    playing = 1;

    SREG = oldSREG;          // restore interrupt state
}

int main(void)
{
    // Select 2.048V internal reference for DAC and keep it always on
    VREF.DAC0REF = VREF_REFSEL_2V048_gc | VREF_ALWAYSON_bm;
    _delay_us(50);

    // Disable digital input buffer on PD6, the DAC output pin
    PORTD.PIN6CTRL &= ~PORT_ISC_gm;
    PORTD.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;

    // Disable pull-up on PD6
    PORTD.PIN6CTRL &= ~PORT_PULLUPEN_bm;

    // Enable DAC and enable output buffer to pin
    DAC0.CTRLA = DAC_ENABLE_bm | DAC_OUTEN_bm;

    // Output fixed voltage
    DAC0_Write(DAC_VALUE);
    
    TCB0_Init_8kHz();
    sei();

while (1)
{
    if (!playing)
    {
        _delay_ms(2000);
        Audio_Play();
    }
}
}

ISR(TCB0_INT_vect)
{
    TCB0.INTFLAGS = TCB_CAPT_bm;     // clear interrupt flag

    if (playing)
    {
        uint8_t sample = pgm_read_byte(&welcome_to_the_show_data[sample_index]);

        DAC0_Write((uint16_t)sample << 2);

        sample_index++;

        if (sample_index >= SAMPLE_COUNT)
        {
            sample_index = 0;
            playing = 0;
            DAC0_Write(512);          // middle voltage = silence
        }
    }
    else
    {
        DAC0_Write(512);              // silence
    }
}