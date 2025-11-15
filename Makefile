NAME=ThaiPlusPlus

CC=clang
CXX=clang++

LIBS=

INC_DIR=include/

LIBFLAGS=$(addprefix -l,$(LIBS))
INCLUDE_FLAGS=$(addprefix -I,$(INC_DIR))

include ./src/Makefile

OBJS=$(addprefix build/, $(filter %.o, $(SRCS:.cpp=.o) $(SRCS:.c=.o)))
CFLAGS=-Wall

all: $(OBJS)
	$(CXX) $^ -o $(NAME).exe $(LIBFLAGS) $(INCLUDE_FLAGS)

build/%.o: %.c
	mkdir "$(dir $@)"
	$(CC) -c $< -o $@

build/%.o: %.cpp
	mkdir "$(dir $@)"
	$(CXX) -c $< -o $@

clean:
	rm -rf $(NAME).exe build/
