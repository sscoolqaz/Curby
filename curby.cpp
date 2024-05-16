// for using fopen
#include <stdio.h> 
// for testing
#include <iostream>
// for exponential
#include <cmath>
// for signal management
#include <signal.h>
#include "curby.h"

void sighandler(int s){

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
    	
        // gets the correct speed based on a sigmoid function
	    // example with numbers:
	    // f(x) = ((5500-2300)/1+e^-.1(x-55))+2300
        // example with notation
        // f(x) = ((U-L)/(1+e^(-k*(x-xsub0)))+L

        // we're using exp instead of pow cause it uses exp anyways lmao
	     
        Conf.cSpeed = (
            (Conf.maxSpeed-Conf.minSpeed) / (1+(exp(-1*(Conf.steep)*(getDieTemp()-Conf.mid))))
            +Conf.minSpeed);

        // echo's speed value to driver fanX_min to change speed
        setSpeed(Conf.cSpeed, Conf.fPath1);
        if (Conf.fanNum == 2){
            setSpeed(Conf.cSpeed, Conf.fPath2);
        }

        //testing
        //std::cout << "Current RPM: " << Conf.cSpeed << ", DieTemp: " << getDieTemp() << "\r";
    }

        return 0;
}
