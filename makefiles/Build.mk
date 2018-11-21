exec := sat

sources := $(wildcard $(SRC)base/*.cc)   \
	   $(wildcard $(SRC)io/*.cc)     \
           $(wildcard $(SRC)sat/*.cc)    \
           $(wildcard $(SRC)utils/*.cc)  \
           $(wildcard $(SRC)sym/*.cc)

headers := $(wildcard $(SRC)base/*.h)   \
	   $(wildcard $(SRC)io/*.h)     \
           $(wildcard $(SRC)sat/*.h)    \
           $(wildcard $(SRC)utils/*.h)  \
           $(wildcard $(SRC)sym/*.h)

objects         := $(patsubst %.cc, $(OBJ)%.o, $(sources))
release_objects := $(patsubst %.cc, $(OBJ)release/%.o, $(sources))
debug_objects   := $(patsubst %.cc, $(OBJ)debug/%.o, $(sources))

$(call REQUIRE-DIR, $(objects))
$(call REQUIRE-DIR, $(BIN)$(exec))

$(call REQUIRE-DIR, $(release_objects))
$(call REQUIRE-DIR, $(BIN)$(exec)_release)

$(call REQUIRE-DIR, $(debug_objects))
$(call REQUIRE-DIR, $(LIB)/libsattools.a)
$(call REQUIRE-DIR, $(LIB)/libsattools_release.a)
$(call REQUIRE-DIR, $(LIB)/libsattools_debug.a)

$(call REQUIRE-DEP, $(sources))


$(BIN)$(exec): $(objects)
$(BIN)$(exec)_release: $(release_objects)
$(BIN)$(exec)_debug: $(debug_objects)

CFLAGS += -I. -I$(SRC) -DUSE_GLOG
LDFLAGS += -lbliss -lsaucy -lglog -lz

default: release

release: CFLAGS += -O3 -fPIC -Wall -Wextra -DNDEBUG
release: $(LIB)/libsattools_release.a

debug: CFLAGS += -O0 -fPIC -Wall -Wextra -g -DDEBUG
debug: $(LIB)/libsattools_debug.a

.PHONY: default release debug test


check-style: $(sources) $(headers)
	$(call cmd-call, ./tests/sanity/cpplint.py, $^)

################################################################################

# Generic rules

$(LIB)/libsattools_release.a: $(release_objects)
	$(call cmd-ar, $@, $^)
	$(call cmd-ln, $(notdir $@), $(LIB)/libsattools.a)

$(LIB)/libsattools_debug.a: $(debug_objects)
	$(call cmd-ar, $@, $^)
	$(call cmd-ln, $(notdir $@), $(LIB)/libsattools.a)

$(OBJ)release/%.o: %.cc
	$(call cmd-cxx, $@, $<, $(CFLAGS))

$(OBJ)debug/%.o: %.cc
	$(call cmd-cxx, $@, $<, $(CFLAGS))
