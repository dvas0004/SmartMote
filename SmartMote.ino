// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>

// From particle documentation: enable OS system threading
SYSTEM_THREAD(ENABLED);

// This is where the NeoPixel Stick is plugged in
int led = D2; 

// This is where the mini-relay "set" and "unset" pins are connected respectively
int RELAY_SET = D3;
int RELAY_UNSET = D4;

// Even though my stick has 8 LEDS, the last two wouldn't work if unless I used 10
int NUMBER_OF_LEDS = 10;

// From Adafruit documentation : initialize the stick. Note the use of "SK6812RGBW"
// you may need to experiment with the last value - this is the one that worked for me
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_OF_LEDS, led, SK6812RGBW);

// Thread setup. "continue_thread" is used as a flag to control the thread
bool continue_thread = false;
Thread thread("ledThread", threadFunction);


// Next we go into the setup function.
void setup() {
    
    
    // First, declare all of our pins. This lets our device know which ones will be used for outputting voltage, and which ones will read incoming voltage.
    pinMode(led,OUTPUT); // Our LED pin is output (lighting up the NeoPixel)    
    pinMode(RELAY_SET, OUTPUT);
    pinMode(RELAY_UNSET, OUTPUT);
    
    // We are also going to declare a Particle.function so that we can turn the NeoPixel LEDs on and off from the cloud.
    Particle.function("led",ledToggle);
    // We do the same for the relay    
    Particle.function("relay",relayToggle);

    // Make sure the NeoPixel is blank when we start
    strip.begin();
    strip.clear();
    strip.show();

}


// Next is the loop function...

void loop() {

    // In this case we actually dont do anything here - all the work is done by the thread
    
}

void threadFunction(void *param) {
  strip.begin();
  int counter = 0;
	while(true) {
      // TODO: cleanup the code below - there's some repetition with two "switch" statements that can be removed
	    if (continue_thread) {
            // FADE IN the LEDs, and cycle between red, green and blue
            for( int a = 0; a < 150; a = a + 5 ) {
                for( int l = 0; l < NUMBER_OF_LEDS; l = l + 1 ) {
                    
                    int r = 0;
                    int g = 0;
                    int b = 0;
                    
                    switch (counter){
                        case(0): 
                            r = a;
                            break;
                        case(1):
                            g = a;
                            break;
                        case(2):
                            b = a;
                            break;
                    }
                    //GRBW
                    strip.setPixelColor((uint16_t)l, (uint8_t)g, (uint8_t)r, (uint8_t)b, (uint8_t)0);
                }
                strip.show();
                delay(100);
            }
            // FADE OUT the LEDs, and cycle between red, green and blue
            for( int a = 150; a >= 0; a = a - 5 ) {
                for( int l = 0; l < NUMBER_OF_LEDS; l = l + 1 ) {
                    
                    int r = 0;
                    int g = 0;
                    int b = 0;
                    
                    switch (counter){
                        case(0): 
                            r = a;
                            break;
                        case(1):
                            g = a;
                            break;
                        case(2):
                            b = a;
                            break;
                    }
                    //GRBW
                    strip.setPixelColor((uint16_t)l, (uint8_t)g, (uint8_t)r, (uint8_t)b, (uint8_t)0);
                }
                strip.show();
                delay(100);
            }
            counter = (counter + 1)%3;
        } else {
            // If the control variable "continue_thread" is set to False, - clear the neopixels
            for( int b = 0; b < NUMBER_OF_LEDS; b = b + 1 ) {
                strip.setPixelColor((uint16_t)b, (uint8_t)0, (uint8_t)0, (uint8_t)0, (uint8_t)0);
            }
            strip.show();
            delay(1000);
        }
	}
}

// Define our relayToggle function, which we can call through the particle cloud

int relayToggle(String command) {
    if (command=="on"){
        digitalWrite(RELAY_UNSET, LOW);
        digitalWrite(RELAY_SET, HIGH);
        return 1;
    }
    else if (command=="off"){
        digitalWrite(RELAY_SET, LOW);
        digitalWrite(RELAY_UNSET, HIGH);
        return 0;
    }
        
    
    return -1;
}


// Define our ledToggle function, which we can call through the particle cloud

int ledToggle(String command) {
    
    if (command=="on") {
        
        if (!continue_thread){
            continue_thread=true;
        }
        
        return 1;
    }
    else if (command=="off") {
        continue_thread=false;
        strip.begin();
            
        return 0;
    }
    else {
        return -1;
    }

}

