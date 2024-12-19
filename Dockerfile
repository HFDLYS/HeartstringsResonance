FROM ubuntu:22.04


RUN apt-get update && apt-get install -y tzdata \
    && ln -fs /usr/share/zoneinfo/Asia/Shanghai /etc/localtime \
    && dpkg-reconfigure --frontend noninteractive tzdata

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    make gcc g++ llvm clang cmake\
    pkg-config libssl-dev zlib1g-dev\
    build-essential libgl1-mesa-dev gdb\
    libxkbcommon-dev\
    wget vim bash curl git\
    qt6-base-dev qt6-declarative-dev\
    qt6*\
    libassimp-dev\
    mysql-client libmysqlclient-dev\
    libqt6sql6-mysql\
    && apt-get clean

ENV QT_QPA_PLATFORM=offscreen

WORKDIR /workspace

COPY . /workspace

RUN mkdir build && cd build && cmake .. && make HeartstringsResonanceServer

CMD ["./build/HeartstringsResonanceServer"]