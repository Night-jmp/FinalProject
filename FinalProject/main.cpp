//
//  main.cpp
//  FinalProject
//
//  Created by Joshua Connolly on 11/28/18.
//  Copyright © 2018 Joshua Connolly. All rights reserved.
//

#include <iostream>
#include "Message.hpp"
#include "Contact.hpp"

int main(int argc, const char * argv[]) {
//    unsigned char to = '\x41';
//    unsigned char from = '\x42';
//    unsigned char* pTo = &to;
//    unsigned char* pFrom = &from;
//    unsigned short payload = 7;
//    unsigned char length = 3;
//    char const* mess = {"._.."};
//
//    Message message(pFrom, pTo, payload,length);
//    //std::cout << (int)message.getLength() << std::endl;
//
//    Message newMessage(pFrom, pTo, mess);
//    //std::cout << newMessage.getPayload() << std::endl;
//    std::cout << newMessage.getPayloadString() << std::endl;
    unsigned char* PUUID = nullptr;
    unsigned char UUID = 65;
    PUUID = &UUID;
    
    char name[5] = "hell";
    
    char nameChar = 'B';
    
    
    Contact contact(PUUID, nameChar);
    
    char* ptrToName = contact.getName();
//    for(int i = 0; ptrToName[i] != '\0'; i++)
//        std::cout << ptrToName[i];
    
    std::cout << *ptrToName << std::endl;

    unsigned char* ptrToUUID = contact.getUUID();
    std::cout << *ptrToUUID << std::endl;
    return 0;
}
