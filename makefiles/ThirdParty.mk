GTEST_BASE_DIR := third_party/gtest/
GTEST_URL := https://github.com/google/googletest.git

GTEST_DIR := $(GTEST_BASE_DIR)googletest/

libgtest := $(LIB)libgtest.a
libgtest_main := $(LIB)libgtest_main.a

gtest_sources := $(GTEST_DIR)src/gtest-all.cc \
	         $(GTEST_DIR)src/gtest_main.cc
gtest_objects := $(patsubst %.cc, $(OBJ)%.o, $(gtest_sources))

$(call REQUIRE-DIR, $(gtest_objects))
$(call REQUIRE-DIR, $(libgtest))
$(call REQUIRE-DIR, $(libgtest_main))

gtest: CFLAGS += -isystem $(GTEST_DIR)include -I$(GTEST_DIR) -pthread
gtest: $(libgtest) $(libgtest_main)

$(GTEST_BASE_DIR):
	$(call cmd-clone, $(GTEST_URL), $@)

$(gtest_sources): $(GTEST_BASE_DIR)

$(libgtest): $(OBJ)$(GTEST_DIR)src/gtest-all.o $(gtest_sources)
	$(call cmd-ar, $@, $<)

$(libgtest_main): $(OBJ)$(GTEST_DIR)src/gtest_main.o $(gtest_sources)
	$(call cmd-ar, $@, $<)
