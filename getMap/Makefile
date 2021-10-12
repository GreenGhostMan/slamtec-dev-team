
HOME_TREE := ../../

MODULE_NAME := $(notdir $(CURDIR))

CXXSRC 	   := src/main.cpp
CXXFLAGS   := -std=c++0x
LDFLAGS    := -static

LD_LIBS    =  -Xlinker "-(" \
              -lstdc++ -ldl -lrt -lm -lpthread \
              -l$(RPOS_SLAMWARE_NAME) -l$(RPOS_CORE_NAME) \
              -lboost_atomic \
              -lboost_chrono \
              -lboost_date_time \
              -lboost_regex \
              -lboost_system \
              -lboost_thread \
              -lboost_filesystem \
              -lboost_random \
              -lbase64 \
              -ljsoncpp \
              -lrlelib \
              -lcrypto \
              -lcurl \
              -lssl \
              -Xlinker "-)" \

all: build_app

clean: clean_app

#includes the common definitions...
#do not remove them unless you know what you are doing
include $(HOME_TREE)/mak_def.inc
include $(HOME_TREE)/mak_common.inc

