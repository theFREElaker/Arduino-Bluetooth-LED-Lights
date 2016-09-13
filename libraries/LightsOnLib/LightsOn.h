#ifndef _LIGHTSON_H_
#define _LIGHTSON_H_

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define PIN 2
#define NUM_PIXELS_IN_STRIP 100

class LightsOn {

public:
	void update(uint8_t data[], uint8_t length);

private:
	// Parameter 1 = number of pixels in strip
	// Parameter 2 = Arduino pin number (most are valid)
	// Parameter 3 = pixel type flags, add together as needed:
	//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
	//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
	//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
	//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
	Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS_IN_STRIP, PIN, NEO_RGB + NEO_KHZ800);

	// Fade controls how quickly the pixels will fade during
	//  tough events
	uint8_t fade;

	// Brightness of the strip: 0 - 255
	uint8_t brightness;

	// These are the rgb values that are set on the iphone
	//  for touch events and strip coloration
	uint8_t red, green, blue;

	// Preset Strip Animations
	uint8_t preset;

	void touch(uint8_t pixel);
	void setFade(uint8_t a_fade);
	void setBrightness(uint8_t a_brightness);
	void setColor(uint8_t r, uint8_t g, uint8_t b);
	void updatePreset(uint8_t a_preset);

};

#endif