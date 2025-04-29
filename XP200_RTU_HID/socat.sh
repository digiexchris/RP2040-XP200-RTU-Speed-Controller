#!/bin/bash
socat -v -d -d pty,raw,echo=0,link=/tmp/ttyS0 pty,raw,echo=0,link=/tmp/ttyS1