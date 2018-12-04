//
//  Contact.cpp
//  FinalProject
//
//  Created by Joshua Connolly on 11/28/18.
//  Copyright © 2018 Joshua Connolly. All rights reserved.
//

#include "Contact.hpp"
#include <iostream>

Contact::Contact()
{
    name = 0;
    UUID = 0;
}
Contact::Contact(unsigned char* givenUUID, char const* givenName)
{
    UUID = givenUUID;
    name = givenName;
}
Contact::Contact(unsigned char* givenUUID, char givenName)
{
    UUID = givenUUID;
    name = &givenName;
}
void Contact::setUUID(unsigned char* givenUUID)
{
    UUID = givenUUID;
}
void Contact::setName(char const* givenName)
{
    name = givenName;
}
void Contact::setName(char givenName)
{
    name = &givenName;
}
unsigned char* Contact::getUUID()
{
    return UUID;
}
char* Contact::getName()
{
    return (char*)name;
}
