FROM debian

RUN apt-get update  -y
RUN apt-get upgrade -y
RUN apt-get install build-essential -y
RUN apt-get install mtools -y
RUN apt-get install nasm -y

VOLUME /root/ostk-env
WORKDIR /root/ostk-env
