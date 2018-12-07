class Contact {
 public:
   Contact()
   {
    
   }
   Contact(unsigned char* givenUUID, char const* givenName)
   {
     setUUID(givenUUID);
     setName(givenName);
   }
   Contact(unsigned char* givenUUID, char givenName)
   {
     setUUID(givenUUID);
     setName(givenName);
   }
   void setUUID(unsigned char* givenUUID)
   {
     UUID = givenUUID;
   }
   void setName(char const* givenName)
   {
     Name = givenName;
   }
   void setName(char givenName)
   {
     Name = givenName;
   }
   unsigned char* getUUID()
   {
     return UUID;
   }
   String getName()
   {
     return Name;
   }
   // Add as you see fit

 private:
 String Name;
 unsigned char* UUID;

};
