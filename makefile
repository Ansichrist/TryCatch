########################### VARIABLES ############################

TARGET      = TRY_CATCH
COMPILER    = gcc
C_RELEASE   = -c -O3 -Wall
C_DEBUG     = -ggdb -c -Wall
C_FLAGS     = $(C_RELEASE)
C_FLAGS     = $(C_DEBUG)
LD_FLAGS    =
SOURCES     =
OBJECTS     =
OBJECTS_ADD =

############################ FOLDERS #############################

FLD_SRC  = ./

##################################################################

C_FLAGS += -I$(FLD_SRC)

##################################################################

LD_FLAGS += -pthread

##################################################################

SOURCES += main.c

##################################################################

OBJECTS += $(SOURCES:.c=.o)

##################################################################

all: $(SOURCES) $(TARGET) 

##################################################################

$(TARGET): $(OBJECTS) $(OBJECTS_ADD)
	$(COMPILER) $(LD_FLAGS) $(OBJECTS) $(OBJECTS_ADD) -o $(TARGET)

.c.o:
	$(COMPILER) $(C_FLAGS) $< -o $@

##################################################################

rmo:
	rm $(OBJECTS)

clean:
	rm $(OBJECTS) \
	   $(TARGET)

