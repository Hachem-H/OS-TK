FROM debian

RUN apt-get update  -y
RUN apt-get upgrade -y
RUN apt-get install build-essential -y
RUN apt-get install mtools -y

RUN groupadd -r os-tk
RUN useradd -mr -g os-tk os-tk
USER os-tk

VOLUME /home/os-tk/ostk-env
WORKDIR /home/os-tk/ostk-env