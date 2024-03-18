| Supported Targets |  |
| ----------------- | ----- |
| RP2040 | (Waveshare RP2040-Zero) |

# RP2040 XP200 RTU Speed Controller

Speed controller interface for using an XP200 AC servo drive as a spindle in speed mode.

# Setup
- install zephyr sdk and project
- west init
- run build.bat with one of the supported boards, eg build.bat rpi_pico

# Running the Tests
- add ZEPHYR_SDK to your environment variables for your user, this is the path to the bin dir that contains the zephyr gdb for arm
- install qemu, add bin dir to path
- use the Debug Tests launch configuration
