#include <string.h>
#include <math.h>
#include <stdlib.h>
struct Process {
	int processName;
	float arrivalTime;
	float expectedTotalRunTime;
	int priority;
};

struct Process processFactory(int pName, float arrivalT, float expectedTotalrt, int priority) 
{
	struct Process p;
	p.processName = pName;
	p.arrivalTime = arrivalT;
	p.expectedTotalRunTime = expectedTotalrt;
	p.priority = priority;

	return p;
}

int main(void) {
	
	int i;
	struct Process processes[50];
	for (i = 0; i < 50; i++) 
	{
		processes[i] = processFactory(65+i, (float)rand()/(float)(RAND_MAX/100), ((float)rand()/(float)(RAND_MAX/9.9))+0.1, rand() % 4 + 1 );
	}
	printf("%i ",processes[0].processName);
	printf("%f ",processes[0].arrivalTime);
	printf("%f ",processes[0].expectedTotalRunTime);
	printf("%i\n",processes[0].priority);
	
	printf("%i ",processes[1].processName);
	printf("%f ",processes[1].arrivalTime);
	printf("%f ",processes[1].expectedTotalRunTime);
	printf("%i\n",processes[1].priority);

	printf("%i ",processes[2].processName);
	printf("%f ",processes[2].arrivalTime);
	printf("%f ",processes[2].expectedTotalRunTime);
	printf("%i\n",processes[2].priority);
	
	printf("%i ",processes[3].processName);
	printf("%f ",processes[3].arrivalTime);
	printf("%f ",processes[3].expectedTotalRunTime);
	printf("%i\n",processes[3].priority);
	
}