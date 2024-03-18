ARG VARIANT=ubuntu-22.04
FROM mcr.microsoft.com/devcontainers/cpp:${VARIANT}
LABEL Description="ELSF Build environment clang 14"

RUN apt-get update && apt-get -y --no-install-recommends install \
    build-essential \
    clang \
    clangd \
    lld \
    cmake \
    gdb \
    wget \
    git \
    ninja-build \
    python3 \
    llvm-14 llvm-14-tools curl clang-format-14 ca-certificates openssl ssh libc++-dev libc++abi-dev && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-14 100
RUN update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-14 100
RUN update-alternatives --install /usr/bin/llvm-profdata llvm-profdata /usr/bin/llvm-profdata-14 100
RUN echo 'vscode:vscode!' | chpasswd

# Configure SSHD.
# SSH login fix. Otherwise user is kicked off after login
#RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
#RUN mkdir /var/run/sshd
#RUN bash -c 'install -m755 <(printf "#!/bin/sh\nexit 0") /usr/sbin/policy-rc.d'
#RUN ex +'%s/^#\zeListenAddress/\1/g' -scwq /etc/ssh/sshd_config
#RUN ex +'%s/^#\zeHostKey .*ssh_host_.*_key/\1/g' -scwq /etc/ssh/sshd_config
#RUN RUNLEVEL=1 dpkg-reconfigure openssh-server
#RUN ssh-keygen -A -v
RUN update-rc.d ssh defaults

# Configure sudo.
RUN ex +"%s/^%sudo.*$/%sudo ALL=(ALL:ALL) NOPASSWD:ALL/g" -scwq! /etc/sudoers

EXPOSE 22
