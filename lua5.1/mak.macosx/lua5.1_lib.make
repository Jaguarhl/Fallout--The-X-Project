# C Shared Library Makefile autogenerated by premake
# Don't edit this file! Instead edit `premake.lua` then rerun `make`

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := ../bin
  LIBDIR := ../lib
  OBJDIR := ../obj/lua5.1_lib/Debug
  OUTDIR := ../lib
  CPPFLAGS := -MD -DLUA_USE_POSIX -DLUA_DL_DYLD -I "../include"
  CFLAGS += $(CPPFLAGS) -g
  CXXFLAGS := $(CFLAGS)
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR)
  LDDEPS :=
  TARGET := liblua5.1.a
  BLDCMD = ar -cr $(OUTDIR)/$(TARGET) $(OBJECTS); ranlib $(OUTDIR)/$(TARGET)
endif

ifeq ($(CONFIG),Release)
  BINDIR := ../bin
  LIBDIR := ../lib
  OBJDIR := ../obj/lua5.1_lib/Release
  OUTDIR := ../lib
  CPPFLAGS := -MD -DLUA_USE_POSIX -DLUA_DL_DYLD -I "../include"
  CFLAGS += $(CPPFLAGS) -O2
  CXXFLAGS := $(CFLAGS)
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR)
  LDDEPS :=
  TARGET := liblua5.1.a
  BLDCMD = ar -cr $(OUTDIR)/$(TARGET) $(OBJECTS); ranlib $(OUTDIR)/$(TARGET)
endif

OBJECTS := \
	$(OBJDIR)/lapi.o \
	$(OBJDIR)/lcode.o \
	$(OBJDIR)/ldebug.o \
	$(OBJDIR)/ldo.o \
	$(OBJDIR)/ldump.o \
	$(OBJDIR)/lfunc.o \
	$(OBJDIR)/lgc.o \
	$(OBJDIR)/llex.o \
	$(OBJDIR)/lmem.o \
	$(OBJDIR)/lobject.o \
	$(OBJDIR)/lopcodes.o \
	$(OBJDIR)/lparser.o \
	$(OBJDIR)/lstate.o \
	$(OBJDIR)/lstring.o \
	$(OBJDIR)/ltable.o \
	$(OBJDIR)/ltm.o \
	$(OBJDIR)/lundump.o \
	$(OBJDIR)/lvm.o \
	$(OBJDIR)/lzio.o \
	$(OBJDIR)/lauxlib.o \
	$(OBJDIR)/lbaselib.o \
	$(OBJDIR)/ldblib.o \
	$(OBJDIR)/liolib.o \
	$(OBJDIR)/lmathlib.o \
	$(OBJDIR)/loslib.o \
	$(OBJDIR)/ltablib.o \
	$(OBJDIR)/lstrlib.o \
	$(OBJDIR)/loadlib.o \
	$(OBJDIR)/linit.o \

CMD := $(subst \,\\,$(ComSpec)$(COMSPEC))
ifeq (,$(CMD))
  CMD_MKBINDIR := mkdir -p $(BINDIR)
  CMD_MKLIBDIR := mkdir -p $(LIBDIR)
  CMD_MKOUTDIR := mkdir -p $(OUTDIR)
  CMD_MKOBJDIR := mkdir -p $(OBJDIR)
else
  CMD_MKBINDIR := $(CMD) /c if not exist $(subst /,\\,$(BINDIR)) mkdir $(subst /,\\,$(BINDIR))
  CMD_MKLIBDIR := $(CMD) /c if not exist $(subst /,\\,$(LIBDIR)) mkdir $(subst /,\\,$(LIBDIR))
  CMD_MKOUTDIR := $(CMD) /c if not exist $(subst /,\\,$(OUTDIR)) mkdir $(subst /,\\,$(OUTDIR))
  CMD_MKOBJDIR := $(CMD) /c if not exist $(subst /,\\,$(OBJDIR)) mkdir $(subst /,\\,$(OBJDIR))
endif

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking lua5.1_lib
	-@$(CMD_MKBINDIR)
	-@$(CMD_MKLIBDIR)
	-@$(CMD_MKOUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning lua5.1_lib
	-@rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)

$(OBJDIR)/lapi.o: ../src/lapi.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lcode.o: ../src/lcode.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/ldebug.o: ../src/ldebug.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/ldo.o: ../src/ldo.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/ldump.o: ../src/ldump.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lfunc.o: ../src/lfunc.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lgc.o: ../src/lgc.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/llex.o: ../src/llex.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lmem.o: ../src/lmem.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lobject.o: ../src/lobject.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lopcodes.o: ../src/lopcodes.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lparser.o: ../src/lparser.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lstate.o: ../src/lstate.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lstring.o: ../src/lstring.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/ltable.o: ../src/ltable.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/ltm.o: ../src/ltm.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lundump.o: ../src/lundump.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lvm.o: ../src/lvm.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lzio.o: ../src/lzio.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lauxlib.o: ../src/lauxlib.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lbaselib.o: ../src/lbaselib.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/ldblib.o: ../src/ldblib.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/liolib.o: ../src/liolib.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lmathlib.o: ../src/lmathlib.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/loslib.o: ../src/loslib.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/ltablib.o: ../src/ltablib.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/lstrlib.o: ../src/lstrlib.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/loadlib.o: ../src/loadlib.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/linit.o: ../src/linit.c
	-@$(CMD_MKOBJDIR)
	@echo $(notdir $<)
	@$(CC) $(CFLAGS) -o $@ -c $<

-include $(OBJECTS:%.o=%.d)

