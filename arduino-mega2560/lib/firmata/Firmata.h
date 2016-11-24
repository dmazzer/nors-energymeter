/*
  Firmata.h - Firmata library v2.5.4 - 2016-10-23
  Copyright (c) 2006-2008 Hans-Christoph Steiner.  All rights reserved.
  Copyright (C) 2009-2016 Jeff Hoefs.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

#ifndef Firmata_h
#define Firmata_h

#include "Boards.h"  /* Hardware Abstraction Layer + Wiring/Arduino */
#include "FirmataConstants.h"
#include "FirmataMarshaller.h"
#include "FirmataParser.h"

/* DEPRECATED as of Firmata v2.5.1. As of 2.5.1 there are separate version numbers for
 * the protocol version and the firmware version.
 */
#define FIRMATA_MAJOR_VERSION           2 // same as FIRMATA_PROTOCOL_MAJOR_VERSION
#define FIRMATA_MINOR_VERSION           5 // same as FIRMATA_PROTOCOL_MINOR_VERSION
#define FIRMATA_BUGFIX_VERSION          1 // same as FIRMATA_PROTOCOL_BUGFIX_VERSION

// extended command set using sysex (0-127/0x00-0x7F)
/* 0x00-0x0F reserved for user-defined commands */
// these are DEPRECATED to make the naming more consistent
#define FIRMATA_STRING          0x71 // same as STRING_DATA
#define SYSEX_I2C_REQUEST       0x76 // same as I2C_REQUEST
#define SYSEX_I2C_REPLY         0x77 // same as I2C_REPLY
#define SYSEX_SAMPLING_INTERVAL 0x7A // same as SAMPLING_INTERVAL

// pin modes
//#define INPUT                 0x00 // defined in Arduino.h
//#define OUTPUT                0x01 // defined in Arduino.h
// DEPRECATED as of Firmata v2.5
#define ANALOG                  0x02 // same as PIN_MODE_ANALOG
#define PWM                     0x03 // same as PIN_MODE_PWM
#define SERVO                   0x04 // same as PIN_MODE_SERVO
#define SHIFT                   0x05 // same as PIN_MODE_SHIFT
#define I2C                     0x06 // same as PIN_MODE_I2C
#define ONEWIRE                 0x07 // same as PIN_MODE_ONEWIRE
#define STEPPER                 0x08 // same as PIN_MODE_STEPPER
#define ENCODER                 0x09 // same as PIN_MODE_ENCODER
#define IGNORE                  0x7F // same as PIN_MODE_IGNORE

// TODO make it a subclass of a generic Serial/Stream base class
class FirmataClass
{
  public:
    FirmataClass();
    /* Arduino constructors */
    void begin();
    void begin(long);
    void begin(Stream &s);
    /* querying functions */
    void printVersion(void);
    void blinkVersion(void);
    void printFirmwareVersion(void);
    //void setFirmwareVersion(byte major, byte minor);  // see macro below
    void setFirmwareNameAndVersion(const char *name, byte major, byte minor);
    void disableBlinkVersion();
    /* serial receive handling */
    int available(void);
    void processInput(void);
    void parse(unsigned char value);
    boolean isParsingMessage(void);
    /* serial send handling */
    void sendAnalog(byte pin, int value);
    void sendDigital(byte pin, int value); // TODO implement this
    void sendDigitalPort(byte portNumber, int portData);
    void sendString(const char *string);
    void sendString(byte command, const char *string);
    void sendSysex(byte command, byte bytec, byte *bytev);
    void write(byte c);
    /* attach & detach callback functions to messages */
    void attach(uint8_t command, callbackFunction newFunction);
    void attach(uint8_t command, systemCallbackFunction newFunction);
    void attach(uint8_t command, stringCallbackFunction newFunction);
    void attach(uint8_t command, sysexCallbackFunction newFunction);
    void detach(uint8_t command);

    /* access pin state and config */
    byte getPinMode(byte pin);
    void setPinMode(byte pin, byte config);
    /* access pin state */
    int getPinState(byte pin);
    void setPinState(byte pin, int state);

    /* utility methods */
    void sendValueAsTwo7bitBytes(int value);
    void startSysex(void);
    void endSysex(void);

  private:
    FirmataMarshaller marshaller;
    FirmataParser parser;
    Stream *FirmataStream;
    /* firmware name and version */
    byte firmwareVersionCount;
    byte *firmwareVersionVector;
    /* pin configuration */
    byte pinConfig[TOTAL_PINS];
    int pinState[TOTAL_PINS];

    boolean blinkVersionDisabled;

    /* private methods ------------------------------ */
    void strobeBlinkPin(byte pin, int count, int onInterval, int offInterval);
    friend void FirmataMarshaller::sendValueAsTwo7bitBytes(uint16_t value);
};

extern FirmataClass Firmata;

/*==============================================================================
 * MACROS
 *============================================================================*/

/* shortcut for setFirmwareNameAndVersion() that uses __FILE__ to set the
 * firmware name.  It needs to be a macro so that __FILE__ is included in the
 * firmware source file rather than the library source file.
 */
#define setFirmwareVersion(x, y)   setFirmwareNameAndVersion(__FILE__, x, y)

#endif /* Firmata_h */
