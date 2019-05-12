#!/usr/bin/env python3

from pygecko.multiprocessing import geckopy
from pygecko.messages import lidar_st
from the_collector import BagIt
from the_collector import Pickle

if __name__ == '__main__':
    bag = BagIt(Pickle)
    geckopy.init_node()
    rate = geckopy.Rate(10)

    s = geckopy.subConnectTCP('dalek', 'lidar')

    if s is None:
        raise Exception("subscriber is None")

    # try:
    while not geckopy.is_shutdown():
        msg = s.recv_nb()
        if msg:
            # geckopy.loginfo("{}".format(msg))
            print('.', end='', flush=True)
            bag.push('lidar', msg)
        rate.sleep()

    # except KeyboardInterrupt:
    bag.write('lidar')

    print('sub bye ...')
