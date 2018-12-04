//
//  Contact.hpp
//  FinalProject
//
//  Created by Joshua Connolly on 11/28/18.
//  Copyright © 2018 Joshua Connolly. All rights reserved.
//

#ifndef Contact_hpp
#define Contact_hpp

class Contact {
public:
    Contact();
    Contact(unsigned char* givenUUID, char const* givenName);
    Contact(unsigned char* givenUUID, char givenName);
    void setUUID(unsigned char* givenUUID);
    void setName(char const* givenName);
    void setName(char givenName);
    unsigned char* getUUID();
    char* getName();
    // Add as you see fit
    
private:
    char const* name;
    unsigned char* UUID;
};

#endif /* Contact_hpp */
