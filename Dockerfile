# Use an official Ubuntu base image
FROM ubuntu:20.04

# Set the maintainer label
LABEL maintainer="dillenburg.gabriel@gmail.com"

# Set a non-interactive frontend (to avoid some prompts)
ARG DEBIAN_FRONTEND=noninteractive

# Update and install necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    libopenblas-dev \
    liblapacke-dev \
    libnuma-dev \
    software-properties-common \
    wget \
    git

# Add the Ubuntu Toolchain Test PPA (which provides newer versions of GCC)
RUN add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y gcc-13 g++-13

# Set GCC 13 as the default gcc and g++
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 60 --slave /usr/bin/g++ g++ /usr/bin/g++-13

# Install additional libraries for BLAS
RUN apt-get install -y libopenblas-base liblapack3

# Clone the KASTORS benchmark repository (adjust branch as needed)
RUN git clone -b general-tests https://github.com/UFRGS-PPGC/kastors-benchmark.git /workspace

# Set the working directory to the repository directory
WORKDIR /workspace

# Set environment variables for dynamic library path
ENV LD_LIBRARY_PATH=/usr/lib:/usr/local/lib:${LD_LIBRARY_PATH}

# Default command to execute when starting the container
CMD ["/bin/bash"]
