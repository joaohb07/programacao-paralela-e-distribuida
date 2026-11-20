#!/usr/bin/env bash
set -euo pipefail

if [ "$#" -lt 2 ]; then
  echo "Uso: $0 <boxsize> <gridsize>"
  exit 1
fi

IMAGE_NAME="voronoi-omp-runner"

# Faz o build da imagem (usa cache instantâneo se o código não mudou)
docker build -q -t "$IMAGE_NAME" . > /dev/null

# Sobe o container, repassa os argumentos, imprime a saída e destrói o container ao finalizar (--rm)
docker run --rm "$IMAGE_NAME" "$@"