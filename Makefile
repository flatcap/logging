V	?= 0
CC	?= gcc
RM	= rm -fr
MKDIR	= mkdir -p

DEPDIR	= .dep
OBJDIR	= .obj

OUT	= main

SRC	+= $(wildcard *.c)

HDR	+= $(wildcard *.h)

OBJ	+= $(SRC:%.c=$(OBJDIR)/%.o)

CFLAGS	+= -Wall
# CFLAGS	+= -Wextra
CFLAGS	+= -Wpedantic
CFLAGS	+= -g

all:	$(OBJDIR) $(DEPDIR) $(OBJ) $(OUT) tags

# ----------------------------------------------------------------------------

ifneq ($(V),1)
Q	:= @
endif
QUIET_CC      = $(Q:@=@echo 'CC      '$<;)
QUIET_LINK    = $(Q:@=@echo 'LD      '$@;)
QUIET_TAGS    = $(Q:@=@echo 'TAGS    '$@;)

ifneq ($(filter s% -s%,$(MAKEFLAGS)),)
	QUIET_CC      =
	QUIET_LINK    =
	QUIET_TAGS    =
endif

# ----------------------------------------------------------------------------

$(OBJDIR)/%.o: %.c
	$(QUIET_CC)$(CC) $(CFLAGS) -c $< -o $@ && (										\
	$(CC) -MM $(CFLAGS) -c $< | sed 's/.*:/'$(OBJDIR)'\/\0/' > $(DEPDIR)/$*.d;						\
	cp -f $(DEPDIR)/$*.d $(DEPDIR)/$*.d.tmp;										\
	sed -e 's/.*://' -e 's/\\$$//' < $(DEPDIR)/$*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPDIR)/$*.d;		\
	rm -f $(DEPDIR)/$*.d.tmp)

# ----------------------------------------------------------------------------

$(OUT):	$(OBJ)
	$(QUIET_LINK)$(CC) -o $@ $(OBJ) $(LDFLAGS)

$(DEPDIR) $(OBJDIR):
	$(Q)$(MKDIR) $@

# ----------------------------------------------------------------------------

tags:	$(SRC) $(HDR)
	$(QUIET_TAGS)ctags -R .

clean:
	$(Q)$(RM) $(OUT) $(OBJ) log.txt tmp-*

distclean: clean
	$(Q)$(RM) $(DEPDIR) $(OBJDIR) tags

test:	all
	@for i in c f m q s; do \
		rm -f log.txt; \
		./main $$i; \
		[ -f log.txt ] && cat log.txt || true; \
	done

valgrind:	all
	@for i in c f m q s; do \
		valgrind ./main $$i; \
	done

force:

-include $(SRC:%.c=$(DEPDIR)/%.d)

