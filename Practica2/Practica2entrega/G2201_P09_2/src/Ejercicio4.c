/**
 * @brief Ejercicio 4
 *
 * @file Ejercicio4.c
 * @author Miguel Angel Sanchez y Juan Velasco
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define NUM_PROC 4

/**
 * @brief manejador_SIGUSR1
 *
 * Hace un return en el proceso que efectua este manejador
 *
 * @return void
 */

void manejador_SIGUSR1(int sig){
    return;
}

/**
 * @brief manejador_SIGUSR2
 *
 * Termina el proceso que efectua este manejador
 *
 * @return void
 */

void manejador_SIGUSR2(int sig){
    exit (EXIT_SUCCESS);
}

/**
 * @brief Main
 *
 * El main crea los hijos, que imprimen por pantalla "estoy trabajando", y son estos
 * hijos los que mandan las senales SIGUSR1 y SIGUSR2
 *
 * @return EXIT_SUCCESS o EXIT_FAILURE
 */

int main(){
  int pid[NUM_PROC];
  int i,j;
  if (signal(SIGUSR1,manejador_SIGUSR1) == SIG_ERR){
    perror("signal");
    exit (EXIT_FAILURE);
  }
  if (signal(SIGUSR2,manejador_SIGUSR2) == SIG_ERR){
    perror("signal");
    exit (EXIT_FAILURE);
  }
  for (i = 0; i < NUM_PROC; i++){
    pid[i] = fork();
    if (pid[i] < 0){
      return EXIT_FAILURE;
    }
    /*Esto es lo que hace el primer hijo*/
    if (pid[i] == 0 && i == 0){
      for (j = 0; j < 10; j++){
        printf("Soy %d y estoy trabajando\n",getpid());
        sleep(1);
      }
      kill(getppid(),SIGUSR1);
      while(1){
        printf("Soy %d y estoy trabajando\n",getpid());
        sleep(1);
      }
    /*A partir de el segundo hijo matan al hijo anterior*/
    }else if(pid[i] == 0 && i != 0 && i != NUM_PROC-1){
      kill(pid[i-1],SIGUSR2);
      for (j = 0; j < 10; j++){
        printf("Soy %d y estoy trabajando\n",getpid());
        sleep(1);
      }
      kill(getppid(),SIGUSR1);
      while(1){
        printf("Soy %d y estoy trabajando\n",getpid());
        sleep(1);
      }
    /*El ultimo hijo no se queda esperando*/
    }else if(pid[i]==0 && i == NUM_PROC-1){
      kill(pid[i-1],SIGUSR2);
      for (j = 0; j < 10; j++){
        printf("Soy %d y estoy trabajando\n",getpid());
        sleep(1);
      }
      kill(getppid(),SIGUSR1);
      exit(EXIT_SUCCESS);
    }
    else{
      pause();
    }
  }
  return EXIT_SUCCESS;
}
