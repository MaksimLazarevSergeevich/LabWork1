PROJECT = bmp
LIBPROJECT = $(PROJECT).a
TESTPROJECT = test-$(PROJECT)

CXX = g++
A = ar
AFLAGS = rsv

CXXFLAGS = -I. -std=c++17 -Wall -Werror -Wpedantic -g -fPIC
LDXXFLAGS = $(CXXFLAGS) -L. -l:$(LIBPROJECT)
LDGTESTFLAGS = $(LDXXFLAGS) -lgtest -lgtest_main -lpthread

DEPS = $(wildcard include/*.h)
SRCS = $(wildcard src/*.cpp)
OBJ = $(SRCS:.cpp=.o)
TEST-SRCS = $(wildcard tests/*.cpp)
TEST-OBJ = $(TEST-SRCS:.cpp=.o)

.PHONY: default
default: all

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(LIBPROJECT): $(OBJ)
	$(A) $(AFLAGS) $@ $^

$(PROJECT): main.o $(LIBPROJECT)
	$(CXX) -o $@ main.o $(LDXXFLAGS)

$(TESTPROJECT): $(LIBPROJECT) $(TEST-OBJ)
	$(CXX) -o $@ $(TEST-OBJ) $(LDGTESTFLAGS)

test: $(TESTPROJECT)

all: $(PROJECT)

.PHONY: clean
clean:
	rm -f *.o src/*.o tests/*.o

.PHONY: cleanall
cleanall: clean
	rm -f $(PROJECT) $(LIBPROJECT) $(TESTPROJECT)