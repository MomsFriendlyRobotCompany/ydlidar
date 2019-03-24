
#include "CYdLidar.h"
#include <iostream>
#include <string>
#include <memory>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace ydlidar;
CYdLidar laser;


int main(int argc, char * argv[])
{
    printf("__   ______  _     ___ ____    _    ____  \n");
    printf("\\ \\ / /  _ \\| |   |_ _|  _ \\  / \\  |  _ \\ \n");
    printf(" \\ V /| | | | |    | || | | |/ _ \\ | |_) | \n");
    printf("  | | | |_| | |___ | || |_| / ___ \\|  _ <  \n");
    printf("  |_| |____/|_____|___|____/_/   \\_\\_| \\_\\ \n");
    printf("\n");
    fflush(stdout);


    std::string port = "/dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0";
    int baudrate = 128000;
    // std::string intensity;
    ydlidar::init(argc, argv);
    int intensities  = 1;

    if(!ydlidar::ok()) {
        return 0;
    }

    laser.setSerialPort(port);
    laser.setSerialBaudrate(baudrate);
    laser.setIntensities(intensities);//intensity
    laser.setAutoReconnect(true);//hot plug
    laser.setMaxRange(16.0);
    laser.setMinRange(0.26);
    laser.setMaxAngle(180);
    laser.setMinAngle(-180);
    laser.setReversion(false);
    laser.setFixedResolution(false);
    laser.initialize();

    sleep(3);

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
                    printf("   %7.1f: %7.3f\n", angle, dis);

              }
              ydlidar::console.message("Scan received[%f]: %u ranges",(scan.self_time_stamp - start)/1e9, (unsigned int)scan.ranges.size());
          } else {
              ydlidar::console.warning("Failed to get Lidar Data");
          }

    }

    laser.turnOff();
    laser.disconnecting();
    return 0;


}
