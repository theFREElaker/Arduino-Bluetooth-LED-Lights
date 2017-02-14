// (first color set value) Green = RED
// (second color set value) Red = GREEN
// (thirs color set value) Blue = BLUE

#include <Adafruit_NeoPixel.h>
#include <SimpleTimer.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 2
#define FADE_FACTOR 2
#define NUM_PIXELS_IN_STRIP 200

// Parameter 1 = number of pixels in strip 
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS_IN_STRIP, PIN, NEO_RGB + NEO_KHZ800);

// the timer object
SimpleTimer timer;

//Globals
boolean liveMode = true; //user can control ligths via bluetooth
boolean rainbowMode = false; //rainbow is turned off by default
uint8_t turnRainbowOn = 101;
uint8_t turnLiveModeOn = 100;
uint8_t fadeSpeed = 2;
int fadeSpeedTimer = 2;
int timerID = 0;

uint8_t rr = 255;
uint8_t bb = 80;
uint8_t gg = 170;
uint8_t ddelays = 250;
uint8_t pixelNUM = 0;

void setup() {
   // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  Serial.begin(115200);  //initial the Serial

  // Setup Timer for fade
  timerID = timer.setInterval(fadeSpeedTimer, fadeByFactor);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  setStrandColor(80,255,170,50); //brg
}

void loop(){
  uint8_t pixel[6] = {6, 10, 220, 30, 35, 20};
  
  //setStrandColor(40,150,70,50); //brg
  

  //wait until 5 bytes in buffer
  if(Serial.available() >= 6) {
    
    pixel[0] = Serial.read(); // Blue
    pixel[1] = Serial.read(); // Green
    pixel[2] = Serial.read(); // Red
    pixel[3] = Serial.read(); // Position: LED number 0-48
    pixel[4] = Serial.read(); // if byte == 101 turn on Rainbowmode
    pixel[5] = Serial.read(); // Delay val: 0 - 255

    //print out what we got
    Serial.print("\nblue[0]: ");
    Serial.print(pixel[0]); // blue
    Serial.print("\t green[1]: ");
    Serial.print(pixel[1]); // green
    Serial.print("\t red[2]: ");
    Serial.print(pixel[2]); // red
    Serial.print("\t Pixel[3]: ");
    Serial.print(pixel[3]); // Pixel Number
    Serial.print("\t Mode[4]: ");
    Serial.print(pixel[4]); // Panel Mode
    Serial.print("\t delay[5]: ");
    Serial.print(pixel[5]); // delay
    Serial.println("");

    rr = pixel[2];
    gg = pixel[1];
    bb = pixel[0];
    pixelNUM = pixel[3];
    ddelays = pixel[5];
  
  if(pixel[4] == turnRainbowOn){
    setStrandColor(0,0,0,50);
  } else {
    strip.setBrightness(pixel[5]);
    setStrandColor(pixel[0],pixel[2],pixel[1],50); 
  }
  }
}

void oneAtATime(){
  uint8_t r = 255;
  uint8_t g = 0;
  uint8_t b = 0;
  for(uint16_t i = 0; i < strip.numPixels(); i++){
    if (i == 0) {
      strip.setPixelColor(i, r, g, b); 
      strip.setPixelColor(NUM_PIXELS_IN_STRIP -1,0,0,0);
    } else {
      strip.setPixelColor(i, r, g, b);
      strip.setPixelColor(i-1, 0, 0, 0);  
    }
    strip.show();
    delay(50);
  }
}

void lightUpOneLED(uint8_t pixel, uint8_t r, uint8_t g, uint8_t b){
  strip.setPixelColor(pixel, r, g, b);
}

void slowlyFill(){
  for(uint16_t i = 0; i < strip.numPixels(); i++){
    if (i == 0) {
      strip.setPixelColor(i, rr, gg, bb); 
    } else {
      strip.setPixelColor(i, rr, gg, bb);
      strip.setPixelColor(i-1, 0, 0, 0);  
    }
    strip.show();
    delay(50);
  }
}

void fadeByFactor(){
  uint8_t *pixels = strip.getPixels();
  uint8_t *rgb;
  
  for(uint16_t i = 0; i < strip.numPixels(); i++){
    rgb = &pixels[i*3];
    
    if(rgb[0] < FADE_FACTOR){
      rgb[0] = 0;
    } else {
      rgb[0] -= FADE_FACTOR;
    }

    if(rgb[1] < FADE_FACTOR){
      rgb[1] = 0;
    } else {
      rgb[1] -= FADE_FACTOR;
    }

    if(rgb[2] < FADE_FACTOR){
      rgb[2] = 0;
    } else {
      rgb[2] -= FADE_FACTOR;
    }
    
  }

  strip.show();
}

void printPixels(){
  uint8_t *pixels = strip.getPixels();
  uint8_t *r;
  
  for(uint16_t i = 0; i < strip.numPixels(); i++){
    r = &pixels[i*3];
    Serial.print("\n");
    Serial.print("r:");
    Serial.print(r[0]);
    Serial.print(" g:");
    Serial.print(r[1]);
    Serial.print(" b:");
    Serial.print(r[2]);
    Serial.println("\n");
  } // r1 g0 b2
}

// Um why does this loop through all of the pixels...?
void lightOnlyOne(uint8_t pixel){
  uint8_t g = 255, r = 255, b = 255;
  for(uint16_t i = 0; i < strip.numPixels(); i++){
    if(i == pixel){
      strip.setPixelColor(pixel, (g-(i*5)), (r-(i*5)), (b-(i*5)));
    } else {
      strip.setPixelColor(i, 0, 0, 0);
    }
  }
  strip.show();
  delay(10);
}

void lightUntil(uint8_t pixel){
  if(pixel > 50){
    Serial.println("Pixel out of range");
  } else {
    uint8_t g = 250, r = 250, b = 250;
    for(uint16_t i = 0; i < pixel; i++){
      g -= i*5;
      r -= i*5;
      b -= i*5;
      strip.setPixelColor(i, 255, 0, 0);
    }
    for(uint16_t k = pixel; k < strip.numPixels(); k++){ 
      strip.setPixelColor(k, 0, 0, 0); 
    }
    strip.show();
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void setStrandColor(uint8_t r, uint8_t g, uint8_t b, uint16_t wait){
  for(uint16_t i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
  delay(wait);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      if(Serial.available() >= 5) {
        uint8_t pixel[5] = {6, 10, 220, 30, 35};
        pixel[0] = Serial.read(); 
        pixel[1] = Serial.read();
        pixel[2] = Serial.read();
        pixel[3] = Serial.read();
        pixel[4] = Serial.read(); //if last byte == 101 turn on Rainbowmode
        pixel[5] = Serial.read();

// Just for debugging
        Serial.println("-- Inside of RainbowCycle --");
        Serial.println("-- Printing off extra transmissions --");
        Serial.print("Number of Bits: ");
        Serial.println(Serial.available());
        while(Serial.available() > 0){
          uint8_t extra = Serial.read();
          Serial.println(extra);
        }
        Serial.println("-- Done Printing Extra --");
 // End debugging code
 
        if (pixel[4] == turnLiveModeOn){
          rainbowMode = false; 
          liveMode = true;
          Serial.println("break");
          break;
          } //RainbowMode off leaving rainbow
      }
    }
    if(rainbowMode == true) {
      strip.show();
      delay(wait);
    }
  }
}

