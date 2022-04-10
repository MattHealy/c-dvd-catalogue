.PHONY: all build run
.DEFAULT_GOAL := all

all: build

build:
	gcc dvd_library.c -o dvd_library

run:
	./dvd_library
