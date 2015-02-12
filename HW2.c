#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct Process {
	int processName;
	float arrivalTime;
	float expectedTotalRunTime;
	int priority;
	int timeRemaining;
};
int ATcompare(const void * a, const void * b)
{
	
    const struct Process *elem1 = a;    
    const struct Process *elem2 = b;

   if ( elem1->arrivalTime < elem2->arrivalTime)
      return -1;
   else if (elem1->arrivalTime > elem2->arrivalTime)
      return 1;
   else
      return 0;
}
struct Process processFactory(int pName, float arrivalT, float expectedTotalrt, int priority, int tRemain)
{
	struct Process p;
	p = *(struct Process *) malloc (sizeof(struct Process));
	p.processName = pName;
	p.arrivalTime = arrivalT;
	p.expectedTotalRunTime = expectedTotalrt;
	p.priority = priority;
	p.timeRemaining = tRemain;
	return p;
}
int main(void) {

	int i;
	srand(time(NULL));
	struct Process * processes = malloc(50 * sizeof(struct Process));
	for (i = 0; i < 50; i++)
	{
		float runTime = ((float)rand() / (float)(RAND_MAX / 9.9)) + 0.1;
		processes[i] = processFactory(i, (float)rand() / (float)(RAND_MAX / 100), runTime , rand() % 4 + 1, ceil(runTime));
		//printf("%f ", processes[i].expectedTotalRunTime);
		// printf("%i ", processes[i].priority);
		// printf("%i\n", processes[i].timeRemaining);
	}
	qsort(processes, 50, sizeof(struct Process), ATcompare);
	int c;
	for(c = 0; c < 50; c++)
	{
		 printf("P%i ", processes[c].processName);
		 printf("%f ", processes[c].arrivalTime);
		 printf("%f\n", processes[c].expectedTotalRunTime);
	}
	int j;
	int k;
	int processRunning = 0;
	struct Process * runnableProcesses = malloc(50 * sizeof(struct Process));
	runnableProcesses = processes;
	float timeSoFar = 0;
	float numerator;
	printf("\tTime \tProcessName \t ArrivalTime \t ExpectedTotalRunTime \t TimeRemaining\n");
	for(j = 0; j < 10; j++)
	{
		printf("\t%i ",j);
		printf("\tP%i ", runnableProcesses[processRunning].processName);
		printf("\t%f ", processes[processRunning].arrivalTime);
		printf("\t%f ", processes[processRunning].expectedTotalRunTime);
		printf("\t%i\n", runnableProcesses[processRunning].timeRemaining);
		runnableProcesses[processRunning].timeRemaining--;
		if(runnableProcesses[processRunning].timeRemaining == 0)
		{
			timeSoFar += runnableProcesses[processRunning].expectedTotalRunTime;
			numerator +=  timeSoFar - runnableProcesses[processRunning].arrivalTime;
			processRunning++;
		}
		// for(k = 0; k < 50; k++)
		// {
		// 	if(processes[k].arrivalTime >= j)
		// 	{
		// 		runnableProcesses[k] = processes[k];
		// 	}
			
		// }	
	}
	float averageTurnAround = numerator/(processRunning);
	printf("%f\n", averageTurnAround);

}