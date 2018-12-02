//
//  Message.cpp
//  FinalProject
//
//  Created by Joshua Connolly on 11/28/18.
//  Copyright © 2018 Joshua Connolly. All rights reserved.
//

#include "Message.hpp"
#include <iostream>

//Attempt at mapping all the letters
unsigned short morse[] = {01, 1000, 1010, 100, 0, 0010, 110, 0000, 00, 0111, 101, 0100, 11, 10, 111, 0110, 1101, 010, 000, 1, 001, 0001, 011, 1001, 1011, 1100};

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
    int i = 0;
    unsigned short tempPayload = 0;
    while(message[i] != '\0')
    {
        //This doesnt work because not all letters are 4 bits. This will lead to leading zeros
        // where there shouldn't be, which will create a bad letter
        std::cout << morse[(int)message[i]-65] << std::endl;
        tempPayload |= morse[(int)message[i]-65] << 4;
        i++;
    }
    length = i;
    payload = tempPayload;
    //std::cout << tempPayload << std::endl;
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
//char* Message::getPayloadString()
//{
//    //Returns a C-string of the payload bytes
//}

