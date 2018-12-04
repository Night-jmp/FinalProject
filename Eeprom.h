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
