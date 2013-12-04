PRGM = CLT-MHD

CC = g++

CPPFLAGS =
LDFLAGS = 
CFLAGS = -Wall -ggdb3 
BOOST = -I$(BOOST_INCLUDE) -L$(BOOST_LIB) -lboost_program_options
ALGLIB	=	alglib/src
HDF5I	= /usr/include
HDF5L	= -L/usr/lib64 -lhdf5_cpp

COMPILE = $(CC) $(CPPFLAGS) $(CFLAGS)  -I$(BOOST_INCLUDE) -I$(ALGLIB) -c 
LINKCC = $(CC) $(LDFLAGS) $(BOOST) 

LIBA = 

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

all:$(PRGM)

$(PRGM):$(OBJS) 
	$(LINKCC) $(OBJS) $(LIBA) $(HDF5L) $(ALGLIB)/*.o -o $(PRGM)

%.o:%.cpp
	$(COMPILE) $< -o $@

.PHONY:clean
clean:
	rm -f $(OBJS) $(PRGM)

explain:
	@echo "The information represents in the program:"
	@echo "Final executable name: $(PRGM)"
	@echo "Source files: $(SRCS)"
	@echo "Object files: $(OBJS)"

depend:$(DEPS)
	@echo "Dependencies are now up-to-date"

