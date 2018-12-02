//
//  main.cpp
//  FinalProject
//
//  Created by Joshua Connolly on 11/28/18.
//  Copyright © 2018 Joshua Connolly. All rights reserved.
//

#include <iostream>
#include "Message.hpp"

int main(int argc, const char * argv[]) {
    unsigned char to = '\x41';
    unsigned char from = '\x42';
    unsigned char* pTo = &to;
    unsigned char* pFrom = &from;
    unsigned short payload = 7;
    unsigned char length = 3;
    char const* mess = {"TEST"};
    
    Message message(pFrom, pTo, payload,length);
    //std::cout << (int)message.getLength() << std::endl;
    
    Message newMessage(pFrom, pTo, mess);
    //std::cout << newMessage.getPayload() << std::endl;

    return 0;
}
