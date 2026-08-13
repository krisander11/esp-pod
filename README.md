This is a retro themed mp3 player built around the esp32-wroom-1, 
this is version 1.3, this verson can play audio, browse through songs,
add favorite songs, remane songs, artists and setsong duration. 
It has an inbuild keyboard wich makes it easy to type and save songs. 

The components requried are 
1. ESP-32 Wroom-1
2. DF Player Mini
3. 1.3 inch OLED display
4. 6 Micro switches
5. SD Card
6. Wires
7. Battery (3.7v, over 1000mah)
8. TP5406 charging module
9. 3.7v to 5v booster
10. Speaker 3w 4ohm (not compulsory if you want to use aux or headphones instead)

    Wiring
Signal   	       ESP32           Pin	Notes
OLED SDA	       GPIO21	
OLED SCL	       GPIO22	
OLED VCC/GND     3.3V(or 5v based on display) / GND	
DFPlayer RX	     GPIO17	         through a 1kΩ resistor in series — protects the DFPlayer's 3.3V-tolerant input
DFPlayer TX	     GPIO16	           direct
DFPlayer DAC_L     aux out left
DFPlayer DAC_R     aux out right
DFPlayer GND       AUX Ground
DFPlayer SPK1/SPK2	→ speaker	DFPlayer's own onboard amp, drives speaker for local playback
BTN_UP	          GPIO32	other leg to GND (internal pull-up, no resistor needed)
BTN_DOWN	        GPIO33	same
BTN_LEFT	        GPIO25	
BTN_RIGHT	        GPIO26
BTN_SELECT	      GPIO27	
BTN_MENU	        GPIO14	
    
I will add version 1.4 soon, it will include bluetooth udio streaming.
