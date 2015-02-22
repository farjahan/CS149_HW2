#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>



#define MAX 75
#define ID_BASE 101
#define CLASS_SIZE 20
#define REGISTRATION_DURATION 120



int arrivalsCount = 0;
int leavesCount = 0;
int studentsInSections = 0;

pthread_mutex_t graduatingListMutex;  // mutex protects graduating List and studentsInSection
pthread_mutex_t seniorListMutex;  // mutex protects senior List and studentsInSection
pthread_mutex_t everyoneListMutex;  // mutex protects everyone List and studentsInSection
pthread_mutex_t printMutex;  // mutex protects everyone List and studentsInSection

sem_t graduatingFilled;          // Graduating processor waits on this semaphore
sem_t seniorFilled;          	// Senior processor waits on this semaphore
sem_t everyoneFilled;          // Everyone processor waits on this semaphore

struct itimerval graduatingTimer;  // processes 2 hour timer
time_t startTime;

//STUDENT STRUCT
//FOR NOW ONLY HOLDS ID AND NAME
typedef struct
{
	int id;
	int priority; // 1 = Graduating Senior
				  // 2 = Regular Senior
				  // 3 = Everyone Else

	int canTake;  // 1 = Section 1
				  // 2 = Section 2 
				  // 3 = Section 3
				  // 4 = All Sections
}Student;

Student section1[CLASS_SIZE];
Student section2[CLASS_SIZE];
Student section3[CLASS_SIZE];


//NODE IN THE LIST
//HOLDS A STUDENT AS ITS VALUE
//AND REFERENCES TO THE NEXT AND PREV NODE IN LIST
struct Node
{
	Student s;
	struct Node *next;
	struct Node *prev;
};

//LIST DATA STRUCT 
//HOLDS REFERENCES TO FIRST AND LAST ELEMENT
//AS WELL AS # OF ELEMENTS
typedef struct
{
	int count;
	struct Node *first;
	struct Node *last;
}List;

//CREATING FIRST ARRAY OF ALL STUDENTS
Student allStudents[MAX];

//CREATING THE THREE STUDENT LISTS
List *graduatingList; 
List *seniorList;
List *everyoneList;

Student beingProcessed;

//CREATES A POINTER TO AN EMPTY LIST
List *createList()
{
	return calloc(1,sizeof(List));
}

//PUSHES STUDENT INTO THE END OF THE LIST
void push(List *list, Student *value)
{
	//CREATE NODE AND SET ITS VALUE TO BE THE STUDENT PASSED
	struct Node *node = calloc(1,sizeof(struct Node)); 
	node->s = *value;

	//CHECKS IF LIST IS EMPTY 
	if(list->last == NULL) 
	{
		list->first = node;
		list->last = node;
	}
	else //APPEND TO THE END AND UPDATE THE LAST NODE IN LIST
	{
		list->last->next = node;
		node->prev = list->last;
		list->last = node;
	}
	list->count++; //INCREMENT COUNT FOR EASY ACCESS TO # OF ELEMENTS	
}

//REMOVAL OF SPECIFIC NODE
Student *remove_from_list(List *list, struct Node *node)
{
	if(list->first == NULL && list->last == NULL)
	{
		printf("EMPTY LIST");
		return NULL;
	}
	else if(node == list->first && node == list->last)
	{
		list->first = NULL;
		list->last = NULL;
		return &node->s;
	}
	else if(node == list->first)
	{
		list->first = node->next;
		list->first->prev = NULL;
		return &node->s;
	}
	else if(node == list->last)
	{
		list->last = node->prev;
		list->last->next = NULL;
		return &node->s;
	}
	else 
	{
        struct Node *after = node->next;
        struct Node *before = node->prev;
        after->prev = before;
        before->next = after;
        return &node->s;
    }
    list->count--;
}

//POPS FIRST STUDENT OUT OF LIST WITH ANOTHER CALL TO REMOVE
Student *pop(List *list)
{
	struct Node *node = list->first;
	return remove_from_list(list, node);
}



//RETURNS A STUDENT WITH PARAMETERS PASSED
Student createStudent(int idNum, int prior, int can)
{
	Student c;
	c.id = idNum;
	c.priority = prior;
	c.canTake = can;
	return c;
}

// STUDENT ARRIVES
void studentArrives(Student s)
{
    char event[80];
    arrivalsCount++;

    if (studentsInSections < CLASS_SIZE*3) 
    {
    	if(s.priority == 1)
    	{
    		//LOCK THE LIST 
       		pthread_mutex_lock(&graduatingListMutex);

        	// PLACE A STUDENT INTO A LIST
        	push(graduatingList,&s);
        	
	        // Release the mutex lock.
	        pthread_mutex_unlock(&graduatingListMutex);

	        sprintf(event, "Student %i arrives and waits", s.id);
	        print(event);

	        // Signal the "filledSlots" semaphore.
	        sem_post(&graduatingFilled);  // signal
    	}

    }
    else 
    {
        leavesCount++;
        sprintf(event, "Student %i arrives and leaves", s.id);
        print(event);
    }
}

//STUDENT THREAD
void *student(void *param)
{
    Student student = *((Student *) param);

    //ARRIVE AT RANDOM TIMES DURING REGISTRATION TIME
    sleep(rand()%REGISTRATION_DURATION);
    studentArrives(student);

    return NULL;
}


int timesUp = 0;  // 1 = office hour is over

// The processor meets a GRADUATING SENIOR 
void processorMeetsStudent()
{
    if (!timesUp) {

        // Wait on the "graduatingFilled" semaphore for a student.
        sem_wait(&graduatingFilled);

        // Acquire the mutex lock to protect the chairs and the wait count.
        pthread_mutex_lock(&graduatingListMutex);

        // Critical region: Remove a student from a chair.
        meetingId = chairs[out];
        out = (out+1)%CHAIR_COUNT;
        waitCount--;

        // Release the mutex lock.
        pthread_mutex_unlock(&graduatingListMutex);

        char event[80];
        sprintf(event, "Professor meets with student %d",  meetingId);
        print(event);

        // Meet with the student.
        sleep(rand()%MAX_MEETING_DURATION + 1);
        meetingsCount++;

        sprintf(event, "Professor finishes with student %d",  meetingId);
        meetingId = 0;
        print(event);
    }
}

// The Graduating List Processor thread.
void *graduating(void *param)
{
    //print("Professor opens her door");

    // Set the timer for for office hour duration.
    graduatingTimer.it_value.tv_sec = REGISTRATION_DURATION;
    setitimer(ITIMER_REAL, &graduatingTimer, NULL);

    // Meet students until the office hour is over.
    do {
        processorMeetsStudent();
    } while (!timesUp);

    print("registration time is over");
    return NULL;
}




// Timer signal handler.
void timerHandler(int signal)
{
    timesUp = 1;  // registration time is over
}


//MAIN METHOD
int main()
{
	
	graduatingList = createList(); 
	seniorList = createList();
	everyoneList = createList();

	int graduatingId = 0;

	// Initialize the mutexes and the semaphore.
    pthread_mutex_init(&graduatingListMutex, NULL);
    pthread_mutex_init(&seniorListMutex, NULL);
    pthread_mutex_init(&everyoneListMutex, NULL);
    pthread_mutex_init(&printMutex, NULL);

    sem_init(graduatingFilled, 0, 0);
    sem_init(seniorFilled, 0, 0);
    sem_init(everyoneFilled, 0, 0);

     // Create the graduating senior List processor thread.
    pthread_t graduatingThreadId;
    pthread_attr_t graduatingAttr;
    pthread_attr_init(&graduatingAttr);
    pthread_create(&graduatingThreadId, &graduatingAttr, graduating, &graduatingId);


	//SETTING THE SEED TO GENERATE TRUE RANDOM NUMBERS
	srand(time(0));			   
    time(&startTime);
	
    //LOOPING THROUGH AND CREATING OUR LIST OF STUDENTS
	int studentLoop;
	for(studentLoop = 0; studentLoop < MAX; studentLoop++)
	{
		int id = ID_BASE+studentLoop;
		int priority = rand() % 3 + 1;
		int canTake = rand() % 4 + 1;
		Student s = createStudent(id,priority,canTake);
		allStudents[studentLoop] = s;
		pthread_t studentThreadId;
        pthread_attr_t studentAttr;
        pthread_attr_init(&studentAttr);
        pthread_create(&studentThreadId, &studentAttr, student, &allStudents[studentLoop]);
	}

	 // Set the timer signal handler.
    signal(SIGALRM, timerHandler);

    // Wait for the professor to complete the office hour.
    pthread_join(graduatingThreadId, NULL);
	

	
	
	
	

}
