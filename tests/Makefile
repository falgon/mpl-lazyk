rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
CC := g++
CFLAGS := -Wall -Wextra -pedantic -std=c++1z -O0
INCLUDES := -I../

TEST_SRCS := $(call rwildcard,.,*.cpp)
TEST_BINS := $(patsubst %.cpp,%,$(TEST_SRCS))
TESTS := $(patsubst %.cpp,%.test,$(TEST_SRCS))
BOOST_TEST_FLAGS := --show_progress=yes\
	--build_info=yes\
	--detect_memory_leak=1\
	--result_code=yes\
	--log_level=all

all: $(TEST_BINS) $(TESTS)

%: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

%.test: %
	@./$< $(BOOST_TEST_FLAGS)

clean:
	@$(RM) $(TEST_BINS)

