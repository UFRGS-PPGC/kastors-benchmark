#!/bin/bash

# Diretório de trabalho
cd /workspace

# Baixar o arquivo minha_ferramenta_ompt.c diretamente do repositório
if [ ! -f "minha_ferramenta_ompt.c" ]; then
  wget https://gitlab.com/lnesi/companion-minicurso-openmp-tasks/-/raw/master/codigos/OMPT/minha_ferramenta_ompt.c -O minha_ferramenta_ompt.c
fi

# Compilar minha_ferramenta_ompt.c para gerar libompt.so
gcc -fopenmp -shared -fPIC minha_ferramenta_ompt.c -o libompt.so

# Executar os comandos de setup
./autogen.sh
./configure --with-blas=openblas
