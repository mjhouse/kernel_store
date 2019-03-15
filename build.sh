#!/bin/bash

# DON'T USE THIS SCRIPT:
#   The major number to make the /dev/ file is hard-coded,
#   and probably won't work on a different system.
#   (mknod - line 12)

make clean &&
make module &&
make test &&
(sudo rm /dev/ks || true) &&
sudo mknod /dev/ks c 238 0 &&
sudo chmod 766 /dev/ks
