#include "ComsStation.h"

//#include <cstddef>

/* --------------------------------------------------*/
/* 			Public Methods						 	 */
/* --------------------------------------------------*/	

ComsStation::ComsStation() : messageType(0), messageLength(0), messageData(nullptr)
{
}

ComsStation::~ComsStation()
{
	// TODO
	if(messageData != nullptr){
		messageData = nullptr;
	}
}

/*
	Expects retMessage to be of length: length - NUM_TRANSMISSION_INFO_BYTES + 1 
	this will allow for a null byte to by inserted after all of the data.
	This is used because stuffed bytes scew the actual length of the data
*/
errorType ComsStation::decode(uint8_t message[], uint8_t length, uint8_t retMessage[], uint8_t retLength){

	errorType error;

	if (retLength == length - NUM_TRANSMISSION_INFO_BYTES + 1){

		error = messageReadyToBeDecoded(message, length);

		if (error == messageGoodToBeDecoded) {

			// Extract data from message and put it unstuffed into retLength
			messageLength = message[2];
			messageType = message[3];

			uint8_t retIndex = 1;
			for (uint8_t i = 3; i < length-2; i++){

				// Jump over stuffed bit as it is not relavant for data extraction, just transmission
				if (message[i] == END_MESSAGE_CODE_BYTE_1 && message[i+1] == END_MESSAGE_CODE_BYTE_1) {
					retMessage[retIndex] = message[i++];
				} else if (message[i] == END_MESSAGE_CODE_BYTE_2 && message[i+1] == END_MESSAGE_CODE_BYTE_2) {
					retMessage[retIndex] = message[i++];
				} else {
					retMessage[retIndex] = message[i];
				}
				retIndex += 1;
			}
			// number of data bytes is put in the first index of the array
			retMessage[0] = retIndex; 
		} 

	} else {

		error = returnMessageLengthError; 

	}
	
	return error;
}

errorType ComsStation::messageReadyToBeDecoded(uint8_t message[], uint8_t length){
	if(length < MIN_MESS_LENGTH) {

		return messageTooShortError;

	} else if(length > MAX_MESS_LENGTH){

		return messageTooLongError;

	} else if(message[0] != START_MESSAGE_CODE_BYTE_1 ||
	 	message[1] != START_MESSAGE_CODE_BYTE_2){

		return messageMissingStartCodes;

	} else if(message[length-2] != END_MESSAGE_CODE_BYTE_1 ||
	 	message[length-1] != END_MESSAGE_CODE_BYTE_2){ 

		return messageMissingEndCodes;

	} else if(message[2] != (length-5)) {

		// message[2] is the transmitted message length
		// length - 5 exludes starting, type and ending codes
		return transmittedMessageLengthInvalid;

	} else {

		for(uint8_t i = 4; i < length-2; i++){
			if(message[i] == END_MESSAGE_CODE_BYTE_1 && message[i+1] == END_MESSAGE_CODE_BYTE_1){
				i++;
			} else if (message[i] == END_MESSAGE_CODE_BYTE_1 && message[i+1] != END_MESSAGE_CODE_BYTE_1){
				// ERROR CASE: no stuffed ending byte 1
				return missingStuffedByte;
			}
		}

		return messageGoodToBeDecoded;
	}
}






