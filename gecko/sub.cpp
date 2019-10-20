// // Example with Gecko
// // make sure to run a pygeckocore.py instance with key: dalek.
// // MIT License Kevin J Walchko 2019
// //
//
// #include <stdio.h>
// #include <stdint.h>
// #include <string>
// #include <vector>
// #include <gecko/gecko.hpp>
// #include <gecko/msgpack/msgs.hpp>
// #include <gecko/msgpack/msgpack_pub_sub.hpp>
// #include "CYdLidar.h"
//
// using namespace std;
// using namespace ydlidar;
//
//
// int main(){
//     printf("hello\n");
//     gecko::init();
//
//     // Subscriber *s = gecko::subConnectTCP("dalek", "lidar");
//     Subscriber *s = gecko::subConnectUDS("dalek", "lidar");
//     if (s == nullptr) {
//         printf("*** invalid subscriber ***\n");
//         return 1;
//     }
//
//     mpSubscriber<lidar_st> sub(s);
//
//     while(gecko::ok()){
//         lidar_st msg;
//         bool ret = sub.recv(msg);
//         msg.print();
//     }
//
//     return 0;
// }
