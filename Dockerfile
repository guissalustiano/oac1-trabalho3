FROM debian:12-slim

RUN apt update

RUN apt install -y linux-perf

RUN apt install -y gcc

RUN apt install -y default-jdk

RUN apt install -y python3 python3-pip python-is-python3

RUN apt install -y pypy3 && alias pypy=pypy3

COPY src/requirements.txt tmp/requirements.txt
RUN pip install -r tmp/requirements.txt --break-system-packages
