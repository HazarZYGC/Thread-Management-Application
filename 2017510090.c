#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define WAITING_ROOM_CAPACITY  3   //capacity of each waiting room.
#define UNIT_AMOUNT  8     // total test unit amount 
#define MAX_CUSTOMER 18  // number of patient

void *customer(void *num);
void *unit(void *num);
void randwait(int);


// defining semaphores :

//limits the # of customers allowed to enter the waiting room at one time. -countable(default)
sem_t waitingRoom; 

//cotrol of the operation of medical staff. -binary(default 0)
sem_t mutexStaff; 

//mutex to control waiting room status. -countable(default)
sem_t mutexRoom; 

//mutex to control waiting room status. -countable(default)
sem_t mutexCustomer;

int allDone = 0;   // if all patient's tests are done or note.

int customerInRoom=0;
int numberOfCustomer=0;
int unitNo=1;
int unitSchema[WAITING_ROOM_CAPACITY];  // to visualize the waiting rooms
int unitSchemaCounter = 0;


int printCount = 0; //print control for end.


int main(){
    pthread_t utid[UNIT_AMOUNT];   // unit threads.
    pthread_t tid[MAX_CUSTOMER];    // customer threads.
    int i; //temp

    //arrays to store unit and customer numbers.
    int numberCustomer[MAX_CUSTOMER]; 
    int numberUnit[UNIT_AMOUNT];

    for(i=0;i<UNIT_AMOUNT;i++)
    {
         numberUnit[i]=i+1;
    }
    for(i=0;i<MAX_CUSTOMER;i++)
    {
         numberCustomer[i]=i+1;
    }
    for(i=0;i<WAITING_ROOM_CAPACITY;i++)
    {
         unitSchema[i]=0;
    }

    //Initializing semaphores
    sem_init(&waitingRoom, 0, WAITING_ROOM_CAPACITY);
    sem_init(&mutexStaff, 0 , 0);
    sem_init(&mutexRoom, 0, 1);
    sem_init(&mutexCustomer, 0 , 1);

    //create unit threads
    for(i=0;i<UNIT_AMOUNT;i++)
    {
        pthread_create(&utid[i], NULL, unit, (void *)&numberUnit[i]);
    }
     
     //create customer threads
    for(i=0;i<MAX_CUSTOMER;i++)
    {
        pthread_create(&tid[i], NULL, customer, (void *)&numberCustomer[i]);
    }

    //join customers
    for (i = 0; i < MAX_CUSTOMER; i++) 
    {
		pthread_join(tid[i],NULL);
	}

    //join units.
      for (i = 0; i < UNIT_AMOUNT; i++) 
      {
		pthread_join(utid[i],NULL);
	}

    

    system("PAUSE");

    return 0;
}

void *customer(void *num)
{
    int customerNum = *(int *)num; //take customer (patient) number.

    sem_wait(&mutexCustomer); //lock after one customer.

    sem_post(&mutexStaff); //wake up the staff.

    randwait(4); //sleep

    printf("\nPeople %d is entering the hospital\n", customerNum);
    randwait(1);
    

    numberOfCustomer++;  // increase total customer number to compare with allDone controller.,

    sem_wait(&waitingRoom);  //decrease the remain capacity of waiting room.
    printf("\nPeople %d is at Covid-19 Test Unit %d  waiting room\n",customerNum,unitNo);

    unitSchema[unitSchemaCounter] = customerNum;    //updating unit schema
    unitSchemaCounter++;

    randwait(1);

    if(customerInRoom==0)   // when a customer enter the waiting room first
    { 
		printf("\nPeople %d wake up the Unit %d's medical staff.\n", customerNum,unitNo);
        randwait(1);
	}

    printf("\nPeople %d is filling the form\n\n",customerNum);
    randwait(1);
    printf("Status of unit %d : ",unitNo);
    
    //printing unit schema.
    for (int i = 0; i < WAITING_ROOM_CAPACITY; i++)
    {
        if(unitSchema[i]==0)
        {
            printf("[X]");
        }
        else
        {
            printf("[%d]",unitSchema[i]);
        }
        
    }
    printf("\n");
    randwait(3);
    customerInRoom++;    //increase the customer number in a waiting room.
    

    // if there is no more customer, all will be sleep.
    if(numberOfCustomer==MAX_CUSTOMER)
    { 
		
		allDone=1; 
		for(int i=0;i<UNIT_AMOUNT;i++)
        { 
			sem_post(&mutexStaff); // ventilating room before exit from hospital.
		}
		
	}
	
	sem_post(&mutexCustomer); // unlock the mutex for new customer.
    randwait(3);
} 

void *unit(void *num)
{
    while(!allDone)  // continue until all customer will be tested.
    {
        sem_wait(&mutexStaff); // locking the customer mutex for operation.
        if(!allDone)
        {
            sem_wait(&mutexRoom); // locking room operations for staff operations.
            int remainSeat = WAITING_ROOM_CAPACITY - customerInRoom; // calculating remain seat in the waiting room.

            if(remainSeat==2)
            {
                printf("\nCovid-19 Test Unit %d medical staff announce:\n",unitNo);
                printf("The last 2 people, let's start! Please, pay attention to your social distance and hygiene; use a mask.\n");
            }
            else if(remainSeat==1)
            {
                printf("\nCovid-19 Test Unit %d medical staff announce:\n",unitNo);
                printf("The last person, let's start! Please, pay attention to your social distance and hygiene; use a mask.\n");
            }
            else if(numberOfCustomer!=MAX_CUSTOMER && remainSeat==0)  //if there is no empty seat
            {
                printf("\nCovid-19 Test Unit %d 's mediacal staff apply the covid test\n",unitNo);
                randwait(1);
    
                int temp = customerInRoom;   // reseting waiting room
                for(int i=0;i<temp;i++)
                {
                    sem_post(&waitingRoom); 
                    customerInRoom--; 
                }
                for(int i=0;i<WAITING_ROOM_CAPACITY;i++) //reinitialize the unit schema
                {
                    printf("\nPeople %d is leaving from hospital\n",unitSchema[i]);
                    randwait(1);
                    unitSchema[i]=0;
                }
                unitSchemaCounter = 0;
                unitNo = *(int *)num;
            }
            sem_post(&mutexRoom); // unlock the room mutex.
            randwait(1);
        }
        else // if all customer is tested.
        {
            if(printCount==0)
            { 
                printCount++;
                printf("\nCovid-19 Test Unit %d 's mediacal staff apply the covid test\n",unitNo);
                randwait(1);
                for(int i=0;i<WAITING_ROOM_CAPACITY;i++) // for last unit testing.
                {
                    printf("\nPeople %d is leaving from hospital\n",unitSchema[i]);
                    randwait(1);
                    unitSchema[i]=0;
                }
                printf("\nAll customer is tested in the hospital.\n");
                printf("\nMedical staff are leaving from hospital.\n");
            }
            
        }
        
        
    }
} 


void randwait(int secs) { 
     sleep(secs);
}
