# kernel_store

A kernel module that acts as a key/value store for userspace application on a linux system, done in response to a programming challange. Use the following commands to install the module:

```bash
# build and load the module
make module && make test
```

```bash
# make the /dev/ file and make it read/writable
sudo mknod /dev/ks c <MAJOR> 0 && sudo chmod 766 /dev/ks
```

```bash
# execute the demo and read the log
make demo && ./src/demo && dmesg
```

Reading the value doesn't seem to work, which means I'm probably missing something small. The `copy_to_user` macro just returns 6 bytes un-copied and continues without error.
