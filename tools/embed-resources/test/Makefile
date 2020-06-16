
PYTHON ?= python3.8

.PHONY: all
all: resources build

.PHONY: resources
resources:
	$(PYTHON) ../main.py basic_gl4 ../../../assets/shader/basic_gl4.glsl

.PHONY: build
build:
	$(CXX) main.cpp basic_gl4.cpp -o embed_test -std=c++17
