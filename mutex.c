/*Mutex Program*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node*head;
pthread_mutex_t mutex;

//input variables
int noOfMember;
int noOfInsert;
int noOfDelete;
int noOfThreads;

int i, j;

// linked list data
int data[1000]; 

// linked list data for operations
int testData[10000]; 

void *perform_operation(void* rank);
int Member(int val);
int Insert(int val);
int Delete(int val);
long getCurrentTime(void);

int main(int argc, char* argv[]) {

   if (argc != 5) {
        printf("Incorrect number of input values!");
        exit(0);
    }

	// linked list number of operations
	noOfMember = strtod(argv[1], NULL) * 10000;
	noOfInsert = strtod(argv[2], NULL) * 10000;
	noOfDelete = strtod(argv[3], NULL) * 10000;

	// number of threads
	noOfThreads = strtod(argv[4], NULL);
	   
    //Mutex initialization
	pthread_mutex_init(&mutex, NULL);

	// Random unique number generation between 0 and 2*16 - 1 
    for (i = 0; i < 1000; i++) {
        while (1) {
            int temp = rand() % 65536;
            int found = 0;
            for (j = 0; j < i; j++) {
                if (data[j] == temp) { found = 1; break;}
            }
            if (found == 0) { data[i] = temp; break;}
        }
    }

	// Random unique number generation between 0 and 2*16 - 1 for test data set
    for (i = 0; i < 10000; i++) {
        while (1) {
            int temp = rand() % 65536;
            int found = 0;
            for (j = 0; j < i; j++) {
                if (testData[j] == temp) { found = 1; break;}
            }
            if (found == 0) { testData[i] = temp; break;}
        }
    }

    //populate linked list with 1000 random values
    for (i = 0; i < 1000; i++) {
        Insert(data[i]);
    }

    long startTime, endTime, timeSpent;

    // start time of all operations
    startTime = getCurrentTime();

    long thread;
    pthread_t* thread_handles;
    thread_handles = malloc (noOfThreads * sizeof(pthread_t));

    for (thread = 0; thread < noOfThreads; thread++)
        pthread_create(&thread_handles[thread], NULL, perform_operation, (void*)thread);

    for (thread = 0; thread < noOfThreads; thread++)
        pthread_join(thread_handles[thread], NULL);

    pthread_mutex_destroy(&mutex);
    free(thread_handles);

    // end time of all operations
    endTime = getCurrentTime();

    // time taken for whole operations
	timeSpent = endTime - startTime;
	printf("%ld", timeSpent);
	return 0;
}

//get the current time
long getCurrentTime() {
	struct timeb tmb;
	ftime(&tmb);
	return tmb.millitm;
}

void *perform_operation(void* rank) {
    long my_rank = (long) rank;
    int i, offset = (10000 * my_rank) / noOfThreads;
    int my_last_i = offset + (10000 / noOfThreads);
    long memberOperation = noOfMember/ noOfThreads;
    long insertOperation = noOfInsert / noOfThreads;
    long deleteOperation = noOfDelete / noOfThreads;
    

    for (i = offset; i < my_last_i; i++) {
        if (i < offset + insertOperation) {
            pthread_mutex_lock(&mutex);
            Insert(testData[i]);
            pthread_mutex_unlock(&mutex);
        }
        else if (i < offset + insertOperation + deleteOperation) {
            pthread_mutex_lock(&mutex);
            Delete(testData[i]);
            pthread_mutex_unlock(&mutex);
        }
        else {
            pthread_mutex_lock(&mutex);
            Member(testData[i]);
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

//Linked list member operation
int Member(int value) {
    struct Node*curr_p = head;

    while (curr_p != NULL && curr_p->data < value) {
        curr_p = curr_p->next;
    }
    if (curr_p == NULL || curr_p->data > value) {
        return 0;
    }
    else {
        return 1;
    }
}

//Linked list insert operation 
int Insert(int value) {
    struct Node*curr_p = head;
    struct Node*pred_p = NULL;
    struct Node*temp_p;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct Node));
        temp_p->data = value;
        temp_p->next = curr_p;

        if (pred_p == NULL) {
            head = temp_p;
        }
        else {
            pred_p->next = temp_p;
        }
        return 1;
    }
    else {
        return 0;
    }
}

//Linked list delete operation
int Delete(int value) {
    struct Node*curr_p = head;
    struct Node*pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }
    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {
            head = curr_p->next;
            free(curr_p);
        }
        else {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
    }
    else {
        return 0;
    }
}
