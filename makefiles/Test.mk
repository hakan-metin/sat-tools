tests := $(wildcard tests/units/*.test.cc)
tests_objects := $(patsubst %.cc, $(OBJ)%.o, $(tests))
tests_objects += $(debug_objects)
tests_objects := $(filter-out %main.o, $(tests_objects))



$(call REQUIRE-DIR, $(tests_objects))
$(call REQUIRE-DIR, $(BIN)test)
$(call REQUIRE-DEP, $(tests))


test: CFLAGS +=  -O0 --coverage -fPIC

test: LDFLAGS += -lgtest -lgtest_main -lpthread -lgcov
test: $(BIN)test
run-test: test
	$(call 	cmd-call, ./$(BIN)test)
run-test-valgrind: test
	$(call 	cmd-valgrind,  ./$(BIN)test)
run-test-gdb: test
	$(call 	cmd-gdb, ./$(BIN)test)


$(OBJ)%.test.o: %.test.cc
	$(call cmd-cxx, $@, $<, $(CFLAGS))

$(BIN)test: $(tests_objects)
	$(call cmd-ld, $@, $^, $(LDFLAGS))
