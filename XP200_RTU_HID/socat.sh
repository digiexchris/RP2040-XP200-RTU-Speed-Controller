#!/bin/bash

# used with the native-sim target and a modbus server simulator such as openmodsim. The nativesim client will connect to /tmp/ttyS0 and the modbus should will connect to /tmp/ttyS1.
socat -x -d -d pty,raw,echo=0,link=/tmp/ttyS0 pty,raw,echo=0,link=/tmp/ttyS1