//
//  main.cpp
//  test
//
//  Created by Joshua Connolly on 12/6/18.
//  Copyright © 2018 Joshua Connolly. All rights reserved.
//

#include <iostream>

using namespace std;

int main(int argc, const char * argv[]) {
    
    unsigned char UUID[] = {0x11, 0x11, 0x11, 0x11, 0x11};
    
    uint64_t newUUID = 0;
    
    
    newUUID |= (uint64_t)UUID[0];
    
    for(int i = 1; i < 5; i++)
    {
        newUUID = newUUID << 8;
        newUUID |= (uint64_t)UUID[i];
    }
    
    cout << newUUID << endl;
    
    return 0;
}

