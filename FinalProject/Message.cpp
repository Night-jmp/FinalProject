//
//  Message.cpp
//  FinalProject
//
//  Created by Joshua Connolly on 11/28/18.
//  Copyright © 2018 Joshua Connolly. All rights reserved.
//

#include "Message.hpp"
#include <iostream>
#include <cmath>

//Default Constructor - Initialized all bject variables to null values
Message::Message()
{
    senderUUID = nullptr;
    receiverUUID = nullptr;
    payload = 0;
    messagelength = 0;
    
}

//Parameterized Constructor
Message::Message(unsigned char* from, unsigned char* to, unsigned short payload, unsigned char messagelength)
{
    receiverUUID = to;
    senderUUID = from;
    this->payload = payload;
    this->messagelength = messagelength;
}

//Copy Constructor - Takes in C-string and converts it to payload bytes
Message::Message(unsigned char* from, unsigned char* to, char const* message)
{
    receiverUUID = to;
    senderUUID = from;
    
    payload = stringToPayload(message);
}

//Setters
void Message::setLength(unsigned char messagelength)
{
    this->messagelength = messagelength;
}
void Message::setTo(unsigned char* to)
{
    receiverUUID = to;
}
void Message::setFrom(unsigned char* from)
{
    senderUUID = from;
}
void Message::setPayload(unsigned short payload)
{
    this->payload = payload;
}

//Getters
unsigned char Message::getLength()
{
    return messagelength;
}
unsigned char* Message::getTo()
{
    return receiverUUID;
}
unsigned char* Message::getFrom()
{
    return senderUUID;
}
unsigned short Message::getPayload()
{
    return payload;
}
unsigned short Message::stringToPayload(char const* message)
{
        unsigned short tempPayload = 0;
    
        //Convert c-string to payload bytes
        for(int i = 0; *(message + i) != '\0'; i++)
        {
            tempPayload <<= 1;
            if(*(message + i) == '.')
            {
               messagelength++;
            }
            else if(*(message + i) == '_')
            {
                tempPayload++;
                messagelength++;
            }
        }
        return tempPayload;
}
char* payloadToString(unsigned short payload, unsigned char messagelength)
{
	char* tempPayloadString = new char[16];

	for (int i = 0; i < messagelength; i++)
	{
		if (payload & 1 == 1)
			tempPayloadString[15 - i] = '_';
		else tempPayloadString[15 - i] = '.';
		payload = payload >> 1;
	}
	return tempPayloadString;
}
char* Message::getPayloadString()
{
    //Returns a C-string of the payload bytes
    return payloadToString(payload, messagelength);
   
}


