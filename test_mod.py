#!/usr/bin/env python2.7

import fcntl

def execute( name ):
    with open(name,'rw') as f:
        result = "KEY"
        e = fcntl.fcntl(f,fcntl.LOCK_EX,result)

        print(result) # should now be value

if __name__=='__main__':
    execute('/dev/kernel_store')
