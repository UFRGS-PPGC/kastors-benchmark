# KASTORS Benchmark Docker Environment

This README provides instructions on how to set up and use the KASTORS benchmarks within a Docker container. This environment ensures compatibility and consistency across different systems.

## Prerequisites

- Docker must be installed on your system. You can download Docker Desktop for macOS or Windows from [Docker Hub](https://hub.docker.com/editions/community/docker-ce-desktop-mac/).

## Building the Docker Image

This Docker setup includes all necessary dependencies for the KASTORS benchmarks, including compilers and libraries for BLAS and LAPACK.

1. **Build the Docker image** using the following command:

   ```bash
   docker build -t kastors_env .
   ```

## Running the Docker Container

To start your Docker container with the KASTORS benchmarks environment, run:

```bash
docker run -it --rm kastors_env
```

## Configuring and Running Benchmarks

Once inside the Docker container, follow these steps to configure and run the benchmarks:

1. **Configure the Benchmarks**:

   Navigate to the `/workspace` directory and run the configure script:

   ```bash
   cd /workspace
   ./autogen.sh
   ./configure --with-blas=openblas
   ```

2. **Build the Benchmarks**:

   Compile the benchmarks using:

   ```bash
   make
   ```

3. **Test the Benchmarks** (Optional):

   Verify that everything is compiled correctly:

   ```bash
   make check
   ```

4. **Run the Benchmarks**:

   Execute the benchmarks with:

   ```bash
   make run
   ```
