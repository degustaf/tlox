# Defines

ifeq ($(OSTYPE),cygwin)
	CLEANUP=rm -f
	CLEANDIR=rm -rf
	MKDIR=mkdir -p
	TARGET_EXTENSION=
	MATHLIB=-lm
else ifeq ($(OS),Windows_NT)
	CLEANUP=del /F /Q
	CLEANDIR=rd /S /Q
	MKDIR=mkdir
	TARGET_EXTENSION=.exe
	MATHLIB=
else
	CLEANUP=rm -f
	CLEANDIR=rm -rf
	MKDIR=mkdir -p
	TARGET_EXTENSION=
	MATHLIB=-lm
endif

PATHS = 			src
PATHI =				include
PATHU =				unittest
PATHC = 			clients
PATHB = 			build
PATHLB = 			lib$(PATHB)
PATHD =				depend
PATHUB =			unittestBuild
SRCS =				$(wildcard $(PATHS)/*.cpp)
USRCS =				$(wildcard $(PATHU)/*.cpp)
LIBRARY =			libtlox.a
CC = 				g++
LINK = 				$(CC)
AR = 				ar
C_STD =				c++11
DEF =				-g
CFLAGS =			-I$(PATHS) -I$(PATHI) -Wall -Wextra -Werror $(ARCH) -std=$(C_STD) -D_POSIX_C_SOURCE=200809L $(DEF)
CLIENT_CFLAGS = 	-I$(PATHI) -Wall -Wextra -Werror -pedantic $(ARCH) -std=$(C_STD) -D_POSIX_C_SOURCE=200809L $(DEF)

LDFLAGS =			$(ARCH) $(DEF)
LDLIBS =			$(MATHLIB)

COMPILE =			$(CC) $(CFLAGS) -MT $@ -MP -MMD -MF $(PATHD)/$*.Td
OBJS =				$(addprefix $(PATHLB)/, $(notdir $(SRCS:.cpp=.o)))
POSTCOMPILE =		@mv -f $(PATHD)/$*.Td $(PATHD)/$*.d && touch $@
UBINS =				$(addprefix $(PATHUB)/, $(notdir $(USRCS:.cpp=$(TARGET_EXTENSION))))

.PHONY: all clean test unittest release benchmark

.PRECIOUS: $(PATHD)/%.d
.PRECIOUS: $(PATHB)/%.o
.PRECIOUS: $(PATHB)/%.a
.PRECIOUS: $(PATHLB)/%.o
.PRECIOUS: $(PATHUB)/%


# Rules

all: tlox$(TARGET_EXTENSION)


./$(PATHB):
	$(MKDIR) $@

$(PATHLB):
	$(MKDIR) $@

$(PATHD):
	$(MKDIR) $@

$(PATHUB):
	$(MKDIR) $@


tlox$(TARGET_EXTENSION): $(PATHB)/tlox$(TARGET_EXTENSION)
	ln -sf $^ $@

ifeq ($(PATHB),releasebuild)
else
releasebuild/tlox$(TARGET_EXTENSION):
	$(MAKE) release
endif

$(PATHB)/%$(TARGET_EXTENSION): $(PATHB)/%.o $(PATHLB)/$(LIBRARY) | $(PATHB)
	$(LINK) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(PATHLB)/$(LIBRARY): $(OBJS)
	$(MAKE) DEF="$(DEF)" PATHB="$(PATHB)" unittest
	$(RM) $@
	$(AR) rcs $@ $^

$(PATHB)/%.o: $(PATHC)/%.cpp | $(PATHB) $(PATHD)
	$(CC) $(CLIENT_CFLAGS) -MT $@ -MP -MMD -MF $(PATHD)/$*.Td -c $< -o $@
	$(POSTCOMPILE)

ifeq ($(CC),gcc)
$(PATHLB)/vm.o: $(PATHS)/vm.cpp | $(PATHLB) $(PATHD)
	$(CC) $(CFLAGS) -fno-gcse -fno-crossjumping -MT $@ -MP -MMD -MF $(PATHD)/vm.Td -c $< -o $@
	@mv -f $(PATHD)/vm.Td $(PATHD)/vm.d && touch $@
endif

$(PATHLB)/%.o: $(PATHS)/%.cpp | $(PATHLB) $(PATHD)
	$(COMPILE) -c $< -o $@
	$(POSTCOMPILE)

$(PATHUB)/%.o: $(PATHU)/%.cpp | $(PATHUB) $(PATHD)
	$(COMPILE) -c $< -o $@
	$(POSTCOMPILE)

$(PATHUB)/%$(TARGET_EXTENSION): $(PATHUB)/%.o | $(PATHB)
	$(LINK) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	$@

test: tlox$(TARGET_EXTENSION)
	python3 util/test.py $<

unittest: $(UBINS)

release:
	$(MAKE) DEF="$(DEF) -DNDEBUG -O3" PATHB="releasebuild" test

benchmark: release
	python3 util/benchmark.py releasebuild/tlox$(TARGET_EXTENSION)

clean: _clean
	$(MAKE) PATHB="releasebuild" _clean
	$(CLEANUP) $(PATHD)/*.d
	$(CLEANUP) $(PATHD)/*.Td
	$(CLEANUP) $(PATHUB)/*
	$(CLEANUP) ./tlox$(TARGET_EXTENSION)
	$(CLEANUP) ./gmon.out

_clean:
	$(CLEANUP) $(PATHLB)/*
	$(CLEANUP) $(PATHB)/*

include $(wildcard $(PATHD)/*.d)
