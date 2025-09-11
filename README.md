| Supported Targets |  |
| ----------------- | ----- |
| ESP32-devkit-v1 |  |

# RP2040 XP200 RTU Speed Controller

Speed controller interface for using an XP200 AC servo drive as a spindle in speed mode.

# Setup
- install zephyr sdk and project
- west init
- run build.bat with one of the supported boards, eg build.bat rpi_pico

# Registers

Most of the memory layout of the XP200 is defined in src/modbus if that is useful to you in other projects