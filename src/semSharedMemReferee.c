/**
 *  \file semSharedMemAgent.c (implementation file)
 *
 *  \brief Problem name: SoccerGame
 *
 *  Synchronization based on semaphores and shared memory.
 *  Implementation with SVIPC.
 *
 *  Definition of the operations carried out by the referee:
 *     \li arrive
 *     \li waitForTeams
 *     \li startGame
 *     \li play
 *     \li endgame
 *
 *  \author Nuno Lau - January 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>

#include "probConst.h"
#include "probDataStruct.h"
#include "logging.h"
#include "sharedDataSync.h"
#include "semaphore.h"
#include "sharedMemory.h"


/** \brief logging file name */
static char nFic[51];

/** \brief shared memory block access identifier */
static int shmid;

/** \brief semaphore set access identifier */
static int semgid;

/** \brief pointer to shared memory region */
static SHARED_DATA *sh;

/** \brief referee takes some time to arrive */
static void arrive ();

/** \brief referee waits for teams to be formed */
static void waitForTeams ();

/** \brief referee starts game */
static void startGame ();

/** \brief referee takes some time to allow game to finish */
static void play ();

/** \brief referee ends game */
static void endGame ();

/**
 *  \brief Main program.
 *
 *  Its role is to generate the life cycle of one of intervening entities in the problem: the referee.
 */
int main (int argc, char *argv[])
{
    int key;                                          /*access key to shared memory and semaphore set */

    /* validation of command line parameters */
    if (argc != 4) { 
        freopen ("error_RF", "a", stderr);
        fprintf (stderr, "Number of parameters is incorrect!\n");
        return EXIT_FAILURE;
    }

    /* argv[1] may be ignored as there is only one referee - id=0 */

    /* get logfile name - argv[2]*/
    strcpy (nFic, argv[2]);

    /* redirect stderr to error file  - argv[3]*/
    freopen (argv[3], "w", stderr);
    setbuf(stderr,NULL);

    /* getting key value */
    if ((key = ftok (".", 'a')) == -1) {
        perror ("error on generating the key");
        exit (EXIT_FAILURE);
    }

    /* connection to the semaphore set and the shared memory region and mapping the shared region onto the
       process address space */
    if ((semgid = semConnect (key)) == -1) { 
        perror ("error on connecting to the semaphore set");
        return EXIT_FAILURE;
    }
    if ((shmid = shmemConnect (key)) == -1) { 
        perror ("error on connecting to the shared memory region");
        return EXIT_FAILURE;
    }
    if (shmemAttach (shmid, (void **) &sh) == -1) { 
        perror ("error on mapping the shared region on the process address space");
        return EXIT_FAILURE;
    }

    /* initialize random generator */
    srandom ((unsigned int) getpid ());                                      

    /* simulation of the life cycle of the referee */
    arrive();
    waitForTeams();
    startGame();
    play();
    endGame();

    /* unmapping the shared region off the process address space */
    if (shmemDettach (sh) == -1) { 
        perror ("error on unmapping the shared region off the process address space");
        return EXIT_FAILURE;;
    }

    return EXIT_SUCCESS;
}

/**
 *  \brief referee takes some time to arrive
 *
 *  Referee updates state and takes some time to arrive
 *  The internal state should be saved.
 *
 */
static void arrive ()
{
    if (semDown (semgid, sh->mutex) == -1) {                                                      /* enter critical region */
        perror ("error on the up operation for semaphore access (RF)");
        exit (EXIT_FAILURE);
    }

    /* TODO: insert your code here */
    sh->fSt.st.refereeStat = ARRIVING;
    saveState(nFic, &sh->fSt);

    if (semUp (semgid, sh->mutex) == -1) {                                                        /* leave critical region */
        perror ("error on the up operation for semaphore access (RF)");
        exit (EXIT_FAILURE);
    }
    
    usleep((100.0*random())/(RAND_MAX+1.0)+10.0);
   
}

/**
 *  \brief referee waits for teams to be formed
 *
 *  Referee updates state and waits for the 2 teams to be completely formed
 *  The internal state should be saved.
 *
 */
static void waitForTeams ()
{
    if (semDown (semgid, sh->mutex) == -1) {                                                      /* enter critical region */
        perror ("error on the up operation for semaphore access (RF)");
        exit (EXIT_FAILURE);
    }

    /* TODO: insert your code here */
    // esperar pelo estado
    sh->fSt.st.refereeStat = WAITING_TEAMS;
    saveState(nFic, &sh->fSt);


    if (semUp (semgid, sh->mutex) == -1) {                                                        /* leave critical region */
        perror ("error on the up operation for semaphore access (RF)");
        exit (EXIT_FAILURE);
    }

    if (semDown(semgid, sh->refereeWaitTeams) == -1 ){
        perror ("error on the up operation for semaphore access (RF)");
        exit (EXIT_FAILURE);
    } 


}

/**
 *  \brief referee starts game
 *
 *  Referee updates state and notifies players and goalies to start match
 *  The internal state should be saved.
 *
 */
static void startGame ()
{
    if (semDown (semgid, sh->mutex) == -1) {                                                      /* enter critical region */
        perror ("error on the up operation for semaphore access (RF)");
        exit (EXIT_FAILURE);
    }

    
    sh->fSt.st.refereeStat = STARTING_GAME;
    saveState(nFic, &sh->fSt);

    if (semUp (semgid, sh->mutex) == -1) {                                                        /* leave critical region */
        perror ("error on the up operation for semaphore access (RF)");
        exit (EXIT_FAILURE);
    }

    /* TODO: insert your code here */
    for(int i = 0; i < NUMPLAYERS; i++){
        if (semUp (semgid, sh->playersWaitReferee) == -1) {                                                        /* leave critical region */
            perror ("error on the up operation for semaphore access (RF)");
            exit (EXIT_FAILURE);
        }
    }
    

}

/**
 *  \brief referee takes some time to allow game to finish
 *
 *  Referee updates state and takes some time to finish the game 
 *  The internal state should be saved.
 *
 */
static void play ()
{
    if (semDown (semgid, sh->mutex) == -1) {                                                      /* enter critical region */
        perror ("error on the up operation for semaphore access (RF)");
        exit (EXIT_FAILURE);
    }

    sh->fSt.st.refereeStat = REFEREEING;
    saveState(nFic, &sh->fSt);
    if (semUp (semgid, sh->mutex) == -1) {                                                        /* leave critical region */
        perror ("error on the up operation for semaphore access (RF)");
        exit (EXIT_FAILURE);
    }

    usleep((100.0*random())/(RAND_MAX+1.0)+900.0);
}

/**
 *  \brief referee ends game
 *
 *  Referee updates state and notifies players and goalies to end match
 *  The internal state should be saved.
 *
 */
static void endGame ()
{
    if (semDown (semgid, sh->mutex) == -1) {                                                      /* enter critical region */
        perror ("error on the up operation for semaphore access (RF)");
        exit (EXIT_FAILURE);
    }

    sh->fSt.st.refereeStat = ENDING_GAME;
    saveState(nFic, &sh->fSt);

    if (semUp (semgid, sh->mutex) == -1) {                                                        /* leave critical region */
        perror ("error on the up operation for semaphore access (RF)");
        exit (EXIT_FAILURE);
    }

    for(int i = 0; i < NUMPLAYERS; i++){
        if (semUp (semgid, sh->playersWaitEnd) == -1) {                                                        /* leave critical region */
            perror ("error on the up operation for semaphore access (RF)");
            exit (EXIT_FAILURE);
        }
    }

}
