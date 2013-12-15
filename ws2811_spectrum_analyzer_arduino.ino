/*
     Arduino - Processing WS2811 Spectrum Analyzer.

This controls 100 WS2811 RGB LEDs for a "waterfall" spectrum analyzer.
A potentiometer is used on A0 to control the sensitivity.
More information, including full parts list and pictures of the final product can be seen or requested at www.12vtronix.com.
This program is part of the code used to drive the 16 x 64 RGB spectrum analyzer also found on www.12vtronix.com.
Based on adafruit neopixel library

           Created:  4th Dec 2013 by Stephen Singh.
     Last Modified: 14th Dec 2013 by Stephen Singh.

*/




#include <Adafruit_NeoPixel.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(100, 13 , NEO_RGB + NEO_KHZ800);

int colourpattern;
int time; 
int cycles;
double soundval = 0;
int average = 0;
double temp1;


//Analog Input Pins
int left_channel = 0;
int right_channel = 1;

//Light Control Pins
int light_data = 3;
int light_clk = 2;

//Set Strip Constants
const int length = 100;
const int half = length/2;

uint32_t half_array[half];


void setup() {
  
  
    for(int i=0; i<half;i++)
  {
    half_array[i] = 0;
  }
  Serial.begin(115200);
  strip.begin();
  strip.show();
}

void loop() {
  temp1 = temp1 - 50;
  average = temp1 * ((analogRead(A0)/2)/150);
  
  if (average < 20)
  {
    average = 0;
  }
  
  average = constrain(average, 0 , 255);

  
  //Set the hue (0-255) and 24-bit color depending on left channel value
  byte stringoutput = average;

  uint32_t color = Wheel(stringoutput);
  

  //Serial.println(hue_left);
  
  //Shift the current values.
  for (int i = 0; i<half-1; i++)
  {
    half_array[i] = half_array[i+1];
  }
  
  //Fill in the new value at the end of each array
  half_array[half-1] = color;

  
  //Go through each Pixel on the strip and set its color
  for (int i=0; i<half; i++)
  {
    //set pixel color
    strip.setPixelColor(i, half_array[i]);
    strip.setPixelColor(length-i-1, half_array[i]);
  }


  //Display the new values
  strip.show();
  
  int x = 0;
  double sum;
  while (x < 1)
  {
    
  if(Serial.read() == 'M') {      
    soundval = Serial.parseInt();
    x = x+1;
    sum = sum + soundval;
  }
  
  }
  
  temp1 = sum / 1;
  

}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition b - g - r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85 ) {
   return Color(0,0, WheelPos * 3);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(0, WheelPos * 3 , 255 - WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

