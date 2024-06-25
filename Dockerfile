# Use a NixOS base image
FROM nixos/nix

# Install Nix and update channels
RUN nix-channel --add https://nixos.org/channels/nixpkgs-unstable nixpkgs && \
    nix-channel --update

# # Copy the Nix expression and setup script into the container
COPY . /workspace

# Set the working directory
WORKDIR /workspace

# Run nix-shell to install dependencies and run the setup script
RUN nix-shell /workspace/default.nix --run "sh /workspace/scripts/setup.sh"
