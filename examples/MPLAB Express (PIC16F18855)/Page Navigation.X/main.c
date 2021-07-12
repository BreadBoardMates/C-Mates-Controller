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

void Day_and_Time_Animation(void) {

    // Days of Week Strings
    const char * daysOfWeek[] = {
        "SUNDAY",
        "MONDAY",
        "TUESDAY",
        "WEDNESDAY",
        "THURSDAY",
        "FRIDAY",
        "SATURDAY"
    };

    // Test Start Values: Monday, 23:59:47 (will actually start at 48 seconds)
    uint8_t lastDay = 1;
    uint8_t day = 1;
    int16_t hrs = 23;
    int16_t min = 59;
    int16_t sec = 47;

    mates_updateTextArea(0, daysOfWeek[day]);
    
    // Simulated Timer Variable
    unsigned long lastUpdate = millis() - 1000; // Ensures first write

    // stop at Day 2, Tuesday, 00:00:0  7
    while (day != 2 || hrs != 0 || min != 0 || sec != 7) {

        if (millis() - lastUpdate >= 1000) {

            lastUpdate = millis();

            sec++;
            if (sec == 60) {
                sec = 0;
                min++;
            }
            if (min == 60) {
                min = 0;
                hrs++;
            }
            if (hrs == 24) {
                hrs = 0;
                day++;
            }
            day %= 7;

            mates_setWidgetValue(MATES_LED_DIGITS, 0, hrs);
            mates_setWidgetValue(MATES_LED_DIGITS, 1, min);
            mates_setWidgetValue(MATES_LED_DIGITS, 2, sec);
            //            mates_setLedDigitsShortValue(0, hrs);
            //            mates_setLedDigitsShortValue(1, min);
            //            mates_setLedDigitsShortValue(2, sec);
            if (lastDay != day) {
                mates_updateTextArea(0, daysOfWeek[day]);
                lastDay = day; // prevents writing the same text to TextArea
            }

        }

    }
}

void Numbered_LEDs_Animation(void) {

    mates_setWidgetValue(MATES_MEDIA_LED, 0, 1);

    uint8_t ledOff = 0;
    uint8_t ledOn = 1;

    for (int i = 0; i < 20; i++) {

        delay(500);

        mates_setWidgetValue(MATES_MEDIA_LED, ledOff, 0);
        mates_setWidgetValue(MATES_MEDIA_LED, ledOn, 1);
        
        ledOff = ledOn;
        ledOn++;
        ledOn %= 8;

    }
    
    mates_setWidgetValue(MATES_MEDIA_LED, ledOff, 0);

}

void Various_Gauges_Animation(void) {
    int16_t value = 0;
    int8_t inc = 1;

    unsigned long lastUpdate = millis();

    while (millis() - lastUpdate <= 10000) {

        value += inc;
        if (value == 100) inc = -1;
        if (value == 0) inc = 1;

        mates_setWidgetValue(MATES_GAUGE_A, 0, value);
        mates_setWidgetValue(MATES_LED_DIGITS, 3, value);
        mates_setWidgetValue(MATES_MEDIA_GAUGE_B, 0, value);

    }
}

void Various_Digits_Animation(void) {
    int16_t value = 0;
    int32_t longValue = 100000;
    float floatValue = 3.1416;

    unsigned long lastUpdate = millis();

    while (millis() - lastUpdate <= 5000) {

        //        mates_setWidgetValue(MATES_LED_DIGITS, 4, value);
        mates_setLedDigitsShortValue(4, value);
        mates_setLedDigitsLongValue(5, longValue);
        mates_setLedDigitsFloatValue(6, floatValue);

        value++;
        longValue += 12345;
        floatValue += 3.1416;

    }
}

void Print_Strings_Animation(void) {

    const char * msg = "Mates Studio offers a variety of widgetswhich includes this Print Area. For moreinfo, please refer  to our manuals.";

    uint8_t len = (uint8_t) strlen(msg);

    char str[2];

    mates_appendStringToPrintArea(0, msg);

    delay(2000);

    mates_clearPrintArea(0);

    for (uint8_t i = 0; i < len; i++) {
        str[0] = msg[i];
        str[1] = 0;
        mates_appendStringToPrintArea(0, str);
        delay(50);
    }

}

void Print_Hex_Values_Animation(void) {
    const int16_t colors[] = {
        (int16_t) 0xFFFF, (int16_t) 0xF800, (int16_t) 0x07E0, (int16_t) 0x001F,
        (int16_t) 0x07FF, (int16_t) 0xF81F, (int16_t) 0xFFE0, (int16_t) 0x39FF
    };

    int8_t val = 0;
    int8_t ctr = 0;

    unsigned long lastUpdate = millis();
    
    while (millis() - lastUpdate <= 5000) {

        if (ctr >= 42) {
            mates_clearPrintArea(0);
            ctr = 0;
        }

        mates_setPrintAreaColor565(1, colors[ctr % 8]);
        mates_appendArrayToPrintArea(1, &val, 1);
        ctr++;
        val++;

        delay(100);

    }
}

void Audio_Spectrum_Animation(void) {
    uint8_t col = 1;

    unsigned long lastUpdate = millis();
    
    while (millis() - lastUpdate <= 5000) {

        mates_setSpectrumValue(MATES_MEDIA_SPECTRUM, 0, col, (uint8_t) (rand() % 101));
//        mates_setMediaSpectrumValue(0, col, (uint8_t) (rand() % 101));

        col++;
        if (col == 6) col = 0;
        
    }
}

void Updating_Scope_Animation(void) {
    uint16_t deg = 270;
    int16_t value = 18;
    
    const int16_t values[] = {
        40, 50, 59, 67, 73, 77, 79, 77, 73, 67, 59, 50,
        40, 29, 20, 12, 6, 2, 1, 2, 6, 12, 20, 29
    };
    
    unsigned long lastUpdate = millis();

    while (millis() - lastUpdate <= 5000) {
        
        // Computed version START
        /*
        deg += 15;
        if (deg >= 360) {
            deg %= 360;
        }
        
        // 180 / PI = 57.29578
        // value = (int16_t) (39 * sin(deg * 3.1416 / 180)) + 40;
        value = (int16_t) (39 * sin(deg / 57.29578)) + 40;
        // Float computations can take a significant time

        mates_setWidgetValue(MATES_SCOPE, 0, value);
        */
        // Computed version END

        // Pre-computed version START
        
        value++;
        if (value >= 24) value = 0;
        mates_setWidgetValue(MATES_SCOPE, 0, values[value]);
        
        // Pre-computed version END

    }
}

void Update_Dot_Matrix(void) {
    char buffer[25]; // 24 characters + null terminator (8 * 3 + 1 = 25)
    strcpy(buffer, "DotMatrxVal1 XXXVal2 XXX");
    char * val1ptr = buffer + 13;
    char * val2ptr = buffer + 21;

    uint8_t val1 = 0;
    uint8_t val2 = 255;

    unsigned long lastUpdate = millis();

    while (millis() - lastUpdate <= 5000) {

        // START of Simulated Values
        val1++;
        val2--;

        if (val1 < 100) {
            val1ptr[0] = ' ';
        } else {
            val1ptr[0] = '0' + (val1 / 100);
        }

        if (val1 < 10) {
            val1ptr[1] = ' ';
        } else {
            val1ptr[1] = '0' + ((val1 % 100) / 10);
        }

        val1ptr[2] = '0' + (val1 % 10);

        if (val2 < 100) {
            val2ptr[0] = ' ';
        } else {
            val2ptr[0] = '0' + (val2 / 100);
        }

        if (val2 < 10) {
            val2ptr[1] = ' ';
        } else {
            val2ptr[1] = '0' + ((val2 % 100) / 10);
        }

        val2ptr[2] = '0' + (val2 % 10);
        // END of Simulated Values

        mates_updateDotMatrix(0, buffer);

    }
}

void (* animations[])(void) = {
    Day_and_Time_Animation,
    Numbered_LEDs_Animation,
    Various_Gauges_Animation,
    Various_Digits_Animation,
    Print_Strings_Animation,
    Print_Hex_Values_Animation,
    Audio_Spectrum_Animation,
    Updating_Scope_Animation,
    Update_Dot_Matrix
};

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

    uint16_t page = 0;
    while (1) {
        (*animations[page])();
        page++;
        page %= 9;
        mates_setPage(page);
    }

}
/**
 End of File
 */