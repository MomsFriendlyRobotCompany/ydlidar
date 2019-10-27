// Example with Gecko
// make sure to run a pygeckocore.py instance with key: dalek.
// MIT License Kevin J Walchko 2019
//

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <gecko/gecko.hpp>
#include <gecko/protobuf/msgs.pb.h>
#include <gecko/protobuf/helper_pb.h>
#include "CYdLidar.h"

using namespace std;
using namespace ydlidar;
using namespace geckopb;


int main(){
    // printf("hello\n");
    gecko::init();

    // Subscriber *s = gecko::subConnectTCP("dalek", "lidar");
    // Subscriber *s = gecko::subConnectUDS("dalek", "lidar");
    Subscriber *s = new Subscriber();
    s->connect(zmqUDS("/tmp/test-0"));
    if (s == nullptr) {
        printf("*** invalid subscriber ***\n");
        return 1;
    }

    Rate rate(7);

    // mpSubscriber<lidar_st> sub(s);

    while(gecko::ok()){
        // lidar_st msg;
        // bool ret = sub.recv(msg);
        zmq::message_t m = s->recv_nb();
        if (m.size() > 0){
            LaserScan scan = protobufUnpack<LaserScan>(m);
            // cout << scan.DebugString() << "\n\n" << endl;
            for (int i=0; i < scan.points_size(); i++){
                LaserPt *p = scan.mutable_points(i);
                printf("   %7.3f: %7.3f\n", p->angle(), p->range());
            }
        }
        rate.sleep();
    }

    delete s;

    return 0;
}
