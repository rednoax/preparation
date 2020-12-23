#TO TEST:
#this is an example from "GNU Make book" chapter 1
export FOO=$(shell echo fooey)
all:;@echo FOO is $$FOO