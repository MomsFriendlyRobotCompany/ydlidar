#include <stdio.h>
#include <stdint.h>
#include <string>
#include "gecko.hpp"
#include "CYdLidar.h"

using namespace std;
using namespace ydlidar;

int main(){
    printf("hello\n");
    gecko::init();

    string port = "/dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0";
    // string port = "/dev/ttyUSB0";
    int baudrate = 128000;

    CYdLidar laser;
    laser.setSerialPort(port);
    laser.setSerialBaudrate(baudrate);
    // laser.setIntensities(intensities);//intensity supported by S4B lidar
    laser.setAutoReconnect(true);//hot plug
    laser.setMaxRange(10.0);
    laser.setMinRange(0.12);
    laser.setMaxAngle(180);
    laser.setMinAngle(-180);
    laser.setScanFrequency(7);  // scanning frequency
    laser.setSampleRate(5);  // range frequency
    laser.setReversion(true);
    laser.setFixedResolution(false);
    laser.initialize();

    Publisher *p = gecko::pubBindTCP("local", "bob2");


    unsigned long long start = 0;

    while(ydlidar::ok()){
        bool hardError;
        LaserScan scan;

        if(laser.doProcessSimple(scan, hardError )){
            for(int i =0; i < scan.ranges.size(); i++ ){
                if (start == 0) {
                    start = scan.self_time_stamp;
                    printf("set start --------------------------------------\n");
                }
                float angle = (scan.config.min_angle + i*scan.config.ang_increment)*180.0/M_PI;
                float dis = scan.ranges[i];
                    // printf("   %7.3f: %7.3f\n", angle, dis);

            }
            ydlidar::console.message("Scan received[%f]: %u ranges",(scan.self_time_stamp - start)/1e9, (unsigned int)scan.ranges.size());
        }
        else {
            ydlidar::console.warning("Failed to get Lidar Data");
        }

    }

    laser.turnOff();
    laser.disconnecting();

    return 0;
}
