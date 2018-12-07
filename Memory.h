#include "Message.h"
#include "Contact.h"
#include <EEPROM.h>

class Memory {
  public:
    Memory()
    {
      setSchema();
      clearMessages();
      clearContacts();
    }
    Memory(Contact node)
    {
      setSchema();
      clearMessages();
      clearContacts();
      saveNodeInformation(node);
    }
    unsigned char* getNodeUUID()
    {
      int start = 3;
      unsigned char * UUID = new unsigned char[5];
      for (int i = 0; i < 5; i++)
        UUID[i] = EEPROM.read(start++);
      return UUID;
    }
    char* getNodeName()
    {
      int start = 8;
      char * nodename = new char[10];
      for (int i = 0; i < 10; i++)
        nodename[i] = EEPROM.read(start++);
      return nodename;
    }
    unsigned short getNumberContacts()
    {
      return EEPROM.read(20);
    }
    unsigned short getNumberMessages()
    {
      return EEPROM.read(173);
    }
    Contact getContact(unsigned short index)
    {
      int start = (index * 15) + 21;
      unsigned char * UUID = new unsigned char[5];
      char * contactname = new char[10];
      for (int i = 0; i < 5; i++)
        UUID[i] = EEPROM.read(start++);
      for (int i = 0; i < 10; i++)
        contactname[i] = EEPROM.read(start++);
      Contact tempcontact(UUID, contactname);
      return tempcontact;
    }
    Message getMessage(unsigned short index)
    {
      int start = (index * 13) + 174;
      unsigned char * senderUUID = new unsigned char[5];
      unsigned char * recieverUUID = new unsigned char[5];
      unsigned short payload = 0;
      unsigned char messagelength = 0;
      for (int i = 0; i < 5; i++)
        senderUUID[i] = EEPROM.read(start++);
      for (int i = 0; i < 5; i++)
        recieverUUID[i] = EEPROM.read(start++);
      payload |= EEPROM.read(start++);
      payload <<= 8;
      payload |= EEPROM.read(start++);
      messagelength = EEPROM.read(start++);
      Message tempmessage(senderUUID, recieverUUID, payload, messagelength);
      return tempmessage;
    }
    bool saveContact(Contact contact)
    {
      if (EEPROM.read(20) == MAX_CONTACTS)
        return false;
      else {
        int start = (EEPROM.read(20) * 15) + 21;
        for (int i = 0; i < 5; i++)
          EEPROM.write(start++, contact.getUUID()[i]);
        for (int i = 0; i < 10; i++)
          EEPROM.write(start++, contact.getName()[i]);
          
        EEPROM.write(20, EEPROM.read(20) + 1);
        return true;
      }
    }
    void saveMessage(Message message)
    {
      if (EEPROM.read(173) == MAX_MESSAGES)
        EEPROM.write(173, 0);
      else 
      {
        int start = (EEPROM.read(173) * 13) + 174;
        for (int i = 0; i < 5; i++)
          EEPROM.write(start++, message.getFrom()[i]);
        for (int i = 0; i < 5; i++)
          EEPROM.write(start++, message.getTo()[i]);
        EEPROM.write(start++, message.getPayload() >> 8);
        EEPROM.write(start++, (message.getPayload() | 0xFF));
        EEPROM.write(start++, message.getLength());
      }
        
    }
    void saveNodeInformation(Contact contact)
    {
        int start = 3;
        for (int i = 0; i < 5; i++)
          EEPROM.write(start++, contact.getUUID()[i]);
        for (int i = 0; i < 10; i++)
          EEPROM.write(start++, contact.getName()[i]);
      
    }
    // Add as you see fit

  protected:
    bool hasSchema()
    {
      if (EEPROM.read(0) == 0xC0 && EEPROM.read(1) == 0xFF && EEPROM.read(2) == 0xEE && EEPROM.read(18) == 0xFA 
          && EEPROM.read(19) == 0xCE && EEPROM.read(171) == 0xCA && EEPROM.read(172) == 0x11)
        return true;
      else return false;
    }
    void setSchema()
    {
      EEPROM.write(0, 0xC0);
      EEPROM.write(1, 0xFF);
      EEPROM.write(2, 0xEE);
      EEPROM.write(18, 0xFA);
      EEPROM.write(19, 0xCE);
      EEPROM.write(171, 0xCA);
      EEPROM.write(172, 0x11);  
    }
    void clearMessages()
    {
      for(int i = 173; i < 434; i++)
        EEPROM.write(i, 0);
        EEPROM.write(173, 0);
    }
    void clearContacts()
    {
      for (int i = 3; i < 20; i++)
        EEPROM.write(i, 0);
      for (int i = 20; i < 171; i++)
        EEPROM.write(i, 0);
      EEPROM.write(20, 0);
    }
    unsigned short getMessagePointerOffset()
    {
      return EEPROM.read(434);
    }
    // Add as you see fit

  private:

    const unsigned short MAX_CONTACTS = 10;
    const unsigned short MAX_MESSAGES = 20;
    // Add as you see fit

};
