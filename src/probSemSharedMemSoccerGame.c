/**
 *  \file probSemSharedMemSoccerGame.c (implementation file)
 *
 *  \brief Problem name: SoccerGame
 *
 *  Synchronization based on semaphores and shared memory.
 *  Implementation with SVIPC.
 *
 *  Generator process of the intervening entities.
 *
 *  Upon execution, one parameter is requested:
 *    \li name of the logging file.
 *
 *  \author Nuno Lau - January 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <math.h>

#include "probConst.h"
#include "probDataStruct.h"
#include "logging.h"
#include "sharedDataSync.h"
#include "semaphore.h"
#include "sharedMemory.h"

/** \brief name of player program */
#define   PLAYER               "./player"

/** \brief name of goalie program */
#define   GOALIE               "./goalie"

/** \brief name of referee program */
#define   REFEREE              "./referee"

void launch_processes(char *bin, char *prefix, int nProc, char *logFilename, int *pids)
{
    char idstr[3];
    char errorFilename[128];
    int p;
    for (p = 0; p < nProc; p++) {           
        if ((pids[p] = fork ()) < 0) {
            perror ("error on the fork operation");
            exit (EXIT_FAILURE);
        }
        sprintf(idstr,"%d", p);
        sprintf(errorFilename,"error_%s%02d", prefix, p); 
        if (pids[p] == 0)
            if (execl (bin, bin, idstr, logFilename, errorFilename, NULL) < 0) { 
                perror ("error on the generation of the process");
                exit (EXIT_FAILURE);
            }
    }
}



/**
 *  \brief Main program.
 *
 *  Its role is starting the simulation by generating the intervening entities processes (players, goalies and referee)
 *  and waiting for their termination.
 */
int main (int argc, char *argv[])
{
    char nFic[51];                                                                              /*name of logging file */
    int shmid,                                                                      /* shared memory access identifier */
        semgid;                                                                     /* semaphore set access identifier */
    unsigned int  m;                                                                             /* counting variables */
    SHARED_DATA *sh;                                                                /* pointer to shared memory region */
    int pidPL[NUMPLAYERS],                                                         /* players process identifier array */
        pidGL[NUMGOALIES],                                                         /* goalies process identifier array */
        pidRF;                                                                           /* referee process identifier */
    int key;                                                           /*access key to shared memory and semaphore set */
    int status,                                                                                    /* execution status */
        info;                                                                                               /* info id */

    /* getting log file name */
    if(argc==2) {
        strcpy(nFic, argv[1]);
    }
    else strcpy(nFic, "");

    /* getting key value */
    if ((key = ftok (".", 'a')) == -1) {
        perror ("error on generating the key");
        exit (EXIT_FAILURE);
    }

    /* creating and initializing the shared memory region and the log file */
    if ((shmid = shmemCreate (key, sizeof (SHARED_DATA))) == -1) { 
        perror ("error on creating the shared memory region");
        exit (EXIT_FAILURE);
    }
    if (shmemAttach (shmid, (void **) &sh) == -1) { 
        perror ("error on mapping the shared region on the process address space");
        exit (EXIT_FAILURE);
    }

    /* initialize random generator */
    srandom ((unsigned int) getpid ());                                

    /* initialize problem internal status */
    int p;
    for (p = 0; p < NUMPLAYERS; p++) {
        sh->fSt.st.playerStat[p]        = ARRIVING;                            /* the players are arriving */
    }
    int g;
    for (g = 0; g < NUMGOALIES; g++) {
        sh->fSt.st.goalieStat[p]        = ARRIVING;                            /* the goalies are arriving */
    }
    sh->fSt.st.refereeStat = ARRIVING;                                               /*referee is arriving*/
    
    sh->fSt.nPlayers         = NUMPLAYERS;                                              
    sh->fSt.nGoalies         = NUMGOALIES;
    sh->fSt.playersArrived   = 0;                                             
    sh->fSt.goaliesArrived   = 0;                                             
    sh->fSt.playersFree      = 0;                                             
    sh->fSt.goaliesFree      = 0;                                             
    sh->fSt.teamId           = 1;                                             

    /* create log file */
    createLog (nFic, &sh->fSt);                                  
    saveState(nFic,&sh->fSt);

    /* initialize semaphore ids */
    sh->mutex                       = MUTEX;                                /* mutual exclusion semaphore id */
    sh->playersWaitTeam             = PLAYERSWAITTEAM;
    sh->goaliesWaitTeam             = GOALIESWAITTEAM;
    sh->playersWaitReferee          = PLAYERSWAITREFEREE;
    sh->playersWaitEnd              = PLAYERSWAITEND;
    sh->refereeWaitTeams            = REFEREEWAITTEAMS;
    sh->playerRegistered            = PLAYERREGISTERED;
 
     /* creating and initializing the semaphore set */
    if ((semgid = semCreate (key, SEM_NU)) == -1) { 
        perror ("error on creating the semaphore set");
        exit (EXIT_FAILURE);
    }
    if (semUp (semgid, sh->mutex) == -1) {                             /* enabling access to critical region */
        perror ("error on executing the up operation for semaphore access");
        exit (EXIT_FAILURE);
    }

    /* generation of intervening entities processes */                            
    /* player processes */
    launch_processes(PLAYER, "PL", NUMPLAYERS, nFic, pidPL);

    /* goalie processes */
    launch_processes(GOALIE, "GL", NUMGOALIES, nFic, pidGL);

    /* smoker processes */
    launch_processes(REFEREE, "RF", 1, nFic, &pidRF);


    /* signaling start of operations */
    if (semSignal (semgid) == -1) {
        perror ("error on signaling start of operations");
        exit (EXIT_FAILURE);
    }

    /* waiting for the termination of the intervening entities processes */
    m = 0;
    do {
        info = wait (&status);
        if (info == -1) { 
            perror ("error on aiting for an intervening process");
            exit (EXIT_FAILURE);
        }
        m += 1;
    } while (m < 1 + NUMPLAYERS + NUMGOALIES);

    /* destruction of semaphore set and shared region */
    if (semDestroy (semgid) == -1) {
        perror ("error on destructing the semaphore set");
        exit (EXIT_FAILURE);
    }
    if (shmemDettach (sh) == -1) { 
        perror ("error on unmapping the shared region off the process address space");
        exit (EXIT_FAILURE);
    }
    if (shmemDestroy (shmid) == -1) { 
        perror ("error on destructing the shared region");
        exit (EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
