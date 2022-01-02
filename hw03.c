#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define ERR    (-1)             /* indicates an error condition */
#define READ   0                /* read end of a pipe */
#define WRITE  1                /* write end of a pipe */
#define STDIN  0                /* file descriptor of standard in */
#define STDOUT 1                /* file descriptor of standard out */

//ps aux|grep cn2021|wc
int main()
{
	int pid_1,               /* will be process id of first child - who */
	    pid_2,               /* will be process id of second child - wc */
	    pid_3,
	    pid_4,
	    pfd_1[2],           /* pipe file descriptor table.             */
		pfd_2[2];

	int fd;
	fd = open("the_result",O_WRONLY | O_CREAT, S_IREAD | S_IWRITE);

	//Create two pipe
	if ( pipe ( pfd_1 ) == ERR )             
    {                                 
	    perror (" ");
	    exit (ERR);
    }

    if ( pipe ( pfd_2 ) == ERR )              /* create a pipe  */
    {                                 /* must do before a fork */
	    perror (" ");
	    exit (ERR);
    }

    //ps aux and grep
	if (( pid_1 = fork () ) == ERR)        /* create 1st child   */
	{
	    perror (" ");
	    exit (ERR);
	}
	if ( pid_1 != 0 )                      /* in parent  */
    {
      	//printf("pdf_1[0]=%d, pdf_1[1]=%d,pdf_2[0]=%d, pdf_2[1]=%d,\n",pfd_1[0],pfd_1[1],pfd_2[0],pfd_2[1]);
	    if (( pid_2 = fork () ) == ERR){     /* create 2nd child  */
		    perror (" ");
		    exit (ERR);
	    }
	    if ( pid_2 != 0 ){                /* still in parent  */
	    	
	    	if (( pid_3 = fork () ) == ERR){     /* create 3rd child  */
	          perror (" ");
	          exit (ERR);
	        }
	        if ( pid_3 != 0 ){          /* still in parent  */
		        close ( pfd_1 [READ] );         /* close pipe in parent */
		        close ( pfd_1 [WRITE] );        /* conserve file descriptors */
		        close ( pfd_2 [READ] );         /* close pipe in parent */
		        close ( pfd_2 [WRITE] );        /* conserve file descriptors */
		        system("ps aux|grep cn2021|wc");
		        wait (( int * ) 0);           /* wait for children to die */
		        wait (( int * ) 0);
		        wait (( int * ) 0);
	        }
	        else{              /* in 3rd child   */
	          	close (STDIN);           /* close standard input */
	          	dup ( pfd_2 [READ] );      /* read end of pipe becomes stdin */
	          	close (STDOUT);
	          	dup ( fd );
	          	close(fd);
	          	close ( pfd_1 [READ] );            /* close unneeded I/O  */
	            close ( pfd_1 [WRITE] );           /* close unneeded I/O   */
	            close ( pfd_2 [READ] );            /* close unneeded I/O  */
	            close ( pfd_2 [WRITE] );           /* close unneeded I/O   */
	          	execl ("/usr/bin/wc", "wc", (char *) NULL);
	        }
	    }
      	else {                                /* in 2nd child   */
        	close (STDIN);           /* close standard input */
            dup ( pfd_1 [READ] );      /* read end of pipe becomes stdin */
            close (STDOUT);            /* close standard out */
		    dup ( pfd_2 [WRITE] );       /* write end of pipes becomes stdout */
            close ( pfd_1 [READ] );            /* close unneeded I/O  */
            close ( pfd_1 [WRITE] );           /* close unneeded I/O   */
	    	close ( pfd_2 [READ] );         /* close pipe in parent */
	    	close ( pfd_2 [WRITE] );        /* conserve file descriptors */
            execl ("/usr/bin/grep", "grep", "cn2021",(char *) NULL);
        }
    }
	else                                      /* in 1st child   */
    {
	    close (STDOUT);            /* close standard out */
	    dup ( pfd_1 [WRITE] );       /* write end of pipes becomes stdout */
	    close ( pfd_1 [READ] );                 /* close unneeded I/O */
	    close ( pfd_1 [WRITE] );                /* close unneeded I/O */
    	close ( pfd_2 [READ] );         /* close pipe in parent */
	    close ( pfd_2 [WRITE] );        /* conserve file descriptors */
	    execl ("/usr/bin/ps", "ps", "aux",(char *) NULL);
    }
	exit (0);
}
