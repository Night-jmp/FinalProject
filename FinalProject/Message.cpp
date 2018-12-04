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
    length = 0;
    
}

//Parameterized Constructor
Message::Message(unsigned char* from, unsigned char* to, unsigned short payload, unsigned char length)
{
    receiverUUID = to;
    senderUUID = from;
    this->payload = payload;
    this->length = length;
}

//Copy Constructor - Takes in C-string and converts it to payload bytes
Message::Message(unsigned char* from, unsigned char* to, char const* message)
{
    receiverUUID = to;
    senderUUID = from;
    
    payload = stringToPayload(message);
}

//Setters
void Message::setLength(unsigned char length)
{
    this->length = length;
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
    return length;
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
                length++;
            }
            else if(*(message + i) == '_')
            {
                tempPayload++;
                length++;
            }
        }
        return tempPayload;
}
char* Message::payloadToString(unsigned short payload, unsigned char length)
{
    char* tempPayloadString = new char[16];
    
    for(int i = 0; i < length; i++)
    {
        //ADD LOGIC HERE
    }
    return tempPayloadString;
}
char* Message::getPayloadString()
{
    //Returns a C-string of the payload bytes
    return payloadToString(payload, length);
   
}


