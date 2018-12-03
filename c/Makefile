CFLAGS=-MMD -MP -Wall

PROGRAMS=albumlister
OBJECTS=album.o track.o

ALLOBJECTS=$(OBJECTS) $(patsubst %,%.o,$(PROGRAMS))

default: $(PROGRAMS)

albumlister: $(OBJECTS)

clean:
	-rm -f $(ALLOBJECTS) $(ALLOBJECTS:%.o=%.d) $(PROGRAMS)

-include $(ALLOBJECTS:%.o=%.d)
