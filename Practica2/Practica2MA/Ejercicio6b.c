/**
 * @brief Ejercicio 6b
 *
 * @file Ejercicio6b.c
 * @author Miguel Angel Sanchez y Juan Velasco
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#define NUM_PROC 5

/**
 * @brief manejador_SIGTERM
 *
 * Es el manejador de la senal SIGTERM, que la asocia a una impresion por pantalla y 
 * un exit(0).
 *
 * @return 
 */

void manejador_SIGTERM(int sig){
	printf("Soy %d y he recibido la senal SIGTERM\n",getpid());
	exit(0);
}

/**
 * @brief Main
 *
 * El main crea un proceso hijo que pone a trabajar y hace que el padre espere 40 segs.
 * Cuando han pasado estos segundos, el padre manda una senal al hijo para que termine.
 *
 * @return 
 */

int main (void)
{
	int pid, counter;

	if (signal(SIGTERM,manejador_SIGTERM) == SIG_ERR){
		perror("signal");
		exit (EXIT_FAILURE);
	}

	pid = fork();
	if (pid == 0){
		while(1){
			for (counter = 0; counter < NUM_PROC; counter++){
				printf("%d\n", counter);
				sleep(1);
			}
			sleep(3);
		}
	}
	else{
		/*El padre envia la senal SIGTERM al hijo 40 segs despues de crearlo*/
		sleep(40);
		kill(pid,SIGTERM);
	}
	while(wait(NULL)>0);
}




