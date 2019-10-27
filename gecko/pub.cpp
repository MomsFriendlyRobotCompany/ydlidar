/************************************************
Example with Gecko
MIT License Kevin J Walchko 2019
----------------------------------------------
node -u file -t topic -s serial -v
*/

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <gecko/gecko.hpp>
#include <gecko/protobuf/msgs.pb.h>
#include <gecko/protobuf/helper_pb.h>
// #include <gecko/protobuf/gecko_pb.h>
#include "CYdLidar.h"

using namespace std;
using namespace ydlidar;
using namespace geckopb;


int main(){
    printf("hello\n");
    gecko::init();

    // string port = "/dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0";
    string port = "/dev/ttyUSB0";
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

    // Publisher *p = gecko::pubBindTCP("dalek", "lidar");
    // Publisher *p = gecko::pubBindUDS("dalek", "lidar", "/tmp/test-0");
    Publisher *p = new Publisher();
    p->bind(zmqUDS("/tmp/test-0"));

    if (p == nullptr) {
        printf("*** invalid publisher ***\n");
        return 1;
    }

    // mpPublisher<lidar_st> pub(p);
    Rate rate(5);
    unsigned long long start = 0;

    while(gecko::ok()){
        bool hardError;
        LaserScanData scan;
        LaserScan msg;

        if(laser.doProcessSimple(scan, hardError )){
            // lidar_st msg;
            // LaserScan msg;
            msg.clear_points();
            for(int i = 0; i < scan.ranges.size(); i++ ){
                float angle = (scan.config.min_angle + i*scan.config.ang_increment)*180.0/M_PI;
                float dis = scan.ranges[i]; // + 0.00001;  // if every 0 meters, pb will not send field, so add 0.01 mm
                // float inten = scan.intensities[i];
                printf("   %7.3f: %7.3f\n", angle, dis);
                // pt_t pt(angle, dis);
                // msg.data.push_back(pt);
                // LaserPt p;
                // p.set_angle(angle);
                // p.set_range(dis);
                // msg.add_range(dis);
                LaserPt *p = msg.add_points();
                p->set_angle(angle);
                p->set_range(dis);
                // p->set_intensity(inten);
            }
            zmq::message_t m = protobufPack<LaserScan>(msg);
            //
            // for(int i =0; i < scan.ranges.size(); i++ ){
            //     double angle = (scan.config.min_angle + i*scan.config.ang_increment)*180.0/M_PI;
            //     double dis = scan.ranges[i];
            //     // printf("   %7.3f: %7.3f\n", angle, dis);
            //     pt_t pt(angle, dis);
            //     msg.data.push_back(pt);
            // }
            // zmq::message_t m = msg.pack();
            p->publish(m);
        }
        else {
            printf("Failed to get lidar data\n");
        }

        rate.sleep();

    }
    delete p;
    laser.turnOff();
    laser.disconnecting();

    return 0;
}
