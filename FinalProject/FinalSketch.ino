#include <avr/eeprom.h>
#include "Message.hpp"

class Eeprom {
  public:
    Eeprom(){}
    
    //Set uiAddress to designed EEPROM address for reading
    byte read(unsigned int uiAddress)
    {
      //Wait for the last write fuction to finish
      while (EECR & (1 << EEPE));
      
      //Set EEAR to uiAddress
      EEAR = uiAddress;
      
      //Set EECR 1st bit to 1 to begin reading
      EECR |= (1 << EERE);
      
      //Return EEDR
      return EEDR;
    }
    
    //Set uiAddress to desired EEPROM address for writing
    void write(unsigned int uiAddress, unsigned char data)
    {
      //Wait for last write function to finish
      while (EECR & (1 << EEPE));
      
      //Set EEAR = uiAddress
      EEAR = uiAddress;
      
      //Set EEDR = ucData (Data register that stores what we want)
      EEDR = data;
      
      //Set EECR 3rd bit to 1 to master enable writing
      EECR |= (1 << EEMPE);
      
      //Set EERC 2nd bit to 1
      EECR |= (1 << EEPE);
    }
};

typedef enum {SETUP, MENU, ABOUT_ME, NEW_CONTACT, MESSAGES, MESSAGE_OPEN, CONTACTS, NEW_MESSAGE} SystemState;
SystemState state = Menu;

Eeprom myEeprom;
unsigned char initFlag[] = {0xc0, 0xFF, 0xEE};

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < 3; i++)
  {
    myEeprom.write(i, initFlag[i]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(state)
  {
    case SETUP:
    {
      //If first boot, run setup state
      break;
    }
    case MENU:
    {
      //General menu
      break;
    }
    case ABOUT_ME:
    {
      //If About Me selected
      break;
    }
    case NEW_CONTACT:
    {
      //If New Contact selcted
      //Check to see if list is full
      //If full, switch to menu state
      //Else, ask for name & UUID
      //Return to menu
      break;
    }
    case MESSAGES:
    {
      //Shows if new messages are availble
      break;
    }
    case MESSAGE_OPEN:
    {
      //If new, this state opens the message
      break;
    }
    case CONTACTS:
    {
      //Search existing contacts
      
    }
    case NEW_MESSAGE:
    {
      //Message New option
      //If message sent sucessfully, indicate and return to contacts
      //else, indicate failure and return to contacts
      break;
      break;
    }
  }
}
