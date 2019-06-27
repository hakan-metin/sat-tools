
EXAMPLES := examples/

bins_src := $(wildcard $(EXAMPLES)*.cc)
bins := $(patsubst $(EXAMPLES)%.cc, $(BIN)%, $(bins_src))

$(call REQUIRE-DIR, $(bins))

example: $(bins)

$(BIN)%: $(EXAMPLES)%.cc default
	$(call cmd-cxx-bin, $@, $<, $(CFLAGS) -L lib -lsattools -lbliss $(LDFLAGS))
