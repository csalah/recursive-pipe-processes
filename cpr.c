#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>


char **arguments;


/* Prototype */
void creerEnfantEtLire(int);

/*-------------------------------------------------------------
Function: main
Arguments:

int ac – number of command line arguments
char **av – array of pointers to the command line arguments

Parses the number of child processes to create from the command-line input. If an error occurs, the program exits. Calls the creerEnfantEtLire function to create a child process and read the data sent from it.
-------------------------------------------------------------*/

int main(int ac, char **av)
{
    int numeroProcessus; 

    if(ac == 2)
    {
       if(sscanf(av[1],"%d",&numeroProcessus) == 1)
       {   
		   arguments = av;
           creerEnfantEtLire(numeroProcessus);
       }
       else fprintf(stderr,"Ne peut pas traduire argument\n");
    }
    else fprintf(stderr,"Arguments pas valide\n");
    return(0);
}


/*-------------------------------------------------------------
Function: creerEnfantEtLire
Arguments:
int prcNum – the process number

Creates a child process passing prcNum - 1 to it. The current process uses prcNum as its identifier. It also reads messages from the read end of the pipe and sends them to standard output (file descriptor 1). When no more data can be read from the pipe the process terminates.
-------------------------------------------------------------*/

void creerEnfantEtLire(int prcNum)
{

    int status;
	int fd[2]; //descripteurs de fichier fd[0] (lecture) et fd[1] (écriture)
	int pid;

	char *buff = (char*)malloc(45 * sizeof(char)); 

	status = pipe(fd); //tuyaux

    if (status == -1) 
	{
		fprintf(stderr, "Tuyaux échoué.\n");
		exit(-1);
	}


	if (prcNum < 1)
	{
		fprintf(stderr, "Veuillez entrer un nombre positif.\n");
		fflush(stdout);
		exit(-1);
	} 

	else if (prcNum == 1)
	{
		sprintf(buff, "Processus %d commence\n", prcNum);
		write(1, buff, strlen(buff)); 

		sleep(5);

		sprintf(buff, "Processus %d termine\n", prcNum);
		write(1, buff, strlen(buff));

		sleep(10); 
	} 

	else 
	{ 
		sprintf(buff, "Processus %d commence\n", prcNum);
		write(1, buff, strlen(buff));

		pid = fork();


		if (pid < 0)
		{
			fprintf(stderr, "Erreur de fork.\n");
			exit(-1);
		}

		if (pid == 0) //processus enfant
		{ 
			close(fd[1]); 
			dup2(fd[1], 1); 

			char num[20]; 

			sprintf(num, "%d", prcNum-1);
			execlp(arguments[0], arguments[0], num, NULL);
		} 
		
		else //processus parent
		{ 
			close(fd[1]);

			memset(buff, 0, sizeof(buff)); 
			read(fd[0], buff,strlen(buff));
			write(1, buff, strlen(buff));

			wait(NULL);

			memset(buff, 0, sizeof(buff));
			sprintf(buff, "Processus %d termine\n", prcNum);
			write(1, buff, strlen(buff));

			sleep(10);
		}

	}

}
