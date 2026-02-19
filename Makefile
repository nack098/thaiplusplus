NAME=ThaiPlusPlus

CC=clang
CXX=clang++

LIBS=

INC_DIRS=include/

LIBFLAGS=$(addprefix -l,$(LIBS))
INCLUDE_FLAGS=$(addprefix -I,$(INC_DIRS))

include ./src/Makefile

OBJS=$(addprefix build/, $(filter %.o, $(SRCS:.cpp=.o) $(SRCS:.c=.o)))
CFLAGS=-Wall

all: $(OBJS)
	$(CXX) $^ -o $(NAME).exe $(LIBFLAGS)

build/%.o: %.c
	mkdir -p "$(dir $@)"
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_FLAGS)

build/%.o: %.cpp
	mkdir -p "$(dir $@)"
	$(CXX) -c $< -o $@ $(CFLAGS) $(INCLUDE_FLAGS)

.PHONY: clean
clean:
	if exist $(NAME).exe del /Q $(NAME).exe
	if exist build rmdir /S /Q build
