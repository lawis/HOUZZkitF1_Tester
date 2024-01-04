#ifndef __HFT_DEVICE_H__
#define __HFT_DEVICE_H__

#include <Arduino.h>
#include "avdweb_Switch.h"
#include "CyMCP23016.h"

#define RELAY_PIN  8

class HFTDevice{

private:

    Switch toggleSwitch = Switch(RELAY_PIN);
    CyMCP23016 mcpU7;
    CyMCP23016 mcpU6;

public:

    HFTDevice();

    ~HFTDevice();

    void init();

    void relayOn();

    void relayOff();

    uint8_t mcpU6Read(uint8_t pin);

    uint8_t mcpU7Read(uint8_t pin);

};

#endif //__HFT_DEVICE_H__



/*
01 X            
02 X                
03 U6_1.0           0B4         12      X
04 X                
05 U6_1.1           0B3         11      X
06 GND
07 U6_1.2           4D2         154     X
08 TX_DEBUG
09 GND
10 RX_DEBUG
11 U6_1.3           0C4         20      X
12 U6_1.4           0B6         14      X
13 U6_1.5           3A4         100     √
14 GND              
15 U6_1.7           3A6         102     X
16 U6_1.6           3A5         101     √
17 X
18 U6_0.7           3A1         97      √
19 U6_0.6           2D6         94      √
20 GND
21 U6_0.4           2D7         95      √
22 U6_0.5           3A2         98      √
23 U6_0.2           3A0         96      √
24 U6_0.3           2D5         93      √
25 GND
26 U6_0.1           2D4         92      √
27 U7_0.1           1A0         32      X
28 U6_0.0           1A1         33      X
29 U7_0.0           3B5         109     X
30 GND
31 U7_0.2           0C7         23      X
32 U7_0.3           0C1         17      X
33 U7_0.4           0C0         16      X
34 GND
35 U7_0.5           0B5         13      X
36 U7_1.0           2D3         91      √
37 U7_0.7           3A3         99      X
38 U7_1.1           3C3         115     √
39 GND
40 U7_0.6           2D2         90      √



*/