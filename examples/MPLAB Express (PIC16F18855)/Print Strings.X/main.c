/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F18855
        Driver Version    :  2.00
 */

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
 */

#include "mcc_generated_files/mcc.h"
#include "mates/controller.h"

unsigned long timerMilliseconds = 0;

void TimerInterruptHandler(void) {
    timerMilliseconds++;
}

unsigned long millis() {
    return timerMilliseconds;
}

void delay(uint16_t timeout) {
    unsigned long startTime = timerMilliseconds;
    while ((timerMilliseconds - startTime) < timeout);
}

void matesReset(void) {
    RST_SetLow();
    delay(100); // doesn't feel like 5seconds
    RST_SetHigh();
}

uint8_t matesSerialAvailable(void) {
    return eusartRxCount;
}

/*
                         Main application
 */
void main(void) {
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    TMR0_SetInterruptHandler(TimerInterruptHandler);
    TMR0_StartTimer();

    mates_attachHwResetFnc(matesReset);
    mates_attachMillisFnc(millis);
    mates_attachWriteFnc(EUSART_Write);
    mates_attachReadFnc(EUSART_Read);
    mates_attachRxCountFnc(matesSerialAvailable);

    if (!mates_begin()) {
        // Display didn't send ready signal in time
        while (1) {
            ERR_Toggle();
            delay(100);
        }
    }

    const char * msg =    "Mates Studio offers a variety of widgetswhich includes this Print Area. For moreinfo, please refer  to our manuals.";
    
    uint8_t len  = (uint8_t) strlen(msg);

    char str[2];
    
    mates_appendStringToPrintArea(0, msg);

    unsigned long lastUpdate = millis();    
    
    while (1) {
                
        if (millis() - lastUpdate >= 3000) {
            
            mates_clearPrintArea(0);
            
            for (uint8_t i = 0; i < len; i++) {
                str[0] = msg[i];
                str[1] = 0;
                mates_appendStringToPrintArea(0, str);
                delay(50);
            }

            lastUpdate = millis();
        }

    }

}
/**
 End of File
 */