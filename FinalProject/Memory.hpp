//
//  Memory.hpp
//  FinalProject
//
//  Created by Joshua Connolly on 11/28/18.
//  Copyright © 2018 Joshua Connolly. All rights reserved.
//

#ifndef Memory_hpp
#define Memory_hpp

#include "Contact.hpp"
#include "Message.hpp"

class Memory {
public:
    Memory();
    Memory(Contact node);
    unsigned char* getNodeUUID();
    char* getNodeName();
    unsigned short getNumberContacts();
    unsigned short getNumberMessages();
    Contact getContact(unsigned short index);
    Message getMessage(unsigned short index);
    bool saveContact(Contact contact);
    void saveMessage(Message message);
    void saveNodeInformation(Contact contact);
    // Add as you see fit
    
protected:
    bool hasSchema();
    void setSchema();
    void clearMessages();
    void clearContacts();
    unsigned short getMessagePointerOffset();
    // Add as you see fit
    
private:
    const unsigned short MAX_CONTACTS = 10;
    const unsigned short MAX_MESSAGES = 20;
    Contact nodeContact;
    unsigned short numOfContacts = 0;
    unsigned short numOfMessages = 0;
    unsigned short offset;
    
};

#endif /* Memory_hpp */
