/*   Program Architecture BreakDown
 * LightsOn.ino includes two libraries: ComsStationh and LightsOn.h
 *  both are libraries written in cpp and encapsulate the bulk of the
 *  functionality of LightsOn.ino
 *  
 * ComsStation.h
 *    - Handles communication protocol
 *    - Strips transmission specific protocol bytes
 *    - Returns raw data
 *    
 * LightsOn.h
 *    - Handles translating the data from ComsStation to physical light updates
 *    - Interfaces with the Adafruit_Neopixel library to update the light state
 */


#include <ComsStation.h>
#include <LightsOn.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

/*****************************************
       Constants
*****************************************/
#define MIN_MESSAGE_LENGTH 7
#define FADE_FACTOR 2
#define BUFFER_SIZE 255

// when messageStartCodeReceived and messageStopCodeReceived == -2
// neither of the bytes have been received
// when messageStartCodeReceived and messageStopCodeReceived == -1
// START_MESSAGE_CODE_BYTE_1 has been found
int messageStartCodeReceived = -2;
int messageEndCodeReceived = -2;

uint8_t numElementsInBuffer = 0;
uint8_t bufferArray[BUFFER_SIZE];

ComsStation comSatOnline;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  Serial.begin(115200);

  // Set the buffer to 0's
  clearBuffer();
}

void loop() {
  if(Serial.available()){
    readData(); 
  }
  /*
  uint8_t numBytesAvailable = messageReceived();
  
  if((numBytesAvailable >= MIN_MESSAGE_LENGTH) && (Serial.peek() == END_MESSAGE_CODE_BYTE_2)){
    
    // the actual data will always be at least 5 bytes shorter than the transmitted
    //  message (See Message Format in ComsStation.h). But becuase we are adding
    //  the valid data counter at the beginning of the data array we need to add 1
    //  to its length
    uint8_t dataLen = numBytesAvailable - NUM_TRANSMISSION_INFO_BYTES + 1;
    uint8_t receivedMessage[numBytesAvailable], data[numBytesAvailable+1];
    
    readData(receivedMessage, numBytesAvailable);
    errorType error = comSatOnline.decode(receivedMessage, numBytesAvailable, data, dataLen);
    
    if (error == messageGoodToBeDecoded) {
      //lightsOn.update(data, dataLen);
    } else {
      // Refer to ComsStation.h for message error codes
      Serial.print("Message ERROR Code: ");
      Serial.println(error);
    }
  }*/
  
  
}

void readData(){
  uint8_t readByte;

  while (Serial.available()) {
    readByte = Serial.read();
    
    switch (messageStartCodeReceived) {
      case -2:
        // look for START_MESSAGE_CODE_BYTE_1
        
        if(readByte == START_MESSAGE_CODE_BYTE_1){
          if(addToBuffer(readByte)){
            messageStartCodeReceived = -1;
          } else {
            Serial.print("CASE -2: Well shoot, this shouldn't happen. The buffer is full...\n");
          }
        } // else {//just throw that value away}
        break;
        
      case -1:
        // look for START_MESSAGE_CODE_BYTE_2
        
        if (readByte == START_MESSAGE_CODE_BYTE_2) {
          if (addToBuffer(readByte) {
            messageStartCodeReceived = numElementsInBuffer - 1;
          } else {
            Serial.print("CASE -1: Well shoot, this shouldn't happen. The buffer is full...\n");
          }
        } else {
          messageStartCodeReceived = -2;
          clearBuffer(); // @Excessive??
        }
        break;
        
      default:
        // add data to buffer

          // TODO
          // Check if END_MESSAGE_CODE_BYTE_1
          // Check if next is END_MESSAGE_CODE_BYTE_2
          switch (messageEndCodeReceived) {
            case -2:
              // Looking for END_MESSAGE_CODE_BYTE_1

              if (readByte == END_MESSAGE_CODE_BYTE_1) {
                if (addToBuffer(readByte)) {
                  messageEndCodeReceived = -1;
                } else {
                  Serial.print("DEFAULT: Well shoot, this shouldn't happen. The buffer is full...\n");
                }
              }
              break;

   // Double check this. It seems that maybe removing the stuffed bytes right here
   //  is the best option. If this is taken inside of ComsStation.h this could be handled
   //  more elegantly. BUT that will compromise the low coupling of the ComsStation library
            case -1:
              // Looking for END_MESSAGE_CODE_BYTE_2

              if (readByte == END_MESSAGE_CODE_BYTE_2) {
                if (addToBuffer(readByte)) {
                  // reached end of message
                  messageEndCodeReceived = numElementsInBuffer - 1;
                  // @TODO send message to comsStation
                } else {
                  Serial.print("DEFAULT: Well shoot, this shouldn't happen. The buffer is full...\n");
                }
              } else if (readByte == END_MESSAGECODE_BYTE_1) {
                // Stuffed byte, not message termination
                messageEndCodeReceived = -2;
              } else {
                Serial.print("ERROR: message missing stuffed END_MESSAGE_CODE_BYTE_1");
                messageEndCodeReceived = -2;
              }
              break;

            default:
          }
    }
  }
}

/*
uint8_t messageReceived(){
  if (Serial.available() > MIN_MESSAGE_LENGTH){
    return Serial.available();
  } else {
    return 0;
  }
}

void readData(uint8_t message[], uint8_t numBytesAvailable){
  uint8_t i;
  for (i = 0; i < numBytesAvailable; i++){
    message[i] = Serial.read();
    Serial.println(message[i]);
  }
}
*/
void clearBuffer(){
  numElementsInBuffer = 0;
  messageStartCodeReceived = -2;
  messageEndCodeReceived = -2;
  
  int i;
  for(i = 0; i < BUFFER_SIZE; i++){
    bufferArray[i] = 0;
  }
}

bool addToBuffer(uint8_t element){
  if(numElementsInBuffer == BUFFER_SIZE){
    return false;
  } else {
    // Room in the buffer so add it
    bufferArray[numElementsInBuffer] = element;
    numElementsInBuffer += 1;
    return true;
  }
}






