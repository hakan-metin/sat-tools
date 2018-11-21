
EXAMPLES := examples/

cnfsym: cnfbliss cnfsaucy
cnfbliss: $(BIN)CNFBliss
cnfsaucy: $(BIN)CNFSaucy

$(call REQUIRE-DIR, $(BIN)CNFBliss)
$(call REQUIRE-DIR, $(BIN)CNFSaucy)

LDFLAGS += -lsattools
$(BIN)CNFBliss: $(EXAMPLES)CNFBliss.cc | default
	$(call cmd-cxx-bin, $@, $<, $(LDFLAGS))

$(BIN)CNFSaucy: $(EXAMPLES)CNFSaucy.cc | default
	$(call cmd-cxx-bin, $@, $<, $(LDFLAGS))
