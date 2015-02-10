struct Process {
	char[2] processName
	float arrivalTime;
	float expectedTotalRunTime;
	int priority;
}

Process processFactory(char[2] pName, float arrivalT, float expectedTotalrt, int priority) {
	Process p;

	p.processName = pName;
	p.arrivalTime = arrivalT;
	p.expectedTotalRunTime = expectedTotalRunTime;
	p.priority = priority;

	return p;
}

int main(void) {
	int i;
	Process[50] processes;

	for (i = 0; i < 50; i++) {
		processes[i] = processFactory(65+i, (float)rand()/(float)(RAND_MAX/100), ((float)rand()/(float)(RAND_MAX/9.9))+0.1, rand() % 4 + 1 );
	}

	cout << processes[0].processName << endl;
	cout << processes[1].processName << endl;
	cout << processes[2].processName << endl;
}