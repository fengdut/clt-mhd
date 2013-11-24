PRGM = CLT-MHD

CC = g++
CPPFLAGS =
LDFLAGS = 
CFLAGS = -Wall -ggdb3
COMPILE = $(CC) $(CPPFLAGS) $(CFLAGS) -c
LINKCC = $(CC) $(LDFLAGS)

LIBA = 

SRCS := $(wildcard *.cxx)
DEPS := $(patsubst %.cxx,%.d,$(SRCS))
OBJS := $(patsubst %.cxx,%.o,$(SRCS))

all:$(PRGM)

$(PRGM):$(OBJS) $(DEPS)
	$(LINKCC) $(OBJS) $(LIBA) -o $(PRGM)

%.d:%.cxx
	$(CC) -MM $(CPPFLAGS) $< > $@

%.o:%.cxx
	$(COMPILE) $< -o $@

.PHONY:clean
clean:
	rm -f $(OBJS) $(DEPS) $(PRGM)

explain:
	@echo "The information represents in the program:"
	@echo "Final executable name: $(PRGM)"
	@echo "Source files: $(SRCS)"
	@echo "Object files: $(OBJS)"

depend:$(DEPS)
	@echo "Dependencies are now up-to-date"

-include $(DEPS)
