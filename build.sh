#!/bin/bash

make clean &&
make module &&
make test &&
(sudo rm /dev/ks || true) &&
sudo mknod /dev/ks c 238 0 &&
sudo chmod 766 /dev/ks
