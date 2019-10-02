
proto      := $(wildcard $(SRC)*.proto)
proto_gen  := $(patsubst %.proto, %.pb.cc, $(proto))

sources := $(wildcard $(SRC)*.cc) $(proto_gen)
headers := $(wildcard $(SRC)*.h)
dst_headers := $(patsubst $(SRC)%.h, $(INC)%.h, $(headers))

objects         := $(patsubst %.cc, $(OBJ)%.o, $(sources))
release_objects := $(patsubst %.cc, $(OBJ)release/%.o, $(sources))
debug_objects   := $(patsubst %.cc, $(OBJ)debug/%.o, $(sources))

tests := $(wildcard tests/units/*.test.cc)
tests_objects := $(patsubst %.cc, $(OBJ)%.o, $(tests))
tests_objects += $(debug_objects)
tests_objects := $(filter-out %main.o, $(tests_objects))

$(call REQUIRE-DIR, $(INC))

$(call REQUIRE-DIR, $(objects))
$(call REQUIRE-DIR, $(LIB)libsattools.a)

$(call REQUIRE-DIR, $(tests_objects))
$(call REQUIRE-DIR, $(BIN)test)

$(call REQUIRE-DEP, $(sources))
$(call REQUIRE-DEP, $(tests))


CFLAGS  += -I. -I$(SRC)
LDFLAGS += -lz  -lpthread

default: CFLAGS += -O3 -fPIC -Wall -Wextra -g
default: $(LIB)libsattools.a

release: CFLAGS += -O3 -fPIC -Wall -Wextra -DNDEBUG

debug: CFLAGS += -O0 -fPIC -Wall -Wextra -g  -DDEBUG

.PHONY: default release debug

.generate: $(proto_gen)

################################################################################
# TESTS

test: CFLAGS +=  -O0 --coverage -fPIC
test: LDFLAGS += -lgtest -lgtest_main -lpthread -lgcov
test: $(BIN)test
run-test: test
	$(call 	cmd-call, ./$(BIN)test)
run-test-valgrind: test
	$(call 	cmd-valgrind,  ./$(BIN)test)
run-test-gdb: test
	$(call 	cmd-gdb, ./$(BIN)test)

check-style: $(sources) $(headers)
	$(call cmd-call, ./tests/sanity/cpplint.py, $^)

################################################################################

# Generic rules

$(LIB)libsattools.a: $(objects) $(headers) | $(INC)
	$(call cmd-ar, $@, $^)

# $(call cmd-cp, $(INC), $(headers))


$(OBJ)release/%.o: %.cc
	$(call cmd-cxx, $@, $<, $(CFLAGS))

$(OBJ)debug/%.o: %.cc
	$(call cmd-cxx, $@, $<, $(CFLAGS))

##

$(OBJ)%.test.o: %.test.cc
	$(call cmd-cxx, $@, $<, $(CFLAGS))

$(BIN)test: $(tests_objects)
	$(call cmd-ld, $@, $^, $(LDFLAGS))

%.pb.cc: %.proto
	$(call cmd-protoc,$<,$(SRC))
