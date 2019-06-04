# FROM ubuntu:18.04
# RUN apt-get update && apt-get install -y make gcc

FROM alpine:3.9
RUN apk add gcc libc-dev libstdc++
RUN apk add make tar
RUN apk --no-cache add ca-certificates wget \
    && wget -q -O /etc/apk/keys/sgerrand.rsa.pub https://alpine-pkgs.sgerrand.com/sgerrand.rsa.pub \
    && wget https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.29-r0/glibc-2.29-r0.apk \
    && apk add glibc-2.29-r0.apk

COPY docker/primusP11-1.0.2.715.tar.gz .
RUN tar -xvf primusP11-1.0.2.715.tar.gz
COPY docker/primus.cfg /usr/local/primus/etc/primus.cfg
COPY docker/secrets.cfg /usr/local/primus/etc/.secrets.cfg

WORKDIR app
COPY . .

# CMD ["make", "check"]
# CMD ["make", "check", "SH=/bin/ash"]
CMD ["/bin/ash"]
