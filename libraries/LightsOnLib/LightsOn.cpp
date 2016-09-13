#include "LightsOn.h"
#include "ComsStation.h"

void LightsOn::update(uint8_t data[], uint8_t length){

	if (data[0] > length) {
		Serial.print("LightsOn::update - data[0] > length\n");
	} else {
		uint8_t messageType = data[1];
		
		switch(messageType) {
			case touchEvent: 
				touch(data[INDEX_TOUCH_PIXEL]);
				break;
			case fadeTimerUpdateEvent:
				setFade(data[INDEX_FADE]);
				break;
			case colorUpdateEvent:
				setColor(data[INDEX_RED], data[INDEX_GREEN], data[INDEX_BLUE]);
				break;
			case presetUpdateEvent:
				updatePreset(data[INDEX_PRESET]);
				break;
			default:
				// Well this shouldn't really happen
				Serial.print("\nLightsOn::update -> UhOh!\n");
		}
	}
}

void LightsOn::touch(uint8_t pixel){
	//TODO
}

void LightsOn::setFade(uint8_t a_fade){
	fade = a_fade;
	// Update fade timer
}

void LightsOn::setBrightness(uint8_t a_brightness){
	brightness = a_brightness;
	// update strip brightness
}

void LightsOn::setColor(uint8_t r, uint8_t g, uint8_t b){
	red = r;
	green = g;
	blue = b;
	// Update Color
}

void LightsOn::updatePreset(uint8_t a_preset){
	preset = a_preset;
	// display preset
}








