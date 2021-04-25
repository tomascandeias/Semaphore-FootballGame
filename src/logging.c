/**
 *  \file logging.c (implementation file)
 *
 *  \brief Problem name: SoccerGame
 *
 *  \brief Logging the internal state of the problem into a file.
 *
 *  Defined operations:
 *     \li file initialization
 *     \li writing the present full state as a single line at the end of the file.
 *
 *  \author Nuno Lau - January 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <unistd.h>


#include "probConst.h"
#include "probDataStruct.h"

/* internal functions */

static FILE *openLog(char nFic[], char mode[])
{
    FILE *fic;
    char *fName;                                                                                      /* log file name */

    if ((nFic == NULL) || (strlen (nFic) == 0)) {
        return stdout;
    }
    else  {
        fName = nFic;
    }

    fprintf(stderr,"%d opening log %s %s\n",getpid(),nFic,mode);

    if ((fic = fopen (fName, mode)) == NULL) {
        perror ("error on opening log file");
        exit (EXIT_FAILURE);
    }
    return fic;
}

static void closeLog(FILE *fic)
{
    if(fic==stderr || fic == stdout) {
         fflush(fic);
         return;
    }

    if (fclose (fic) == EOF) {
        perror ("error on closing of log file");
        exit (EXIT_FAILURE);
    }
}

static void printHeader(FILE *fic, FULL_STAT *p_fSt)
{
    
    int p;
    for(p=0; p < p_fSt->nPlayers; p++) {
        fprintf(fic, " %s%02d", "P", p);
    }

    fprintf(fic," ");

    int g;
    for(g=0; g < p_fSt->nGoalies; g++) {
        fprintf(fic, " %s%02d", "G", g);
    }

    fprintf(fic," ");

    fprintf(fic," %s%02d","R",1);

    fprintf(fic," ");

    fprintf(fic,"\n");
}

/* external functions */

/**
 *  \brief File initialization.
 *
 *  The function creates the logging file and writes its header.
 *  If <tt>nFic</tt> is a null pointer or a null string, stdout is used.
 *
 *  The file header consists of
 *       \li a title line
 *       \li a blank line.
 *
 *  \param nFic name of the logging file
 */
void createLog (char nFic[], FULL_STAT *p_fSt)
{
    FILE *fic;                                                                                      /* file descriptor */

    fic = openLog(nFic,"w");

    /* title line + blank line */

    fprintf (fic, "%21cSoccerGame - Description of the internal state\n\n", ' ');
    printHeader(fic, p_fSt);

    closeLog(fic);
}

/**
 *  \brief Writing the present full state as a single line at the end of the file.
 *
 *  If <tt>nFic</tt> is a null pointer or a null string, the lines are written to stdout
 *
 *  The following layout is obeyed for the full state in a single line
 *    \li players state
 *    \li goalies state 
 *    \li referee state 
 *
 *  \param nFic name of the logging file
 *  \param p_fSt pointer to the location where the full internal state of the problem is stored
 */
void saveState (char nFic[], FULL_STAT *p_fSt)
{
    FILE *fic;                                                                                      /* file descriptor */

    fic = openLog(nFic,"a");

    int p;
    for(p=0; p < p_fSt->nPlayers; p++) {
        fprintf(fic,"%4u",p_fSt->st.playerStat[p]);
    }

    fprintf(fic," ");

    int g;
    for(g=0; g < p_fSt->nGoalies; g++) {
        fprintf(fic,"%4u",p_fSt->st.goalieStat[g]);
    }

    fprintf(fic," ");

    fprintf(fic,"%4u",p_fSt->st.refereeStat);

    fprintf(fic,"\n");

    closeLog(fic);
}

