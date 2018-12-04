//
//  Memory.cpp
//  FinalProject
//
//  Created by Joshua Connolly on 11/28/18.
//  Copyright © 2018 Joshua Connolly. All rights reserved.
//

#include "Memory.hpp"
#include "Message.hpp"
#include "Contact.hpp"
#include "Eeprom.h"

Memory::Memory()
{
    if(!hasSchema())
    {
        setSchema();
    }
}
Memory::Memory(Contact node)
{
    nodeContact = node;
}
unsigned char* Memory::getNodeUUID()
{
    return nodeContact.getUUID();
}
char* Memory::getNodeName()
{
    return nodeContact.getName();
}
unsigned short Memory::getNumberContacts()
{
    return numOfContacts;
}
unsigned short Memory::getNumberMessages()
{
    return numOfMessages;
}
//Contact Memory::getContact(unsigned short index)
//{
//
//}
//Message Memory::getMessage(unsigned short index)
//{
//
//}
bool Memory::saveContact(Contact contact)
{
    if(numOfContacts >= MAX_CONTACTS)
    {
        return false;
    }
    else
    {
        //SAVE CONTACT TO EEPROM
        return true;
    }
}
void Memory::saveMessage(Message message)
{
    //SAVE MESSAGE TO EEPROM
}
void Memory::saveNodeInformation(Contact contact)
{
    //SAVE NODE INFO TO EEPROM
}
bool Memory::hasSchema()
{
    //if flag set
        //return true
    //else
        return false;
}
void Memory::setSchema()
{
    //SET FLAG BYTES
}
void Memory::clearMessages()
{
    //CLEAR ALL MESSAGES FROM EEPROM
    //RESET COUNTER
}
void Memory::clearContacts()
{
    //CLEAR ALL CONTACTS FROM EEPROM
    //RESET COUNTER
}
unsigned short Memory::getMessagePointerOffset()
{
    return offset;
}
