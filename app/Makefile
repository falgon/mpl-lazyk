rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
CC := g++
CFLAGS := -Wall -Wextra -pedantic -std=c++1z -O0
EXT_CFLAGS :=
INPUT := "Hello, world!"
LAZYK_CODE := "(SKK)I"
LAZYK := -D INPUT='$(INPUT)' -D INPUT_LEN=$(shell echo -n $(INPUT) | wc -m) \
	-D LAZYK_CODE='$(LAZYK_CODE)' -D LAZYK_CODE_LEN=$(shell echo -n $(LAZYK_CODE) | wc -m)
INCLUDES := -I../

SRCS := $(call rwildcard,.,*.cpp)
BINS := $(patsubst %.cpp,%,$(SRCS))
TESTS := $(patsubst %.cpp,%.bin,$(SRCS))

all: $(BINS) $(TESTS)

%: %.cpp
	@$(CC) $(CFLAGS) $(EXT_CFLAGS) $(LAZYK) $(INCLUDES) -o $@ $<

%.bin: %
	@./$<

clean:
	@$(RM) $(BINS)

