//
//  Message.cpp
//  FinalProject
//
//  Created by Joshua Connolly on 11/28/18.
//  Copyright © 2018 Joshua Connolly. All rights reserved.
//

#include "Message.hpp"

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
    //Convert c-string to payload bytes
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
    return senderUUID;
}
unsigned char* Message::getFrom()
{
    return receiverUUID;
}
unsigned short Message::getPayload()
{
    return payload;
}
char* Message::getPayloadString()
{
    //Returns a C-string of the payload bytes
}
