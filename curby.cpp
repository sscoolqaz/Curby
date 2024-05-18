// for using fopen
#include <stdio.h> 
// for testing
#include <iostream>
// for signal management
#include <signal.h>
#include <cmath>
#include <unistd.h>

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

void setSpeed(int nSpeed, std::string speedPath, int fanNum){

    // run the loop here to prevent fan speed race hazard
    for (int i = 1; i <= fanNum; i++){
        // opens file for reading and writing without making a new one, I think
        FILE *fSpeed = fopen((speedPath + "fan" + std::to_string(i) + "_min").c_str(), "r+");
        filecheck(fSpeed, (speedPath + "fan" + std::to_string(i) + "_min"));

        // converts int to char array ig
        //std::string strSpeed = std::to_string(nSpeed);
        fputs(std::to_string(nSpeed).c_str(), fSpeed);

        // cleanup
        fclose(fSpeed);
    }

}

void setConfig(std::string confLoc){

    exit(0);
}

void configure(std::string confLoc){

    exit(0);
}

int main(int argc, char* argv[]) {

    // sets up signal handlers incase of process death via outside means
    signal(SIGINT, sighandler);  // sigint ie ctrl + c
    signal(SIGTERM, sighandler); // sigterm ie systemd stop or pkill

    // arguments
    int opt;
    // function steepness
    float steep = 0.1;
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
    
    while((opt = getopt(argc, argv, "if:lrx")) != 1) {
    
        switch(opt) {
            case 'c':
                setConfig(cLocation);
        }  
    }


    // read temp1_input for die temp
    while (true){
    	
        // based on a sigmoid function where x = the die temp set the speed of the fans
        setSpeed(sigmoid(getDieTemp(dPath+"temp5_input"), maxSpeed, minSpeed, steep, mid), dPath, fanNum);
        usleep(200000); // give the CPU some time off
        //testing
        //std::cout << "Current RPM: " << Conf.cSpeed << ", DieTemp: " << getDieTemp() << "\r";
    }

        return 0;
}
