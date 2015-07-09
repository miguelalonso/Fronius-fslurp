# $RCSfile: Makefile,v $
# $Revision: 1.8 $
# $Date: 2013/12/21 14:51:13 $
#

CC		= g++

TARGETS		= fslurp simIFCEasy \
		simSolarNet2i1s simSolarNet1i0s simSolarNet0i0s \
		sim422IGTL

COMMON_SRC	= args.cpp interface.cpp inverter.cpp report.cpp \
		main.cpp request.cpp response.cpp utils.cpp \
		device.cpp sensor.cpp iterator.cpp

COMM_SRC	= comm.cpp
SIMIFCEASY	= simIFCEasy.cpp

# Two Inverters, One Sensor Card
SIMSOLARNET2I1S	= simSolarNet2i1s.cpp
# One Inverter, Zero Sensor Cards
SIMSOLARNET1I0S	= simSolarNet1i0s.cpp
# Zero Inverters, Zero Sensor Cards
SIMSOLARNET0I0S	= simSolarNet0i0s.cpp

# RS-422, 1 IG-TL inverter
SIM422IGTL	= sim422IGTL.cpp


FSLURP_SRC		= $(COMMON_SRC) $(COMM_SRC)
SIMIFCEASY_SRC		= $(COMMON_SRC) $(SIMIFCEASY)
SIMSOLARNET2I1S_SRC	= $(COMMON_SRC) $(SIMSOLARNET2I1S)
SIMSOLARNET1I0S_SRC	= $(COMMON_SRC) $(SIMSOLARNET1I0S)
SIMSOLARNET0I0S_SRC	= $(COMMON_SRC) $(SIMSOLARNET0I0S)
SIM422IGTL_SRC		= $(COMMON_SRC) $(SIM422IGTL)

#FSLURP_OBJS		= $(FSLURP_SRC:.cpp=.o)
#SIMIFCEASY_OBJS	= $(SIMIFCEASY_SRC:.cpp=.o)
#SIMSOLARNET2I1S_OBJS	= $(SIMSOLARNET2I1S_SRC:.cpp=.o)
#SIMSOLARNET1I0S_OBJS	= $(SIMSOLARNET1I0S_SRC:.cpp=.o)
#SIMSOLARNET0I0S_OBJS	= $(SIMSOLARNET0I0S_SRC:.cpp=.o)

BUILD_DIR		= build
FSLURP_OBJS		= $(FSLURP_SRC:%.cpp=$(BUILD_DIR)/%.o)
SIMIFCEASY_OBJS		= $(SIMIFCEASY_SRC:%.cpp=$(BUILD_DIR)/%.o)
SIMSOLARNET2I1S_OBJS	= $(SIMSOLARNET2I1S_SRC:%.cpp=$(BUILD_DIR)/%.o)
SIMSOLARNET1I0S_OBJS	= $(SIMSOLARNET1I0S_SRC:%.cpp=$(BUILD_DIR)/%.o)
SIMSOLARNET0I0S_OBJS	= $(SIMSOLARNET0I0S_SRC:%.cpp=$(BUILD_DIR)/%.o)
SIM422IGTL_OBJS		= $(SIM422IGTL_SRC:%.cpp=$(BUILD_DIR)/%.o)

INCLUDES	= fslurp.h fronius.h
SIMINCLUDES	= simulator.h

DEBUGFLAGS	= -g
CFLAGS		= -c -Wall $(DEBUGFLAGS)

LDFLAGS		= -lm

VERSION		:= fslurp-$(shell tail -1 version)

all:		$(TARGETS)

fslurp:		$(FSLURP_OBJS)
		$(CC) $(LDFLAGS) $^ -o $@

simSolarNet2i1s: \
		$(SIMSOLARNET2I1S_OBJS)
		$(CC) $(LDFLAGS) $^ -o $@

simSolarNet1i0s: \
		$(SIMSOLARNET1I0S_OBJS)
		$(CC) $(LDFLAGS) $^ -o $@

simSolarNet0i0s: \
		$(SIMSOLARNET0I0S_OBJS)
		$(CC) $(LDFLAGS) $^ -o $@

simIFCEasy:	$(SIMIFCEASY_OBJS)
		$(CC) $(LDFLAGS) $^ -o $@

sim422IGTL:	$(SIM422IGTL_OBJS)
		$(CC) $(LDFLAGS) $^ -o $@

test:		$(TARGETS) clean
		./testcase.sh 

SampleOutput:	simSolarNet2i1s
		./$^

release:	clobber
		mkdir ../$(VERSION)
		cp -a * ../$(VERSION)
		cd ..; tar zcvf $(VERSION).tgz --exclude=RCS $(VERSION)

clean:
		rm -f test.log test.output
		rm -rf $(BUILD_DIR)
	
clobber:	clean
		rm -f $(TARGETS) test.log

$(BUILD_DIR)/%.o: %.cpp
		mkdir -p $(dir $@)
		$(CC) $(CFLAGS) $< -o $@
