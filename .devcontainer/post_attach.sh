#!/bin/bash
python3 -m venv /workspaces/local/.venv && \
    source /workspaces/local/.venv/bin/activate
    # pip install west
    # cd /workspaces/local
    # west update
    # west zephyr-export
    # # west packages pip --install && \
    # # west sdk install
    # # cd /opt/zephyr-sdk-0.17.4 && \
    # # ./setup.sh -t arm-zephyr-eabi -t xtensa-espressif_esp32_zephyr-elf -t xtensa-espressif_esp32s2_zephyr-elf -t xtensa-espressif_esp32s3_zephyr-elf -t x86_64-zephyr-elf -h -c

cd /workspaces
# west init -l local
cd local
west update
west zephyr-export