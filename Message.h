class Message {
  public:
    Message()
    {
      payload = 0;
      messagelength = 0;
    }
    Message(unsigned char* from, unsigned char* to, unsigned short payload, unsigned char messagelength)
    {
      setTo(to);
      setFrom(from);
      this->payload = payload;
      this->messagelength = messagelength;
      
    }
    Message(unsigned char* from, unsigned char* to, char const* message)
    {
      setTo(to);
      setFrom(from);
      payload = stringToPayload(message);
    }
    void setLength(unsigned char messagelength)
    {
      this->messagelength = messagelength;
    }
    void setTo(unsigned char* to)
    {
      for (int i = 0; i < 5; i++)
        receiverUUID[i] = to[i];
    }
    void setFrom(unsigned char* from)
    {
      for (int i = 0; i < 5; i++)
        senderUUID[i] = from[i];
    }
    void setPayload(unsigned short payload)
    {
      this->payload = payload;
    }
    unsigned char getLength()
    {
      return messagelength;
    }
    unsigned char* getTo()
    {
      return &receiverUUID[0];
    }
    unsigned char* getFrom()
    {
      return &senderUUID[0];
    }
    unsigned short getPayload()
    {
      return payload;
    }
    char* getPayloadString()
    {
      return payloadToString(payload, messagelength);
    }

  
    unsigned short stringToPayload(char const* message)
    {
       unsigned short tempPayload = 0;
      
       for (int i = 0; i < 16; i ++)
       {
          if (message[i] == '_')
          {
            tempPayload |= 1;
          }
          else if (message[i] == '.')
          {
            tempPayload |= 0;
          }
          else if (message[i] == ' '){
            i = 16;
            tempPayload <<= 1;
          }
          if (i < 15)
          tempPayload = tempPayload << 1;
       }
        return tempPayload;
      
    }
    protected:
    char* payloadToString(unsigned short payload, unsigned char messagelength)
    {
      char* tempPayloadString = new char[16];
      for (int i = 0; i < messagelength; i++)
        {
          if (payload & 1 == 1)
            tempPayloadString[15 - i] = '_';
          else tempPayloadString[15 - i] = '.';
          payload = payload >> 1;
        }
      
      return tempPayloadString;
    }

  private:
    unsigned char senderUUID[5] = {};
    unsigned char receiverUUID[5] = {};
    unsigned short payload;
    unsigned char messagelength;
};
