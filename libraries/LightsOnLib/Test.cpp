// Strictly for testing Functionality

// When compiling for Arduino just leave Testing as False
// false will prevent the test suit from compiling
#define TESTING false

#if TESTING
#include <cstdint>
#include <iostream>

#include "ComsStation.h"

void callDecode(uint8_t arr[], uint8_t arrLen);
ComsStation comSatOnline;

/* TOUCH EVENT Example Message
	Start Message Code: 3 10
	Message Length: 6 bytes
	Message Type Identifier: 0 -> Touch Event
	Message Data: Pixel # = 45
	End Message Code: 10, 3
*/
uint8_t m1[] = {3, 10, 2, 0, 45, 10, 3};
uint8_t m1_length = 2;
uint8_t m1_type = 0;

/* TOUCH EVENT Example Message with STUFFED BITS
	Start Message Code: 3 10
	Message Length: 6 bytes
	Message Type Identifier: 0 -> Touch Event
	Message Data: Pixel # = 100
	End Message Code: 10, 3
*/
uint8_t m2[] = {3, 10, 3, 0, 100, 10, 3};
uint8_t m2_length = 2;
uint8_t m2_type = 0;

/* TOUCH EVENT Example Message
	Start Message Code: 3 10
	Message Length: 11 bytes
	Message Type Identifier: 0 -> Touch Event
	Message Data: Pixel # = 10
	End Message Code: 10, 3
*/
uint8_t m10[] = {3, 10, 3, 0, 10, 10, 10, 3};
uint8_t m10_length = 3;
uint8_t m10_type = 0;

/* 
ERROR: Empty 
error cose: 1 -> messageTooShortError
*/
uint8_t m3[] = {};
uint8_t m3_length = 0;
uint8_t m3_type = 0;

/* 
ERROR: Message missing byte (simulate transmission loss)
error code: 5 -> transmittedMessageLengthInvalid
*/
uint8_t m4[] = {3, 10, 9, 0, 10, 10, 10, 155, 3, 200, 10, 3};
uint8_t m4_length = 7;
uint8_t m4_type = 0;

/* 
ERROR: Missing starting Code byte 1
*/
uint8_t m5[] = {4, 10, 6, 0, 45, 55, 155, 255, 200, 10, 3};
uint8_t m5_length = 6;
uint8_t m5_type = 0;

/* 
ERROR: Missing Starting cody byte 2
*/
uint8_t m6[] = {3, 8, 6, 0, 45, 55, 155, 255, 200, 10, 3};
uint8_t m6_length = 6;
uint8_t m6_type = 0;

/* 
ERROR: Missing Ending cody byte 1
*/
uint8_t m7[] = {3, 10, 6, 0, 45, 55, 155, 255, 200, 3, 3};
uint8_t m7_length = 6;
uint8_t m7_type = 0;

/* 
ERROR: Missing Ending cody byte 2
*/
uint8_t m8[] = {3, 10, 6, 0, 45, 55, 155, 255, 200, 10, 10};
uint8_t m8_length = 6;
uint8_t m8_type = 0;

/* 
ERROR: Missing stuffed byte
*/
uint8_t m9[] = {3, 10, 8, 0, 10, 10, 10, 155, 3, 5, 200, 10, 3};
uint8_t m9_length = 8;
uint8_t m9_type = 0;



int main(){
	
	printf("=============================\n");
	printf("Testing Good Messages: RETURN 7\n");
	printf("Message Length: %d, return message: %d\n", m1_length, comSatOnline.messageReadyToBeDecoded(m1, m1_length+5));
	printf("Message Length: %d, return message: %d\n", m2_length, comSatOnline.messageReadyToBeDecoded(m2, m2_length+5));
	printf("=============================\n");
	printf("Testing Bad Messages: RETURN Varies\n");
	printf("Message Length: %d, return message: %d\n", m3_length, comSatOnline.messageReadyToBeDecoded(m3, 0));
	printf("Message Length: %d, return message: %d\n", m4_length, comSatOnline.messageReadyToBeDecoded(m4, m4_length+5));
	printf("Message Length: %d, return message: %d\n", m5_length, comSatOnline.messageReadyToBeDecoded(m5, m5_length+5));
	printf("Message Length: %d, return message: %d\n", m6_length, comSatOnline.messageReadyToBeDecoded(m6, m6_length+5));
	printf("Message Length: %d, return message: %d\n", m7_length, comSatOnline.messageReadyToBeDecoded(m7, m7_length+5));
	printf("Message Length: %d, return message: %d\n", m8_length, comSatOnline.messageReadyToBeDecoded(m8, m8_length+5));
	printf("Message Length: %d, return message: %d\n", m9_length, comSatOnline.messageReadyToBeDecoded(m9, m9_length+5));

	callDecode(m1, m1_length);
	callDecode(m2, m2_length);
	callDecode(m10, m10_length);
	callDecode(m3, m3_length);

	return 0;
}

void callDecode(uint8_t arr[], uint8_t arrLen) {
	printf("=============================\n");

	printf("Decoding Message || \t");
	for(int i = 0; i < arrLen+5; i++){
		printf("%d ", arr[i]);	
	}

	uint8_t arghLen = arrLen + 1;
	uint8_t argh[arghLen];
	for(int i = 0; i < arghLen; i++){
		argh[i] = 0;
	}
	errorType error = comSatOnline.decode(arr, arrLen + 5, argh, arghLen);

	if (error != messageGoodToBeDecoded){
		printf("\nError: %d || \t\t", error);
	} else {
		printf("\nretMessage || \t\t");
	}
	for(int i = 0; i < arghLen; i++){
		printf("%d ", argh[i]);
	}

	printf("\n=============================\n");
}

#endif

