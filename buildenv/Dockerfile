FROM debian

RUN apt-get update  -y
RUN apt-get upgrade -y

RUN apt-get install binutils -y
RUN apt-get install mtools -y
RUN apt-get install make -y
RUN apt-get install nasm -y
RUN apt-get install gcc -y

RUN useradd -ms /bin/bash anon
USER anon

VOLUME /home/anon/ostk
WORKDIR /home/anon/ostk
