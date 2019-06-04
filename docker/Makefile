TAG_NAME := scone-pkcs
FILE := ./Dockerfile
CONTEXT := ../
SGX_DEV := /dev/isgx

.PHONY: all run

all: build

build: $(FILE) Makefile
	docker build -t $(TAG_NAME) -f $(FILE) $(CONTEXT)

run:
	docker run --device=$(SGX_DEV) --network=host --rm -it $(TAG_NAME)
