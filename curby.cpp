// for using fopen
#include <stdio.h> 
// for testing
#include <iostream>
// for signal management
#include <signal.h>
#include <cmath>

class Config{
    public:
        // function steepness
        float steep = 0.17;
        // function midpoint location;
        int mid = 55;
        // Lower Limit in RPM
        int minSpeed = 2300;
        // Upper Limit in RPM
        int maxSpeed = 5500;
        // number of fans in the system
        int fanNum = 1;
        // DriverPath
        std::string dPath = "/sys/devices/platform/applesmc.768/";
        // config location
        std::string cLocation = "/etc/curby/curby.conf";
        // temp5 is TC0C, ie the die in applese
        std::string TC0C = "/sys/devices/platform/applesmc.768/temp5_input";
};

void filecheck(FILE *checkme, std::string fLocation){
    // checks if file is empty
    if (checkme == NULL){
        std::clog << "Error Opening " << fLocation << ", does it exist?" << std::endl;
        exit(1);
    }
}

void sighandler(int s){
    
    std::clog << "Exited with " << s << " code."; // output signal to log
    exit(1);

}

// takes Upper limit, Lower limit, the "k"urviture, and midpoint
// f(x) = ((U-L)/(1+e^(-k*(x-xsub0)))+L
int sigmoid(int x, int upperLimit, int lowerLimit, float kurve, int midpoint){
    
    return (((upperLimit - lowerLimit) / (1+(exp(-1*(kurve)*(x-midpoint)))))+lowerLimit);
    
}  

int getDieTemp(std::string temp5){

    // opens file to read temps temp5 is TC0C, ie the die in applese
    FILE *f = fopen(temp5.c_str(), "r");
    filecheck(f, temp5); // checks if file is valid

    // creating variable(s) to pass to fgets()
    char tempstring[6];

    // reads the entire line, there's only one lol
    fgets(tempstring, 6, f);

    // cleanup
    fclose(f);

    return (atoi(tempstring)/1000);
}

void setSpeed(int nSpeed, std::string speedPath, int iterator){

    // opens file for reading and writing without making a new one, I think
    FILE *fSpeed = fopen((speedPath + "fan" + std::to_string(iterator) + "_min").c_str(), "r+");
    filecheck(fSpeed, (speedPath + "fan" + std::to_string(iterator) + "_min"));

    // converts int to char array ig
    //std::string strSpeed = std::to_string(nSpeed);
    fputs(std::to_string(nSpeed).c_str(), fSpeed);

    // cleanup
    fclose(fSpeed);

}

void setConfig(Config conf){
    // config in /etc/curby/curby.conf
    // opens .conf to pull user data
    FILE *fonfig = fopen(conf.cLocation.c_str(), "r");

    fclose(fonfig);
}

int main() {

    // sets up signal handlers incase of process death via outside means
    signal(SIGINT, sighandler);  // sigint ie ctrl + c
    signal(SIGTERM, sighandler); // sigterm ie systemd stop or pkill

    // initialize the class
    Config Conf;
    
    //setConfig(Conf);


    // read temp1_input for die temp
    while (true){
    	
        // echo's speed value to driver fanX_min to change speed
        for (int i = 1; i <= Conf.fanNum; i++){
            setSpeed(sigmoid(getDieTemp(Conf.TC0C), Conf.maxSpeed, Conf.minSpeed, Conf.steep, Conf.mid), Conf.dPath, i);
        }
        //testing
        //std::cout << "Current RPM: " << Conf.cSpeed << ", DieTemp: " << getDieTemp() << "\r";
    }

        return 0;
}
