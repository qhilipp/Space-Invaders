CXX	:= c++
CXXFLAGS := -Wall -std=c++20

# Contain path for any includes (headers)
# Depending on your platform: Include a path to boost, on linux should be 
# /usr/local/include, on mac could be /opt/homebrew/include
INCLUDES := -I./include -I/usr/local/include 

# Contains libraries we need to (-L is directory search path, -l is lib)
LDFLAGS = -L/usr/local/lib -L/usr/local/lib 
LDLIBS = -lncurses -lboost_unit_test_framework

SRCDIR := ./src

game:
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDLIBS)

tests:
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDLIBS)

%.o:
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $^ -o $@

clean:
	test ! -f game || rm game
	test ! -f tests || rm tests
	rm *.o