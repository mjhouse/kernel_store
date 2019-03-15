# kernel_store

A kernel module that acts as a key/value store for userspace applications on a linux system, done in response to a programming challange from Star Lab. Don't use the `build.sh` file in the root dir- it has a hard-coded major number. Use the following commands to install the module (preferably on a VM):

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
