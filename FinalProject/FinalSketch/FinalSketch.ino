#include <avr/eeprom.h>
#include <Entropy.h>
#include "RF24.h"
#include <LiquidCrystal.h>
#include "Message.hpp"
#include "Memory.hpp"
#include "Contact.hpp"

class LCDKeypad : public LiquidCrystal
{
  public:
  LCDKeypad(int rs, int en, int d4, int d5, int d6, int d7) : LiquidCrystal(rs, en, d4, d5, d6, d7){}
  Button getButtonPress()
  {
    //Implement button debouncing
    /*
    The Arduino reads the value at the pin and provides a number ranging from 0 - 1023 corresponding to 
    the input voltage. Since we calculated the voltage at pin A0 to be 0.7081 V we can proceed to map this 
    value to reflect the Arduino's 10-bit ADC resolution. We know that 5V is represented by the decimal value 
    1023 and that 0V is represented by 0. We can now map the value accordingly using the Rule of Three. 
    We find out that the value at the pin read by the Arduino is around 144.
    */
  }
  private:
  typedef enum {LEFT, RIGHT, UP, DOWN, SELECT, NONE} Button;
}

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

typedef enum {SETUP, MENU, ABOUT_ME, NEW_CONTACT, MESSAGES, MESSAGE_OPEN, CONTACTS, NEW_MESSAGE, MESSAGE_RECEIVED} SystemState;
SystemState state = Menu;

Eeprom myEeprom;
unsigned char initFlag[] = {0xc0, 0xFF, 0xEE};
unsigned char contactListFlag[] = {0xFA, 0xCE};
unsigned char messageListFlag[] = {0xCA, 0x11};

void setup() {
  // put your setup code here, to run once:
//  unsigned char eepromContact;
//  for(int i = 0; i < 15; i++)
//  {
//    eepromContact += myEeprom.read(3+i) << i;
//  }
//  Contact checkContact = reintepret_cast<Contact>(eepromContact);
//
//  //CHECK TO SEE IF THE MEMORY IN 3-17 IS A CONTACT
//  //IF IT IS A CONTACT, THEN IGNORE THE REST OF SETUP
//
//  /*--------------------------------------------------
//                    FIX ME
//  
//  --------------------------------------------------*/
//  
//  for(int i = 0; i < 3; i++)
//  {
//    myEeprom.write(i, initFlag[i]);
//  }
//
//  //Create a screen for the user to enter name
//
//  String myName;
//  
//  Contact myNode(Entropy.random(40), "Dumpsterfire");
//  unsigned char* pContact = reinterpret_cast<char*>(&myNode); 
//  for(int i = 0; i < 15; i++)
//  {
//    myEeprom.write(3+i, *(pContact+i));
//  }
//  for(int i = 0; i < 2; i++)
//  {
//    myEeprom.write(18+i, contactListFlag[i]);
//  }
//  myEeprom.write(20, 0);
//  for(int i = 0; i < 3
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(state)
  {
    case SETUP:
    {
       /*
        Setup: First time boot. User enters his name and a UUID gets generated. 
        User information is stored in the EEPROM. Subsequent boots skip this state and start in the Menu state.
        
        SELECT: Saves the user's name and goes to the main menu.
        LEFT: Erases the last character and moves cursor to the left.
        RIGHT: Confirms character moves cursor to right.
        UP: Scrolls letter
        DOWN: Scrolls letter
  */
      state = MENU;
      break;
    }
    case MENU:
    {
      /*
      Displays the menu options. There are blinking arrows on the ends of the second row. 
      These arrows indicate that the user can press the Left or Right buttons to scroll to the next option. 
      The options wrap around once all have been displayed. The arrows complete a blink cycle in a second.
  
      SELECT: Goes to the selected option. The following is a list of valid menu options.
        Contacts: Goes to Contacts.
        Messages: Goes to Messages.
        N. Contact: Goes to New Contact.
        About Me: Goes to About Me.
      LEFT: Scrolls options.
      RIGHT: Scrolls options.
      UP: None
      DOWN: None
      */
      break;
    }
    case ABOUT_ME:
    {
      /*
      About Me: Shows the user's name and UUID.

      SELECT: None
      LEFT: None
      RIGHT: None
      UP: Goes back to main menu.
      DOWN: None
      */
      break;
    }
    case NEW_CONTACT:
    {
      /*
      Validates whether or not there is space for a new contact. 
      If there is, it transitions to the New Contact Name state. 
      Otherwise, it transitions to the List Full state.


      New Contact Name: Screen for new contact name input. A marker on the top right of the screen determines whether 
      this is the first or second screen in the process of adding a contact. The first screen consists 
      of inputting the new contact's name while the second screen consists of inputting the new contact's UUID.

      SELECT: Saves name and goes to the New Contact UUID screen.
      LEFT: Erases the last character and moves cursor to the left.
      RIGHT: Confirms character and moves cursor to the right.
      UP: Scrolls letter.
      DOWN: Scrolls letter.


      New Contact UUID: Screen for new contact name input. A marker on the top right of the screen determines that 
      this is the second screen in the process of adding a new contact.

      SELECT: Saves UUID and goes to the New Contact Added screen. Saves contact to the EEPROM.
      LEFT: Erases the last character and moves cursor to the left.
      RIGHT: Confirms character and moves cursor to the right.
      UP: Scrolls letter.
      DOWN: Scrolls letter.


      Contact Added: Informative screen that let's the user know that the contact was successfully added. 
      Times out in two seconds returning back to the main menu.

      SELECT: None
      LEFT: None
      RIGHT: None
      UP: Goes back to main menu.
      DOWN: None
      Time out: Goes back to main menu in two seconds. The delay should be non-blocking.


      List Full: Informative screen that let's the user know that there is no space for a new contact. 
      Times out in two seconds returning back to the main menu.

      SELECT: None
      LEFT: None
      RIGHT: None
      UP: Goes back to main menu.
      DOWN: None
      Time out: Goes back to main menu in two seconds. The delay should be non-blocking.

      */
      break;
    }
    case MESSAGES:
    {
      /*
      Displays the messages stored in the device. A list of sent and received messages. 
      A marker on the top right of the screen determines whether the message was sent or received.

      SELECT: Open the selected option.
      LEFT: Scroll messages.
      RIGHT: Scroll messages.
      UP: Goes back to the previous screen.
      DOWN: None
      */
      break;
    }
    case MESSAGE_OPEN:
    {
      /*
      Message Open: Displays a message that has been saved in the device. Displays whether the message was sent or received, the user, and the message.

      SELECT: None
      LEFT: None
      RIGHT: None
      UP: Goes back to the previous screen.
      DOWN: None
      */
      break;
    }
    case CONTACTS:
    {
      /*
      Displays the contacts stored in the device. There are blinking arrows on the ends of the second row. 
      These arrows indicate that the user can press the Left or Right buttons to scroll to the next option. 
      The options wrap around once all have been displayed. The arrows complete a blink cycle in a second.

      SELECT: Compose message for selected contact.
      LEFT: Scrolls contact options.
      RIGHT: Scrolls contact options.
      UP: Goes back to the previous screen.
      DOWN: None
      */
      
    }
    case NEW_MESSAGE:
    {
      /*
      Message New: Displays the user name to who we are sending the message to. 
      It also allows constructing a morse string to be sent.

      SELECT: Attempts to send the message.
      LEFT: Write a dot.
      RIGHT: Write a dash.
      UP: Goes back to the previous screen.
      DOWN: Erases one character.


      Message Sent: Informative screen that let's the user know that the message was sent successfully. Play a tone through the buzzer indicating that a message was sent. Times out in two seconds returning back to the main menu.

      SELECT: None
      LEFT: None
      RIGHT: None
      UP: Goes back to main menu
      DOWN: None
      Time out: Goes back to main menu in two seconds. The delay should be non-blocking.


      Message Failed: Informative screen that let's the user know that the message could not be sent. Play a tone through the buzzer indicating that sending the failed. Times out in two seconds returning back to the main menu.

      SELECT: None
      LEFT: None
      RIGHT: None
      UP: Goes back to main menu
      DOWN: None
      Time out: Goes back to main menu in two seconds. The delay should be non-blocking.
      */
      break;
    }
    case MESSAGE_RECEIVED:
    {
      /*
      Message Received: Informative screen that let's the user know that a new message has been received. Times out in two seconds returning back to the previous screen. Play a tone through the buzzer indicating that a message was received.

      SELECT: None
      LEFT: None
      RIGHT: None
      UP: Goes back to the previous screen.
      DOWN: None
      Time out: Goes back to the previous screen in two seconds. The delay should be non-blocking.
      */
      break;
    }
  }
}
