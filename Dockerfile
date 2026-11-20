# Stage 1: Compilação dos binários C++ com suporte a OpenMP
FROM gcc:13-bookworm AS builder

WORKDIR /app
COPY . .

RUN g++ -O3 -fopenmp -static-libstdc++ -static-libgcc sequential.cpp -o sequential && \
    g++ -O3 -fopenmp -static-libstdc++ -static-libgcc parallel_voronoi_1.cpp -o parallel_voronoi_1 && \
    g++ -O3 -fopenmp -static-libstdc++ -static-libgcc parallel_voronoi_2.cpp -o parallel_voronoi_2

# Stage 2: Runtime leve contendo apenas os binários e a libgomp (OpenMP)
FROM debian:bookworm-slim

RUN apt-get update && \
    apt-get install -y --no-install-recommends libgomp1 && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /app/sequential /app/parallel_voronoi_1 /app/parallel_voronoi_2 ./

ENTRYPOINT ["/bin/sh", "-c", "./sequential \"$@\" && ./parallel_voronoi_1 \"$@\" && ./parallel_voronoi_2 \"$@\"", "--"]