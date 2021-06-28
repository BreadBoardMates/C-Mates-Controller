/* 
 * File:   controller.c
 * Author: cruzj
 *
 * Created on June 18, 2021, 11:31 PM
 */

#include "controller.h"

/* ***** PRIVATE VARIABLES ***** */
MatesError matesError = MATES_ERROR_NONE;
uint16_t matesBufferSize = __MATES_STRING_BUFFER_SIZE__;
uint16_t matesBootTimeout = __MATES_BOOT_TIMEOUT__;
/* ***** PRIVATE VARIABLES ***** */

/* ******* User Functions ****** */

// Prerequisite Functions
void (* _mates_Reset)(void);
uint32_t (* _mates_Millis)(void);
uint8_t (* _mates_SerialAvailable)(void);
uint8_t (* _mates_SerialRead)(void);
void (* _mates_SerialWrite)(uint8_t);

// Setup Functions

void mates_attachHwResetFnc(void (* resetFnc)(void)) {
    _mates_Reset = resetFnc;
}

void mates_attachMillisFnc(uint32_t (* millisFnc)(void)) {
    _mates_Millis = millisFnc;
}

void mates_attachWriteFnc(void (* writeFnc)(uint8_t)) {
    _mates_SerialWrite = writeFnc;
}

void mates_attachReadFnc(uint8_t(* readFnc)(void)) {
    _mates_SerialRead = readFnc;
}

void mates_attachRxCountFnc(uint8_t(* rxCountFnc)(void)) {
    _mates_SerialAvailable = rxCountFnc;
}

bool mates_begin(void) {
    if (!_mates_Reset) {
        return false;
    }
    if (!_mates_Millis) {
        return false;
    }
    if (!_mates_SerialWrite) {
        return false;
    }
    if (!_mates_SerialRead) {
        return false;
    }
    if (!_mates_SerialAvailable) {
        return false;
    }    
    _mates_Reset();
    return _mates_WaitForACK(matesBootTimeout);
}

void mates_setBootTimeout(uint16_t timeout) {
    matesBootTimeout = timeout;
}

bool mates_reset(void) {
    if (!(_mates_Reset)) return false;
    _mates_Reset();
    return _mates_WaitForACK(matesBootTimeout);
}

bool mates_softReset(void) {
    _mates_WriteCommand(MATES_CMD_SYSTEM_RESET);
    return _mates_WaitForACK(matesBootTimeout);
}

// Non-widget functions

bool mates_setBacklight(uint8_t value) {
    _mates_WriteCommand(MATES_CMD_SET_BACKLIGHT);
    _mates_WriteWord((int16_t) value);
    return _mates_WaitForACK(500);
}

bool mates_setPage(uint16_t page) {
    _mates_WriteCommand(MATES_CMD_SET_PAGE);
    _mates_WriteWord((int16_t) page);
    return _mates_WaitForACK(500);
}

int16_t mates_getPage(void) {
    _mates_WriteCommand(MATES_CMD_GET_PAGE);
    return _mates_ReadResponse(500);
}

// Common widget functions

bool mates_setWidgetValue(MatesWidget type, uint8_t index, int16_t value) {
    _mates_WriteCommand(MATES_CMD_SET_WIDGET_VALUE);
    _mates_WriteByte((int8_t) type);
    _mates_WriteByte((int8_t) index);
    _mates_WriteWord(value);
    return _mates_WaitForACK(500);
}

int16_t mates_getWidgetValue(MatesWidget type, uint8_t index) {
    _mates_WriteCommand(MATES_CMD_GET_WIDGET_VALUE);
    _mates_WriteByte((int8_t) type);
    _mates_WriteByte((int8_t) index);
    return _mates_ReadResponse(500);
}

// Widget-specific functions
bool mates_setLedDigitsShortValue(uint8_t index, int16_t value) {
    _mates_WriteCommand(MATES_CMD_SET_WIDGET_VALUE);
    _mates_WriteByte((int8_t) MATES_LED_DIGITS);
    _mates_WriteByte((int8_t) index);
    _mates_WriteWord(value);
    return _mates_WaitForACK(500);
}

bool mates_setLedDigitsLongValue(uint8_t index, int32_t value) {
    return _mates_setWidgetLongValue((int16_t)((MATES_LED_DIGITS << 8) | index), value);
}

bool mates_setLedDigitsFloatValue(uint8_t index, float value) {
    return _mates_setWidgetFloatValue((int16_t)((MATES_LED_DIGITS << 8) | index), value);
}

bool mates_setLedSpectrumValue(uint8_t index, uint8_t gaugeIndex, uint8_t value) {
    _mates_WriteCommand(MATES_CMD_SET_WIDGET_VALUE);
    _mates_WriteByte((int8_t) MATES_LED_SPECTRUM);    
    _mates_WriteByte((int8_t) index);
    _mates_WriteByte((int8_t) gaugeIndex);
    _mates_WriteByte((int8_t) value);
    return _mates_WaitForACK(500);
}

bool mates_setMediaSpectrumValue(uint8_t index, uint8_t gaugeIndex, uint8_t value) {
    _mates_WriteCommand(MATES_CMD_SET_WIDGET_VALUE);
    _mates_WriteByte((int8_t) MATES_MEDIA_SPECTRUM);
    _mates_WriteByte((int8_t) index);
    _mates_WriteByte((int8_t) gaugeIndex);
    _mates_WriteByte((int8_t) value);
    return _mates_WaitForACK(500);
}

/* ***** Utility Functions ***** */
char * mates_getVersion(void) {
    return __MATES_CONTROLLER_LIBRARY_VERSION__;
}

char * mates_getCompatibility(void) {
    return __MATES_STUDIO_COMPATIBILITY_VERSION__;
}

MatesError mates_getError(void) {
    return matesError;
}

/* ***** Support Functions ***** */

// General Support Functions

void _mates_WriteCommand(MatesCommand value) {
    _mates_SerialWrite(MATES_CMD_START_BYTE);
    _mates_SerialWrite((uint8_t) (value >> 8));
    _mates_SerialWrite((uint8_t) value);
}

void _mates_WriteByte(int8_t value) {
    _mates_SerialWrite((uint8_t) value);
}

void _mates_WriteWord(int16_t value) {
    _mates_SerialWrite((uint8_t) (value >> 8));
    _mates_SerialWrite((uint8_t) value);
}

void _mates_WriteLong(int32_t value) {
    _mates_SerialWrite((uint8_t) (value >> 24));
    _mates_SerialWrite((uint8_t) (value >> 16));
    _mates_SerialWrite((uint8_t) (value >> 8));
    _mates_SerialWrite((uint8_t) value);
}

void _mates_WriteFloat(float value) {
    int32_t * longPtr = (int32_t*) &value; // cast float to long
    _mates_WriteLong(*longPtr);
}

void _mates_WriteString(const char * str) {
    do {
        _mates_SerialWrite(*str);
    } while (*str++);
}

void _mates_WriteByteArray(const int8_t * buf, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        _mates_SerialWrite((uint8_t) buf[i]);
    }
}

void _mates_WriteWordArray(const int16_t * buf, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        _mates_SerialWrite((uint8_t) (buf[i] >> 8));
        _mates_SerialWrite((uint8_t) buf[i]);
    }
}

bool _mates_WaitForACK(uint16_t timeout) {
    uint32_t startTime = _mates_Millis();
    while (_mates_SerialAvailable() < 1) {
        if (_mates_Millis() - startTime >= timeout) {
            matesError = MATES_ERROR_COMMAND_TIMEOUT;
            return false;
        }
    }
    bool res = _mates_SerialRead() == 0x06;
    matesError = res ? MATES_ERROR_NONE : MATES_ERROR_COMMAND_FAILED;
    return res;
}

int16_t _mates_ReadWord(void) {
    int16_t value = _mates_SerialRead();
    value = (value << 8) | (_mates_SerialRead());
    return value;
}

int16_t _mates_ReadResponse(uint16_t timeout) {
    if (!_mates_WaitForACK(500)) return -1;
    uint32_t startTime = _mates_Millis();
    while (_mates_SerialAvailable() < 2) {
        if (_mates_Millis() - startTime >= timeout) {
            matesError = MATES_ERROR_RESPONSE_TIMEOUT;
            return -1;
        }
    }
    return _mates_ReadWord();
}

// Widget Support Functions

bool _mates_setWidgetLongValue(int16_t widget, int32_t value) {
    _mates_WriteCommand(MATES_CMD_SET_WIDGET_32VAL);
    _mates_WriteWord(widget);
    _mates_WriteLong(value);
    return _mates_WaitForACK(500);
}

bool _mates_setWidgetFloatValue(int16_t widget, float value) {
    _mates_WriteCommand(MATES_CMD_SET_WIDGET_32VAL);
    _mates_WriteWord(widget);
    _mates_WriteFloat(value);
    return _mates_WaitForACK(500);
}

bool _mates_updateDotMatrix(uint16_t index, const int8_t * buf, uint16_t len) {
    _mates_WriteCommand(MATES_CMD_UPDATE_DOT_MATRIX);
    _mates_WriteWord((int16_t) index);
    _mates_WriteWord((int16_t) len);
    _mates_WriteByteArray(buf, len);
    return _mates_WaitForACK(500);
}

