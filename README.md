![image](logo.png)

<br/>

# **Mates Controller C Library**

This is a C Library developed to be used with Mates Studio's Commander and Architect Environments. This library is aimed to be simple enough to learn for beginners and feature-rich for experienced developers.

<br/>

## **Library Discussion**
----------------------------------------------------------------------------

This section serves to give brief discussion about the setup requirements and usable functions included in the library. For functional examples on how to use these functions in a project, refer to the examples folder.

<br/>

### **mates_attachHwResetFnc(resetFnc)**

This should be used during setup to attach a function that will reset the display through the hardware reset pin.

The reset function should be in the format: _void resetFnc(void)_

<br/>

| Parameters | Type                    | Description                                                      |
|:----------:|:-----------------------:| ---------------------------------------------------------------- |
| resetFnc   | void (* resetFnc)(void) | A function triggering a short digital pulse to reset the display |

<br/>

#### Example:
    mates_attachHwResetFnc(resetFnc); // Attaches resetFnc to the library so it could be used in mates_begin() function

<br/>

### **mates_attachMillisFnc(resetFnc)**

This should be used during setup to attach a function that returns the system time in milliseconds.

The millis function should be in the format: _uint32\_t millisFnc(void)_

<br/>

| Parameters | Type                         | Description                                             |
|:----------:|:----------------------------:| ------------------------------------------------------- |
| millisFnc  | uint32_t (* millisFnc)(void) | A function that returns the system time in milliseconds |

<br/>

#### Example:
    mates_attachMillisFnc(millisFnc); // Attaches millisFnc to the library so it could be used to wait for timeouts

<br/>

### **mates_begin()**

This function must be used once to check required functions, reset the display and wait until the display is ready.

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    // Checks the required functions, resets the display and wait until the display is ready
    if (!mates_begin()) {
        // if this returns false, it could be either:
        //   - a required function hasn't been attached to the library
        //   - the display didn't respond with a ready signal (0x06) on time
        while (1);
    }
    // otherwise, the begin function will exit when the display is ready


<br/>

**Return:** success or failure (_boolean_)

<br/>

**Note 1:** _Ensure that all required hardware peripherals and software setup (Reset I/O, Serial UART and Timers) are started before using this function_

**Note 2:** _Ensure that the Serial UART of the host is set to the same baudrate that matches the setting of the Mates Studio Commander/Architect project before this function is used_

<br/>

### **mates_reset()**

This function can be used to reset the display by sending a reset pulse from the reset pin by utilizing the reset function attached using mates_attachHwResetFnc.

The function finishes as soon as the display sends the ready signal or the wait period passes. The default wait period is 5 seconds.

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example:
    // Resets the display and wait until the display is ready
    if (!mates_reset()) {
        // if this returns false:
        //  - the display didn't respond with a ready signal (0x06) on time
        while (1);
    }
    // otherwise, the reset function will exit when the display is ready

<br/>

### **mates_softReset()**

This function can be used to reset the display by sending a Serial UART reset command.

The function finishes as soon as the display sends the ready signal or the wait period passes. The default wait period is 5 seconds.

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example:
    // Resets the display and wait until the display is ready
    if (!mates_softReset()) {
        // if this returns false:
        //  - the display didn't respond with a ready signal (0x06) on time
        while (1);
    }
    // otherwise, the reset function will exit when the display is ready

<br/>

### **mates_setBacklight(value)**

This function can be used to set the backlight level to the _value_ specified.

<br/>

| Parameters | Type    | Description                |
|:----------:|:-------:| -------------------------- |
| value      | uint8_t | The target backlight level |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    mates_setPage(1); // Navigate to Page1

<br/>

### **mates_setPage(page)**

This function can be used to navigate to the specified _page_.

<br/>

| Parameters | Type     | Description           |
|:----------:|:--------:| --------------------- |
| page       | uint16_t | The target page index |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    mates_setPage(1); // Navigate to Page1

<br/>

### **mates_getPage()**

This function can be used to query the current active page.

<br/>

**Return:** Active page index (_uint16_t_)

<br/>

#### Example: 
    uint16_t activePage = mates_getPage(); // Query active page

<br/>

### **mates_setWidgetValue(type, index, value)**

This function can be used to set the 16-bit integer _value_ of the specified _widget_

<br/>

| Parameters | Type        | Description                    |
|:----------:|:-----------:| ------------------------------ |
| type       | MatesWidget | The type of the target widget  |
| index      | int8_t      | The index of the target widget |
| value      | int16_t     | The new value for the widget   |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    mates_setWidgetValue(MATES_MEDIA_GAUGE_B, 0, 50); // Set value of MediaGaugeB0 to 50

**Note:** _All applicable widget types are listed in [here](mates/includes/widgets.md)._

<br/>

### **mates_getWidgetValue(type, index)**

This function can be used to query the value of the widget specified by _type_ and _index_.

<br/>

| Parameters | Type        | Description                    |
|:----------:|:-----------:| ------------------------------ |
| type       | MatesWidget | The type of the target widget  |
| index      | int8_t      | The index of the target widget |

<br/>

**Return:** Value of the specified widget (_int16_t_)

<br/>

#### Example: 
    int16_t widgetVal = mates_getWidgetValue(MATES_MEDIA_LED, 4); // Query the current value of MediaLed4

**Note:** _This function is not applicable to **Int32** and **Float** LedDigits_

<br/>

### **mates_setLedDigitsValue(index, value)**

This function can be used to set the 16-bit integer _value_ of the LedDigits specified by _index_

<br/>

| Parameters | Type        | Description                       |
|:----------:|:-----------:| --------------------------------- |
| index      | int8_t      | The index of the target LedDigits |
| value      | int16_t     | The new value for the LedDigits   |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    mates_setLedDigitsValue(2, 50); // Set value of LedDigits2 to 50

**Note:** _This function is only applicable for **Int16** LedDigits_

<br/>

### **mates_setLedDigitsValue(index, value)**

This function can be used to set the 32-bit integer _value_ of the LedDigits specified by _index_

<br/>

| Parameters | Type        | Description                       |
|:----------:|:-----------:| --------------------------------- |
| index      | int8_t      | The index of the target LedDigits |
| value      | int32_t     | The new value for the LedDigits   |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    mates_setLedDigitsValue(0, 602214076); // Set value of LedDigits0 to 602214076

**Note:** _This function is only applicable for **Int32** LedDigits_


<br/>

### **mates_setLedDigitsValue(index, value)**

This function can be used to set the floating point _value_ of the LedDigits specified by _index_

<br/>

| Parameters | Type        | Description                       |
|:----------:|:-----------:| --------------------------------- |
| index      | int8_t      | The index of the target LedDigits |
| value      | int32_t     | The new value for the LedDigits   |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    mates_setLedDigitsValue(1, 3.1416); // Set value of LedDigits1 to 3.1416

**Note:** _This function is only applicable for **Float** LedDigits_

<br/>

### **mates_setLedSpectrumValue(index, gaugeIndex, value)**

This function can be used to set the _value_ of a specified _gaugeIndex_ of the Led Spectrum widget determined by _index_.

<br/>

| Parameters | Type        | Description                                       |
|:----------:|:-----------:| ------------------------------------------------- |
| index      | int8_t      | The index of the target Led Spectrum widget       |
| gaugeIndex | int8_t      | The gauge index of the target Led Spectrum widget |
| value      | int16_t     | The new value for the column/row of the widget    |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    mates_setLedSpectrumValue(5, 2, 64);
    // Set value of gauge index 2 of LedSpectrum5 to 64

<br/>

### **mates_setMediaSpectrumValue(type, index, gaugeIndex, value)**

This function can be used to set the _value_ of a specified _gaugeIndex_ of the Media Spectrum widget determined by _index_.

<br/>

| Parameters | Type        | Description                                       |
|:----------:|:-----------:| ------------------------------------------------- |
| index      | int8_t      | The index of the target Led Spectrum widget       |
| gaugeIndex | int8_t      | The gauge index of the target Led Spectrum widget |
| value      | int16_t     | The new value for the column/row of the widget    |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    mates_setMediaSpectrumValue(4, 3, 48);
    // Set value of gauge index 3 of MediaSpectrum4 to 48

<br/>

### **mates_setWidgetParam(type, index, param, value);**

This function can be used to set the parameter (_param_) of the target widget, determined by _type_ and _index_, to the specified _value_.

<br/>

| Parameters | Type        | Description                            |
|:----------:|:-----------:| -------------------------------------- |
| type       | MatesWidget | The type of the target widget          |
| index      | int8_t      | The index of the target widget         |
| param      | int16_t     | The target widget parameter            |
| value      | int32_t     | The new value for the widget parameter |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    // Set GaugeA3's Background color to BLACK
    mates_setWidgetParam(MATES_GAUGE_A, 3, MATES_GAUGE_A_BG_COLOR, BLACK); 

<br/>

### **mates_getWidgetParam(type, index, param)**

This function can be used to query the parameter (_param_) of the target widget, determined by _type_ and _index_.

<br/>

| Parameters | Type        | Description                     |
|:----------:|:-----------:| ------------------------------- |
| type       | MatesWidget | The type of the target widget   |
| index      | int8_t      | The index of the target widget  |
| param      | int16_t     | The target widget parameter     |

<br/>

**Return:** The current _param_ value of the widget (int16_t)

<br/>

#### Example: 
    // Query the background color of GaugeA3
    int16_t paramVal = mates_getWidgetParam(MATES_GAUGE_A, 3, MATES_GAUGE_A_BG_COLOR); 

<br/>

### **mates_setBufferSize(size)**

This function can be used to adjust the max string buffer _size_ to be used when composing a string for a TextArea or a PrintArea. The string composition is done by [mates_updateTextArea(index, format, ...)](#) and [mates_appendToPrintArea(index, format, ...)](#)

<br/>

| Parameters | Type  | Description         |
|:----------:|:-----:| ------------------- |
| size       | int   | The new buffer size |

<br/>

**Return:** None

<br/>

#### Example: 
    // Increase buffer size to a maximum of 100 characters
    //     (including the null terminator)
    mates_setBufferSize(100);

<br/>

### **mates_clearTextArea(index)**

This function can be used to clear the TextArea specified by_index_.

<br/>

| Parameters | Type     | Description                             |
|:----------:|:--------:| --------------------------------------- |
| index      | uint16_t | The index of the target TextArea widget |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    mates_clearTextArea(6); // Clear TextArea6

<br/>

### **mates_updateTextArea(index, format, ...)**

This function can be used to update the contents of the TextArea specified by_index_ with the text formed by _format_ and the additional arguments.

<br/>

| Parameters | Type         | Description                                                    |
|:----------:|:------------:| -------------------------------------------------------------- |
| index      | uint16_t     | The index of the target TextArea widget                        |
| format     | const char * | The text to be written to the Text Area                        |
| ...        | -            | Additional values to replace the format specifiers in _format_ |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example No. 1: 
    mates_updateTextArea(2, "Mates"); // Update TextArea2 to "Mates"

#### Example No. 2: 
    int value = 76;
    mates_updateTextArea(3, "Value is %d", 76); // Print value to TextArea3

<br/>

### **mates_clearPrintArea(index)**

This function can be used to clear the PrintArea specified by_index_.

<br/>

| Parameters | Type     | Description                              |
|:----------:|:--------:| ---------------------------------------- |
| index      | uint16_t | The index of the target PrintArea widget |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    mates_clearPrintArea(5); // Clear PrintArea5

<br/>

### **mates_setPrintAreaColor565(index, color)**

This function can be used to set the print color (_rgb565_) used by the PrintArea specified by_index_.

<br/>

| Parameters | Type     | Description                              |
|:----------:|:--------:| ---------------------------------------- |
| index      | uint16_t | The index of the target PrintArea widget |
| rgb565     | int16_t  | The color as a 16-bit RGB565 value       |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    mates_setPrintAreaColor565(4, 0xF800); // Set print color of PrintArea4 to RED (0xF800)

<br/>

### **mates_setPrintAreaColorRGB(index, r, g, b)**

This function can be used to set the print color used by the PrintArea specified by_index_. The color is determined by _r_, _g_ and _b_.

<br/>

| Parameters | Type     | Description                                |
|:----------:|:--------:| ------------------------------------------ |
| index      | uint16_t | The index of the target PrintArea widget   |
| r          | uint8_t  | The red component of the new color value   |
| g          | uint8_t  | The green component of the new color value |
| b          | uint8_t  | The blue component of the new color value  |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    mates_setPrintAreaColor(7, 0, 255, 0); // Set print color of PrintArea7 to GREEN

<br/>

### **mates_appendArrayToPrintArea(index, buffer, len)**

This function can be used to append a number of bytes (_len_) from the data in _buffer_ to the PrintArea specified by_index_ .

<br/>

| Parameters | Type           | Description                               |
|:----------:|:--------------:| ----------------------------------------- |
| index      | uint16_t       | The index of the target Print Area widget |
| buffer     | const int8_t * | The source of data to be appended         |
| len        | uint16_t       | The number of bytes to be sent            |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    int8_t data = {0xF8, 0x7F, 0x1F};
    mates_appendToPrintArea(7, data, 3); // Append data to PrintArea7
    
<br/>

### **mates_appendStringToPrintArea(index, format, ...)**

This function can be used to append contents to the PrintArea specified by_index_ with the text formed by _format_ and the additional arguments.

<br/>

| Parameters | Type         | Description                                                    |
|:----------:|:------------:| -------------------------------------------------------------- |
| index      | uint16_t     | The index of the target Print Area widget                      |
| format     | const char * | The text to be written to the PrintArea                        |
| ...        | -            | Additional values to replace the format specifiers in _format_ |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example No. 1: 
    mates_appendToPrintArea(8, "Mates"); // Append "Mates" to PrintArea8

#### Example No. 2: 
    int value = 108;
    mates_appendToPrintArea(9, "Value: %d", 108); // Append value as text to PrintArea9

<br/>

### **mates_appendToScope(index, buffer, len)**

This function can be used to append a number of 16-bit values (_len_) from the data in _buffer_ to the Scope widget specified by_index_ .

<br/>

| Parameters | Type            | Description                          |
|:----------:|:---------------:| ------------------------------------ |
| index      | uint16_t        | The index of the target Scope widget |
| buffer     | const int16_t * | The source of data to be appended    |
| len        | uint16_t        | The number of values to be sent      |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example: 
    int8_t data = {0xF8, 0x7F, 0x1F};
    mates_appendToPrintArea(7, data, 3); // Append data to PrintArea7

<br/>

### **mates_updateDotMatrix(index, format, ...)**

This function can be used to append contents to the PrintArea specified by_index_ with the text formed by _format_ and the additional arguments.

<br/>

| Parameters | Type         | Description                                                    |
|:----------:|:------------:| -------------------------------------------------------------- |
| index      | uint16_t     | The index of the target Print Area widget                      |
| format     | const char * | The text to be written to the PrintArea                        |
| ...        | -            | Additional values to replace the format specifiers in _format_ |

<br/>

**Return:** success or failure (_boolean_)

<br/>

#### Example No. 1: 
    mates_updateDotMatrix(8, "Mates"); // Update DotMatrix0 to "Mates"

#### Example No. 2: 
    int value = 108;
    mates_updateDotMatrix(9, "Value: %d", 108); // Update DotMatrix0 to show value

<br/>

### **mates_getVersion()**

This function can be used to query the version number of the library.

<br/>

**Return:** Version Information (_char *_)

<br/>

#### Example:
    // Get the library version number as string
    String matesVersion = mates_getVersion();

### **mates_getCompatibility()**

This function can be used to query the version number of Mates Studio compatible with the version of the library.

<br/>

**Return:** Compatibility Version Information (_char *_)

<br/>

#### Example:
    // Get the library version number as string
    String matesVersion = mates_getVersion();

### **mates_getError()**

This function can be used to investigate errors that occurred while controlling the display module. Detailed information of the possible errors is discussed in [here](mates/includes/errors.md).

#### Example:
    // Checks the last error that occurred
    int error = mates_getError();
    if (error == MATES_ERROR_NONE) {
        // Last command was successful
    }

<br/>
