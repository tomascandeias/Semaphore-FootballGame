/**
 *  \file probDataStruct.h (interface file)
 *
 *  \brief Problem name: SoccerGame
 *
 *  Definition of internal data structures.
 *
 *  They specify internal metadata about the status of the intervening entities.
 *
 *  \author Nuno Lau - January 2021
 */

#ifndef PROBDATASTRUCT_H_
#define PROBDATASTRUCT_H_

#include <stdbool.h>

#include "probConst.h"

/**
 *  \brief Definition of <em>state of the intervening entities</em> data type.
 */
typedef struct {
    /** \brief players state */
    unsigned int playerStat[NUMPLAYERS];
    /** \brief goalies state */
    unsigned int goalieStat[NUMGOALIES];
    /** \brief referees state */
    unsigned int refereeStat;

} STAT;


/**
 *  \brief Definition of <em>full state of the problem</em> data type.
 */
typedef struct
{   /** \brief state of all intervening entities */
    STAT st;

    /** \brief total number of players */
    int nPlayers;

    /** \brief total number of goalies */
    int nGoalies;

    /** \brief total number of referees */
    int nReferees;

    /** \brief number of players that already arrived */
    int playersArrived;
    /** \brief number of goalies that already arrived */
    int goaliesArrived;
    /** \brief number of players that arrived and are free (no team) */
    int playersFree;
    /** \brief number of goalies that arrived and are free (no team) */
    int goaliesFree;

    /** \brief id of team that will be formed next - initial value=1 */
    int teamId;

} FULL_STAT;


#endif /* PROBDATASTRUCT_H_ */
