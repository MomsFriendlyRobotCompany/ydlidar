// Example with Gecko
// make sure to run a pygeckocore.py instance with key: dalek.
// MIT License Kevin J Walchko 2019
//

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "gecko.hpp"
#include "CYdLidar.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;
using namespace ydlidar;


int main(){

    json j = {
      {"pi", 3.141},
      {"happy", true},
      {"name", "Niels"},
      {"nothing", nullptr},
      {"answer", {
        {"everything", 42}
      }},
      {"list", {1, 0, 2}},
      {"object", {
        {"currency", "USD"},
        {"value", 42.99}
      }}
    };

    vector<double> pts = {1,2,3,4.5,6.789};
    j["pts"] = pts;
    j["bob"] = 3.456;

    string sj = j.dump();
    printf(">> %s\n", sj.c_str());


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

    Publisher *p = gecko::pubBindTCP("dalek", "lidar");
    if (p == nullptr) {
        printf("*** invalid publisher ***\n");
        return 1;
    }

    unsigned long long start = 0;

    while(gecko::ok()){
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
