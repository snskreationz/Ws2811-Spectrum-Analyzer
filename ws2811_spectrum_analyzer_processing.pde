/*
     Arduino - Processing WS2811 Spectrum Analyzer

This program does an FFT for 20 - 60hz then sends the result to an arduino
The signal being processed would be an input signal to the computer though the line in audio port
More information, including full parts list and pictures of the final product can be seen or requested at www.12vtronix.com
This program is part of the code used to drive the 16 x 64 RGB spectrum analyzer also found on www.12vtronix.com
Based on http://processing.org/learning/libraries/forwardfft.html by ddf

           Created: 12th Dec 2013 by Stephen Singh
     Last Modified: 14th Dec 2013 by Stephen Singh

*/

 
import ddf.minim.analysis.*;
import ddf.minim.*;
import processing.serial.*; //library for serial communication
 
Serial arduinoport;
 
Minim minim;
AudioInput in;
FFT fft;
float[] peaks;

int peak_hold_time = 1;  // how long before peak decays
int[] peak_age;  // tracks how long peak has been stable, before decaying

// how wide each 'peak' band is, in fft bins
int binsperband = 5;
int peaksize; // how many individual peak bands we have (dep. binsperband)
float gain = 20; // in dB
float dB_scale = 2.0;  // pixels per dB

int buffer_size = 4096;  // also sets FFT size (frequency resolution)
float sample_rate = 200000;

int spectrum_height = 140; // determines range of dB shown

int freq_array = 0;

int i,g;
float f;


float freq_height = 0;  //avg amplitude of each freq band

void setup()
{
  size(200, 200);

  minim = new Minim(this);
  
  arduinoport = new Serial(this, "COM11" , 115200); //set baud rate
  
 
  in = minim.getLineIn(Minim.MONO,buffer_size,sample_rate);
 
  // create an FFT object that has a time-domain buffer 
  // the same size as line-in's sample buffer
  fft = new FFT(in.bufferSize(), in.sampleRate());
  // Tapered window important for log-domain display
  fft.window(FFT.HAMMING);

  // initialize peak-hold structures
  peaksize = 1+Math.round(fft.specSize()/binsperband);
  peaks = new float[peaksize];
  peak_age = new int[peaksize];
}


void draw()
{
  
freq_array = 0;



  fft.forward(in.mix);
  

  
  freq_height = (fft.calcAvg((float) 20, (float) 60));
  

  
  

    String sta = "M";
    String ggg = str(freq_height);
    String com = ",";
    String newl = "\n";
    String tosend = sta + ggg + newl;
    arduinoport.write(tosend); //send data to arduino
    


}
 
 
void stop()
{
  // always close Minim audio classes when you finish with them
  in.close();
  minim.stop();
 
  super.stop();
}


