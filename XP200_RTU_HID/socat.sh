#!/bin/bash
socat -d -d pty,raw,echo=0,link=/tmp/ttyS0 pty,raw,echo=0,link=/tmp/ttyS1