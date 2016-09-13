/*
Description: This class parses the bluetooth data that 
			 is sent from the app. The data is parsed
			 as 8 bit integers. Each message is encoded
			 using a Start message identifer followed by
			 the data and concluded with an End message 
			 identifier

Message Format:
[START CODE] [MESSAGE LENGTH] [MESSAGE TYPE] [DATA] [END CODE]

	* START CODE (2 bytes): beginning of message identifier

	* MESSAGE LENGTH (1 byte): is the number of bytes in the 
		data section including: message type and doubled bytes.
		Message length is limited to 255 bytes (largest number
		storable in a uint8_t)

	* MESSAGE TYPE (1 byte): identifies what action
		was taken on the IOS Application side. See Table
		of Codes below for more detail 

	* DATA (1 - 255 bytes): 

	* END CODE (2 bytes): end of message identifier

Table of Codes:
	Type			Code (Hex)		(Dec) 		
- Start Message: 	0x03 0x0A		3  10
- End Message:		0x0A 0x03		10 4
- Touch Event:		0x00			0
- Fade Timer:		0x01			1
- Brightness:		0x02			2
- Color:			0x03			3
- Preset Selected:	0x04			4
- Update Default: 	0x05			5

Limitations to Keep in Mind
	* Serial Buffer default size is 64 Bytes

Issues I need to address
	* How to tell if the messages are too long

Example Messages:
	Touch: [10, 3, 2, 0, 55, 10, 4]
		Start Code: 3, 10
		Message Length: 2
		Message Type: 0
			*Pixel Number: 55
		End Code: 10, 4

	Set Fade: [3, 10, 2, 1, 66, 10, 4]
		Start Code: 3, 10
		Message Length: 2
		Message Type: 1
			*Fade: 66
		End Code: 10, 4

	Set Brightness: [3, 10, 2, 2, 255, 10, 4]
		Start Code: 3, 10
		Message Length: 2
		Message Type: 2
			*Brightness: 255
		End Code: 10, 4

	Set Color: [3, 10, 4, 3, 55, 155, 255, 10, 4]
		Start Code: 3, 10
		Message Length: 4
		Message Type: 3
			*Red: 55
			*Green: 155
			*Blue: 255
		End Code: 10, 4

	Preset Selected: [3, 10, 2, 4, 7, 10, 4]
		Start Code: 3, 10
		Message Length: 2
		Message Type: 4
			*Preset: 7
		End Code: 10, 4

	// Update default pending research
*/

/* --------------------------------------------------*/
/* 						 							 */
/* --------------------------------------------------*/	
#ifndef _COMS_STATION_H_
#define _COMS_STATION_H_

/* --------------------------------------------------*/
/* 			Includes	 							 */
/* --------------------------------------------------*/	
#include <Arduino.h>

/* --------------------------------------------------*/
/* 			Decoded Message Data Index			 	 */
/* --------------------------------------------------*/
// These indexes indentify the array position of relavant
//  data in the decoded/destuffed arrays
#define INDEX_TOUCH_PIXEL 2
#define INDEX_FADE 2
#define INDEX_BRIGHTNESS 2
#define INDEX_RED 2
#define INDEX_GREEN 3
#define INDEX_BLUE 4
#define INDEX_PRESET 2

/* --------------------------------------------------*/
/* 			Setting Up Message Codes/Identifiers	 */
/* --------------------------------------------------*/
#define NUM_TRANSMISSION_INFO_BYTES 5 

static const uint8_t MIN_MESS_LENGTH = 6; // No data bit
static const uint8_t MAX_MESS_LENGTH = 255;

static const uint8_t START_MESSAGE_CODE_BYTE_1 = 3;
static const uint8_t START_MESSAGE_CODE_BYTE_2 = 10;

static const uint8_t END_MESSAGE_CODE_BYTE_1 = 10;
static const uint8_t END_MESSAGE_CODE_BYTE_2 = 4;

typedef enum errorType {
	/* Message Integrity Errors */
	messageMissingStartCodes 		= 0,
	messageTooShortError 			= 1,
	messageTooLongError 			= 2,
	messageLengthMismatch			= 3,
	messageMissingEndCodes			= 4,
	transmittedMessageLengthInvalid = 5,
	messageDataCorrupted			= 6,
	messageGoodToBeDecoded			= 7,

	/* Bit stuffing errors */
	missingStuffedByte				= 8,

	/* function input errors */
	returnMessageLengthError		= 9,

	defaultError,
	errorTypeCount
} errorType;

typedef enum messageType {
	touchEvent 				= 0,
	fadeTimerUpdateEvent 	= 1,	
	brightnessUpdateEvent 	= 2,
	colorUpdateEvent 		= 3,
	presetUpdateEvent		= 4,
	defaultUpdateEvent		= 5,
	messageTypeCount
} messageType;


/* --------------------------------------------------*/
/* 			Class ComsStation Declaration		 	 */
/* --------------------------------------------------*/	
class ComsStation {
	
public:
	ComsStation();
	~ComsStation();

	/*
	Description:
				Takes a message that complies with the Message Format listed above and
				decodes it stripping away transmission start/end bits, the message length
				bit and all stuffed bits. It copies the decoded message to retMessage
				and puts the number of data bits in the array at index 1.
	Precondition: 
				Message complies with Message Format outlined above
	Postcondition

	@param: message 	- complete transmitted message (see Message Format above)
	@param: length 		- length of message array NOTE: not the same as Message Length
						which is transmitted in the message.
	@param: retMessage 	- decoded and destuffed message. First index of the array
						contains the number of valid message bytes in the array
	@param: retLength 	- length of retMessage NOTE: if there were stuffed bits in 
						the message, then this will have empty bits at the end
	*/
	errorType decode(uint8_t message[], uint8_t length, uint8_t retMessage[], uint8_t retLength);

	/*
	Checks for the following:
			- Start and Stop codes
			- Message Length
	Does not check for missing stuffed bytes
	*/
	errorType messageReadyToBeDecoded(uint8_t message[], uint8_t length);
private:

	uint8_t messageType;
	uint8_t messageLength;
	uint8_t* messageData;
};

#endif

