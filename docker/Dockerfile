FROM ubuntu:18.04

# RUN apk add gcc libc-dev
# RUN apk add make tar

RUN apt-get update && apt-get install -y make gcc

COPY docker/primusP11-1.0.2.715.tar.gz .
RUN tar -xvf primusP11-1.0.2.715.tar.gz
COPY docker/primus.cfg /usr/local/primus/etc

WORKDIR app
COPY . .

CMD ["/bin/bash"]
