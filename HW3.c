#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX 75
#define ID_BASE 101
#define CLASS_SIZE 20


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

//MAIN METHOD
int main()
{

	//CREATING THE THREE STUDENT LISTS
	List *graduatingList = createList(); 
	List *seniorList = createList();
	List *everyoneList = createList();

	//SETTING THE SEED TO GENERATE TRUE RANDOM NUMBERS
	srand(time(0));			   
    time(&startTime);
	
    //LOOPING THROUGH AND CREATING OUR LIST OF STUDENTS
    //PRIORITY DETERMINES WHICH LIST THEY ENTER
	int studentLoop;
	for(studentLoop = 0; studentLoop < MAX; studentLoop++)
	{
		int id = ID_BASE+studentLoop;
		int priority = rand() % 3 + 1;
		int canTake = rand() % 4 + 1;
		Student s = createStudent(id,priority,canTake);
		if(priority == 1)
		{
			push(graduatingList, &s);
			//printf("Pushed onto Graduating\n");
		}
		else if(priority == 2)
		{
			push(seniorList, &s);
			//printf("Pushed onto Seniors\n");
		}
		else
		{
			push(everyoneList, &s);
			//printf("Pushed onto Everyone\n");
		}
	}

	int check;
	for(check = 0; check < graduatingList->count; check++)
	{
		Student a = *pop(graduatingList);
		printf("%i ", a.id);
		printf("%i ", a.priority);
		printf("%i\n", a.canTake);
	}
	

	
	
	
	

}
