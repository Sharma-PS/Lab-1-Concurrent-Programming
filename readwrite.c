/* Read-Write Lock Program */

#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <pthread.h>

struct Node {
	int data;
	struct Node* next;
};

struct Node* head;

pthread_rwlock_t rwlock;

// Input variables
int noOfMember;
int noOfInsert;
int noOfDelete;
int noOfThreads;

int i, j;

// linked list data
int data[1000];

// linked list data for operations
int testData[10000];

void* perform_operation(void* rank);
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

	pthread_rwlock_init(&rwlock, NULL);

	// Random unique number generation between 0 and 2*16 - 1 
	for (i = 0; i < 1000; i++) {
		while (1) {
			int temp = rand() % 65536;
			int found = 0;
			for (j = 0; j < i; j++) {
				if (data[j] == temp) { found = 1; break; }
			}
			if (found == 0) { data[i] = temp; break; }
		}
	}

	// Random unique number generation between 0 and 2*16 - 1 for test data set
	for (i = 0; i < 10000; i++) {
		while (1) {
			int temp = rand() % 65536;
			int found = 0;
			for (j = 0; j < i; j++) {
				if (testData[j] == temp) { found = 1; break; }
			}
			if (found == 0) { testData[i] = temp; break; }
		}
	}

	for (i = 0; i < 1000; i++) {
		Insert(data[i]);
	}

	long thread;
	pthread_t* thread_handles;
	thread_handles = malloc(noOfThreads * sizeof(pthread_t));

	long startTime, endTime, timeTaken;

	// start time of all operations
	startTime = getCurrentTime();

	for (thread = 0; thread < noOfThreads; thread++) {
		pthread_create(&thread_handles[thread], NULL, perform_operation, (void*)thread);
	}

	for (thread = 0; thread < noOfThreads; thread++) {
		pthread_join(thread_handles[thread], NULL);
	}


	// end time of all operations
	endTime = getCurrentTime();

	// time taken for whole operations
	timeTaken = endTime - startTime;
	free(thread_handles);
	printf("%ld", timeTaken);
	return 0;
}

// Get current time
long getCurrentTime() {
	struct timeb tmb;
	ftime(&tmb);
	return tmb.millitm;
}

void* perform_operation(void* rank) {
	long my_rank = (long)rank;
	int i, offset = (10000 * my_rank) / noOfThreads;
	int my_last_i = offset + (10000 / noOfThreads);
	long insertOperation = noOfInsert / noOfThreads;
	long deleteOperation = noOfDelete / noOfThreads;
	long memberOperation = noOfMember / noOfThreads;

	for (i = offset; i < my_last_i; i++) {
		if (i < offset + insertOperation) {
			pthread_rwlock_wrlock(&rwlock);
			Insert(testData[i]);
			pthread_rwlock_unlock(&rwlock);
		}
		else if (i < offset + insertOperation + deleteOperation) {
			pthread_rwlock_wrlock(&rwlock);
			Delete(testData[i]);
			pthread_rwlock_unlock(&rwlock);
		}
		else {
			pthread_rwlock_rdlock(&rwlock);
			Member(testData[i]);
			pthread_rwlock_unlock(&rwlock);
		}
	}
	return NULL;
}

int Member(int value) {
	struct Node* curr_p = head;

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

int Insert(int value) {
	struct Node* curr_p = head;
	struct Node* pred_p = NULL;
	struct Node* temp_p;

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

int Delete(int value)
{
	struct Node* curr_p = head;
	struct Node* pred_p = NULL;

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
