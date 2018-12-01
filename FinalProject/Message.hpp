//
//  Message.hpp
//  FinalProject
//
//  Created by Joshua Connolly on 11/28/18.
//  Copyright © 2018 Joshua Connolly. All rights reserved.
//

#ifndef Message_hpp
#define Message_hpp

class Message {
public:
    Message();
    Message(unsigned char* from, unsigned char* to, unsigned short payload, unsigned char length);
    Message(unsigned char* from, unsigned char* to, char const* message);
    void setLength(unsigned char length);
    void setTo(unsigned char* to);
    void setFrom(unsigned char* from);
    void setPayload(unsigned short payload);
    unsigned char getLength();
    unsigned char* getTo();
    unsigned char* getFrom();
    unsigned short getPayload();
    char* getPayloadString();
    
protected:
    unsigned short stringToPayload(char const* message);
    char* payloadToString(unsigned short payload, unsigned char length);
    
private:
    unsigned char* senderUUID;
    unsigned char* receiverUUID;
    unsigned short payload;
    unsigned char length;
};

#endif /* Message_hpp */
