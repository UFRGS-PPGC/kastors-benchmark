let
  pkgs = import (builtins.fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/archive/63dacb46bf939521bdc93981b4cbb7ecb58427a0.tar.gz";
    sha256 = "1lr1h35prqkd1mkmzriwlpvxcb34kmhc9dnr48gkm8hh089hifmx";
  }) {
    config = {
      allowUnfree = true;
      permittedInsecurePackages = [
        "python-2.7.18.8"
      ];
    };
  };
in
pkgs.mkShell {
  buildInputs = [
    pkgs.gcc
    pkgs.openblas
    pkgs.lapack
    pkgs.openmpi
    pkgs.llvmPackages.openmp
    pkgs.llvmPackages.stdenv.cc ## for headers
    pkgs.git
    pkgs.cmake
    pkgs.autoconf
    pkgs.automake
    pkgs.gnumake
    pkgs.numactl
    pkgs.python2
  ];

  shellHook = ''
    export OMP_NUM_THREADS=4
    export OMP_TOOL_LIBRARIES=$(pwd)/libompt.so
    ./scripts/setup.sh
  '';
}