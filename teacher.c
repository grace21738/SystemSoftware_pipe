#include <stdio.h>

#define ERR    (-1)             /* indicates an error condition */
#define READ   0                /* read end of a pipe */
#define WRITE  1                /* write end of a pipe */
#define STDIN  0                /* file descriptor of standard in */
#define STDOUT 1                /* file descriptor of standard out */

int main()
{
  int pid_1,               /* will be process id of first child - who */
      pid_2,               /* will be process id of second child - wc */
      pfd[2];              /* pipe file descriptor table.             */

  if ( pipe ( pfd ) == ERR )              /* create a pipe  */
    {                                 /* must do before a fork */
      perror (" ");
      exit (ERR);
    }
  if (( pid_1 = fork () ) == ERR)        /* create 1st child   */
  {
      perror (" ");
      exit (ERR);
  }
  if ( pid_1 != 0 )                      /* in parent  */
    {
        printf("\npdf[0]=%d, pdf[1]=%d,\n",pfd[0],pfd[1]);
      if (( pid_2 = fork () ) == ERR){     /* create 2nd child  */
        perror (" ");
        exit (ERR);
      }
      if ( pid_2 != 0 ){                 /* still in parent  */
          close ( pfd [READ] );         /* close pipe in parent */
          close ( pfd [WRITE] );        /* conserve file descriptors *
  /
          wait (( int * ) 0);           /* wait for children to die */
          wait (( int * ) 0);
      }
        else {                                /* in 2nd child   */
          close (STDIN);           /* close standard input */
            dup ( pfd [READ] );      /* read end of pipe becomes stdin *
/
            close ( pfd [READ] );            /* close unneeded I/O  */
            close ( pfd [WRITE] );           /* close unneeded I/O   */
            execl ("/usr/bin/wc", "wc",  (char *) NULL);
        }
    }
  else                                      /* in 1st child   */
    {
        close (STDOUT);            /* close standard out */
        dup ( pfd [WRITE] );       /* write end of pipes becomes stdout */
        close ( pfd [READ] );                 /* close unneeded I/O */
        close ( pfd [WRITE] );                /* close unneeded I/O */
        execl ("/usr/bin/who", "who",(char *) NULL);
    }
  exit (0);
}
if (( pid_3 = fork () ) == ERR){     /* create 3rd child  */
          perror (" ");
          exit (ERR);
        }
        if ( pid_3 != 0 ){          /* still in parent  */
          close ( pfd_1 [READ] );         /* close pipe in parent */
            close ( pfd_1 [WRITE] );        /* conserve file descriptors */
          close ( pfd_2 [READ] );         /* close pipe in parent */
            close ( pfd_2 [WRITE] );        /* conserve file descriptors */
          wait (( int * ) 0);           /* wait for children to die */
            wait (( int * ) 0);
            wait (( int * ) 0);
        }
        else{              /* in 3rd child   */
          close (STDIN);           /* close standard input */
              dup ( pfd_2 [READ] );      /* read end of pipe becomes stdin */
          close ( pfd_1 [READ] );            /* close unneeded I/O  */
              close ( pfd_1 [WRITE] );           /* close unneeded I/O   */
              close ( pfd_2 [READ] );            /* close unneeded I/O  */
              close ( pfd_2 [WRITE] );           /* close unneeded I/O   */
          execl ("/usr/bin/wc", "wc", (char *) NULL);
        }
