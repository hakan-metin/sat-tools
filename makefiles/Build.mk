exec := sat

sources := $(wildcard $(SRC)*.cc)
headers := $(wildcard $(SRC)*.h)

objects         := $(patsubst %.cc, $(OBJ)%.o, $(sources))
release_objects := $(patsubst %.cc, $(OBJ)release/%.o, $(sources))
debug_objects   := $(patsubst %.cc, $(OBJ)debug/%.o, $(sources))

tests := $(wildcard tests/units/*.test.cc)
tests_objects := $(patsubst %.cc, $(OBJ)%.o, $(tests))
tests_objects += $(debug_objects)
tests_objects := $(filter-out %main.o, $(tests_objects))

$(call REQUIRE-DIR, $(objects))
$(call REQUIRE-DIR, $(BIN)$(exec))

$(call REQUIRE-DIR, $(release_objects))
$(call REQUIRE-DIR, $(BIN)$(exec)_release)

$(call REQUIRE-DIR, $(debug_objects))
$(call REQUIRE-DIR, $(BIN)$(exec)_debug)

$(call REQUIRE-DIR, $(tests_objects))
$(call REQUIRE-DIR, $(BIN)test)

$(call REQUIRE-DEP, $(sources))
$(call REQUIRE-DEP, $(tests))


$(BIN)$(exec): $(objects)
$(BIN)$(exec)_release: $(release_objects)
$(BIN)$(exec)_debug: $(debug_objects)

CFLAGS += -I. -I$(SRC) -DUSE_BLISS #-DUSE_GLOG
LDFLAGS += -lbliss #-lglog

default: CFLAGS += -O3 -fPIC -Wall -Wextra
default: $(BIN)$(exec)

release: CFLAGS += -O3 -fPIC -Wall -Wextra -DNDEBUG
release: $(BIN)$(exec)_release

debug: CFLAGS += -O0 -fPIC -Wall -Wextra -g  -DDEBUG
debug: $(BIN)$(exec)_debug

.PHONY: default release debug


################################################################################
# TESTS

test: CFLAGS +=  -O0 --coverage -fPIC
test: LDFLAGS += -lgtest -lgtest_main -lpthread -lgcov
test: $(BIN)test
run-test: test
	$(call 	cmd-call, ./$(BIN)test)
run-test-valgrind: test
	$(call 	cmd-call, valgrind --leak-check=full ./$(BIN)test)
run-test-gdb: test
	$(call 	cmd-call, gdb --args ./$(BIN)test)

check-style: $(sources) $(headers)
	$(call cmd-call, ./tests/sanity/cpplint.py, $^)

################################################################################

# Generic rules

$(BIN)$(exec): $(objects)
	$(call cmd-ld, $@, $^)

$(BIN)$(exec)_release: $(release_objects)
	$(call cmd-ld, $@, $^)

$(BIN)$(exec)_debug: $(debug_objects)
	$(call cmd-ld, $@, $^)

$(OBJ)release/%.o: %.cc
	$(call cmd-cxx, $@, $<, $(CFLAGS))

$(OBJ)debug/%.o: %.cc
	$(call cmd-cxx, $@, $<, $(CFLAGS))

##

$(OBJ)%.test.o: %.test.cc
	$(call cmd-cxx, $@, $<, $(CFLAGS))

$(BIN)test: $(tests_objects)
	$(call cmd-ld, $@, $^, $(LDFLAGS))
