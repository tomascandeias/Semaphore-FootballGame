CC = gcc
CFLAGS = -Wall -g

SUFFIX = $(shell getconf LONG_BIT)

PLAYER    = semSharedMemPlayer
GOALIE    = semSharedMemGoalie
REFEREE   = semSharedMemReferee
MAIN      = probSemSharedMemSoccerGame

OBJS = sharedMemory.o semaphore.o logging.o

.PHONY: all pl gl rf all_bin clean cleanall

all:     clean  player      goalie       referee      main  
pl:	     clean  player      goalie_bin   referee_bin  main 
gl:	     clean  player_bin  goalie       referee_bin  main
rf:	     clean  player_bin  goalie_bin   referee      main 
all_bin: clean  player_bin  goalie_bin   referee_bin  main 

player:	 $(PLAYER).o $(OBJS)
	$(CC) -o ../run/$@ $^ -lm

goalie:	 $(GOALIE).o $(OBJS)
	$(CC) -o ../run/$@ $^

referee: $(REFEREE).o $(OBJS)
	$(CC) -o ../run/$@ $^ -lm

main:    $(MAIN).o $(OBJS)
	$(CC) -o ../run/$(MAIN) $^ -lm

player_bin:
	cp ../run/player_bin_$(SUFFIX) ../run/player

goalie_bin:
	cp ../run/goalie_bin_$(SUFFIX) ../run/goalie

referee_bin:
	cp ../run/referee_bin_$(SUFFIX) ../run/referee

clean:
	rm -f *.o

cleanall: clean
	rm -f ../run/$(MAIN) ../run/player ../run/goalie ../run/referee ../run/error_*

