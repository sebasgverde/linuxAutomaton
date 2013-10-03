#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>


int i;

int funcionUno(int a)
{
		printf("hola soy hijo y mi pid es: %d\n", getpid());
		a= a+3;
		//sleep(1);
		return a;

}

int funcionDos(int a)
{
		a = a + 48;
		return a;

}

int main()
{
	/*if(fork() == 0)
	{
		primerHijo();
	}
	else if(fork() == 0)
		{
			segundoHijo();
		}

		else
		{
			
			int sta;
			wait(&sta);
			wait(&sta);
		}*/
	int sta;
	int pid;
	printf("hola soy padre e i vale: %d\n\n", i);
	for(i = 0; i < 6; i++)
	{
		pid = fork();
		if(pid == -1)
			return;
		if(pid == 0)
		{
		int temp = funcionUno(i);
		int otro = funcionDos(temp);
		printf("retornado %d con pid %d\n", temp, getpid());
		printf("ascii %d con pid %d es: %d\n\n", temp, getpid(),otro);
		//sleep(1);
		return;/*quedara en la historia que me tarde mas de 3 horas
		en darme cuenta que me creaba mas de 300 procesos por que no me 
		di cuenta de que la funcion volvia al main y por ende
		hacia todo el ciclo de nuevo. Tenia que poner este return luego
		de llamar la funcion*/
		}
sleep(1);
	}
}