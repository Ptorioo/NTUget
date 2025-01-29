FROM ubuntu:latest
WORKDIR /app
COPY . .
RUN apt update
RUN apt install -y \
    build-essential \
    cmake \
    libcurl4-openssl-dev
RUN make build BUILD_TYPE=Release
CMD ["./build/ntuget"]