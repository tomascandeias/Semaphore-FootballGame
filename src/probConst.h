/**
 *  \file probConst.h (interface file)
 *
 *  \brief Problem name: SoccerGame
 *
 *  Problem simulation parameters.
 *
 *  \author Nuno Lau - January 2021
 */

#ifndef PROBCONST_H_
#define PROBCONST_H_

/* Generic parameters */
 
/** \brief total number of players */
#define  NUMPLAYERS       10
/** \brief total number of goalies */
#define  NUMGOALIES        3
/** \brief total number of referees */
#define  NUMREFEREES       1

/** \brief number of players in each team */
#define  NUMTEAMPLAYERS     4
/** \brief number of goalies in teach team */
#define  NUMTEAMGOALIES     1


/* Player/Goalie state constants */

/** \brief player/goalie initial state, arriving */
#define  ARRIVING          0
/** \brief player/goalie waiting to constitute team */
#define  WAITING_TEAM      1
/** \brief player/goalie waiting to constitute team */
#define  FORMING_TEAM      2
/** \brief player/goalie waiting for referee to start game in team 1 */
#define  WAITING_START_1   3
/** \brief player/goalie waiting for referee to start game in team 2 */
#define  WAITING_START_2   4
/** \brief player/goalie playing in team 1 */
#define  PLAYING_1         5
/** \brief player/goalie playing in team 2 */
#define  PLAYING_2         6
/** \brief player/goalie playing */
#define  LATE              7

/* Referee state constants */

/** \brief referee initial state, arriving  */
#define  ARRIVING          0
/** \brief referee waiting for both teams  */
#define  WAITING_TEAMS     1
/** \brief referee starting game  */
#define  STARTING_GAME     2
/** \brief referee refereeing  */
#define  REFEREEING        3
/** \brief referee ending game  */
#define  ENDING_GAME       4


#endif /* PROBCONST_H_ */
