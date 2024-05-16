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
        // current speed, 0 on init
        int cSpeed = 0;
        // Upper Limit in RPM
        int maxSpeed = 5500;
        // number of fans in the system
        int fanNum = 1;
        // fan path 1
        std::string fPath1 = "/sys/devices/platform/applesmc.768/fan1_min";
        // fan path 2
        std::string fPath2 = "/sys/devices/platform/applesmc.768/fan2_min";
        // config location
        std::string location = "/etc/curby/curby.conf";
};

// takes Upper limit, Lower limit, the "k"urviture, and midpoint
// f(x) = ((U-L)/(1+e^(-k*(x-xsub0)))+L
int sigmoid(int x, int upperLimit, int lowerLimit, float kurve, int midpoint){
    
    return (((upperLimit - lowerLimit) / (1+(exp(-1*(kurve)*(x-midpoint)))))+lowerLimit);
    
}  

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
    //std::string strSpeed = std::to_string(nSpeed);
    fputs(std::to_string(nSpeed).c_str(), fSpeed);

    // cleanup
    fclose(fSpeed);

}

void setConfig(Config conf){
    // config in /etc/curby/curby.conf
    // opens .conf to pull user data
    FILE *fonfig = fopen(conf.location.c_str(), "r");

    fclose(fonfig);
}

void sighandler(int s){
    
    // TODO Add syslog.h processing ie journalctl logging
    // just leave
    exit(1);

}

int main() {

    // sets up signal handlers incase of process death
    // sigint ie ctrl + c
    signal(SIGINT, sighandler);
    // sigterm ie systemd stop or pkill
    signal(SIGTERM, sighandler);

    // initialize the class
    Config Conf;
    
    //setConfig(Conf);


    // read temp1_input for die temp
    while (true){
    	
        // echo's speed value to driver fanX_min to change speed
        setSpeed(sigmoid(getDieTemp(), Conf.maxSpeed, Conf.minSpeed, Conf.steep, Conf.mid), Conf.fPath1);

        if (Conf.fanNum == 2){
            setSpeed(sigmoid(getDieTemp(), Conf.maxSpeed, Conf.minSpeed, Conf.steep, Conf.mid), Conf.fPath2);
        }

        //testing
        //std::cout << "Current RPM: " << Conf.cSpeed << ", DieTemp: " << getDieTemp() << "\r";
    }

        return 0;
}
