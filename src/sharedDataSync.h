/**
 *  \file sharedDataSync.h (interface file)
 *
 *  \brief Problem name: SoccerGame
 *
 *  Synchronization based on semaphores and shared memory.
 *  Implementation with SVIPC.
 *
 *  \brief Definition of the shared data and the synchronization devices.
 *
 *  Both the format of the shared data, which represents the full state of the problem, and the identification of
 *  the different semaphores, which carry out the synchronization among the intervening entities, are provided.
 *
 *  \author Nuno Lau - January 2021
 */

#ifndef SHAREDDATASYNC_H_
#define SHAREDDATASYNC_H_

#include "probConst.h"
#include "probDataStruct.h"

/**
 *  \brief Definition of <em>shared information</em> data type.
 */
typedef struct
        { /** \brief full state of the problem */
          FULL_STAT fSt;

          /* semaphores ids */
          /** \brief identification of critical region protection semaphore – val = 1 */
          unsigned int mutex;
          /** \brief identification of semaphore used by players to wait for forming team teammate - val = 0 */
          unsigned int playersWaitTeam;
          /** \brief identification of semaphore used by goalies to wait for forming team teammate - val = 0 */
          unsigned int goaliesWaitTeam;
          /** \brief identification of semaphore used by players and goalies to wait for the match to start - val = 0 */
          unsigned int playersWaitReferee;
          /** \brief identification of semaphore used by players and goalies to wait for the match to end - val = 0 */
          unsigned int playersWaitEnd;
          /** \brief identification of semaphore used by referee to wait for teams to be formed – val = 0  */
          unsigned int refereeWaitTeams;
          /** \brief identification of semaphore used by players and goalies to acknowledge team registration – val = 0  */
          unsigned int playerRegistered;

        } SHARED_DATA;

/** \brief number of semaphores in the set */
#define SEM_NU                   7 

#define MUTEX                    1
#define PLAYERSWAITTEAM          2
#define GOALIESWAITTEAM          3
#define PLAYERSWAITREFEREE       4
#define PLAYERSWAITEND           5
#define REFEREEWAITTEAMS         6
#define PLAYERREGISTERED         7

#endif /* SHAREDDATASYNC_H_ */
