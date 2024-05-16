#ifndef CURBY_H
#define CURBY_H
#include <stdio.h> 
#include <iostream>


// all of these will get setup during initialization
class Config{
    public:
        // function steepness
        float steep = 0.17;
        // function midpoint location;
        int mid = 55;
        // Lower Limit in RPM
        int minSpeed = 2300;
        // current speed
        int cSpeed = 2300;
        // Upper Limit in RPM
        int maxSpeed = 5500;
        // number of fans in the system
        int fanNum = 1;
        // fan path 1
        std::string fPath1 = "/sys/devices/platform/applesmc.768/fan1_min";
        // fan path 2
        std::string fPath2 = "/sys/devices/platform/applesmc.768/fan2_min";

};

int getDieTemp(){

    // opens file to read temps temp5 is TC0C, ie the die in applese
    FILE *f = fopen("/sys/devices/platform/applesmc.768/temp5_input", "r");

    // checks if file is empty
    if (f == NULL){
        // error here
        return -1;
    }

    // creating variables to pass
    int tempu;
    char tempstring[6];

    // reads the entire line, there's only one lol
    // and converts the char array to a number
    fgets(tempstring, 6, f);
    tempu = atoi(tempstring);

    // cleanup
    fclose(f);

    return (tempu/1000);
}

void setSpeed(int nSpeed, std::string speedPath){

    // opens file for reading and writing without making a new one, I think
    FILE *fSpeed = fopen(speedPath.c_str(), "r+");

    // converts int to char array ig
    std::string strSpeed = std::to_string(nSpeed);
    fputs(strSpeed.c_str(), fSpeed);

    // cleanup
    fclose(fSpeed);

}

//void setConfig(class config){
    
//}
#endif