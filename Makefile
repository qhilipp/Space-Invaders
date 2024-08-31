# Compiler and flags
CXX := c++
CXXFLAGS := -Wall -std=c++20
INCLUDES := -I./include -I/mingw64/include/ncurses -I/mingw64/include/boost
LDFLAGS := -L/mingw64/lib
LDLIBS := -lncursesw -lboost_unit_test_framework-mt

# Directories and files
SRCDIR := ./src
BINDIR := ./bin
TESTDIR := ./tests

GAME_SRCS := $(wildcard $(SRCDIR)/*.cpp)
GAME_OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(GAME_SRCS))

# Test files
TEST_SRCS := $(wildcard $(TESTDIR)/*.cpp)
TEST_OBJS := $(patsubst $(TESTDIR)/%.cpp,$(BINDIR)/%.o,$(TEST_SRCS))

# Targets
all: game tests

game: $(GAME_OBJS)
	$(CXX) $^ -o $(BINDIR)/game $(LDFLAGS) $(LDLIBS)

tests: $(TEST_OBJS) $(BINDIR)/util.o  # Include util.o for test
	$(CXX) $^ -o $(BINDIR)/tests $(LDFLAGS) $(LDLIBS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BINDIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(BINDIR)/game $(BINDIR)/tests
	$(RM) $(BINDIR)/*.o

run: $(BINDIR)/game
	./$(BINDIR)/game

run_tests: $(BINDIR)/tests
	./$(BINDIR)/tests
