/*
PLEASE, DO NOT CHANGE void display(bool verbose, bool testing), int getUsableDay() and int getUsableTime() FUNCTIONS.
YOU HAVE TO WRITE THE REQUIRED  FUNCTIONS THAT IS MENTIONED ABOVE. YOU CAN ADD NEW FUNCTIONS IF YOU NEED.
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include "task_management_tool.h"

using namespace std; 


void WorkPlan::display(bool verbose,bool testing)
{
	
	string inone="***";
	if (head!=NULL)
	{
		Task *pivot =new Task;
		Task *compeer =new Task;
			
		pivot=head;
		do
		{
			if(testing)
				inone+=" ";
			else
				cout << pivot->day <<". DAY"<< endl;
			compeer=pivot;
			while(compeer!=NULL)
			{
				string PREV= compeer->previous!=NULL?compeer->previous->name:"NULL";
				string NEXT=compeer->next!=NULL?compeer->next->name:"NULL";
				string CONT=compeer->counterpart!=NULL?compeer->counterpart->name:"NULL";
				if (testing)
					inone+=compeer->name;
				else
					if(verbose)
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< PREV<<"\t<- "<< compeer->name <<"("<< compeer->priority <<")->\t"<<NEXT <<"\t |_"<<CONT<<endl;
					else{
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< compeer->name <<"("<< compeer->priority <<")"<<endl;
					}				
				compeer=compeer->counterpart;
			}
			pivot=pivot->next;
		}while(pivot!=head);
		if(testing)
		{
			cout<<inone<<endl;
			cout<<"(checking cycled list:";
			if (checkCycledList())
				cout<<" PASS)"<<endl;
			else
				cout<<" FAIL)"<<endl;
		}
	}
	else
		cout<<"There is no task yet!"<<endl;
}

int WorkPlan::getUsableDay()
{
	return usable_day;
}

int WorkPlan::getUsableTime()
{
	return usable_time;
}


void WorkPlan::create()
{		
	head=NULL;							//create a head
	usable_day=1;
	usable_time=8;

}

void WorkPlan::close()
{
	head->previous->next=NULL;			//break the circular linked list
	Task *traverse=head;      			
	Task *counter=head->counterpart;
	Task *traverse_tail=NULL;
	Task *counter_tail=NULL;
	while(traverse!=NULL){				//travese in list
		while(counter!=NULL){			//traverse in traverses day
			counter_tail=counter;
			counter=counter->counterpart;
			delete counter_tail;		//delete counter tail and make it NULL
			counter_tail=NULL;
		}
		traverse_tail=traverse; 		//move traverse to next and traverse tail is back node of traverse
		traverse=traverse->next;		
		if(traverse==NULL){				//if traverse is NULL it means linked list is finished
			delete traverse_tail;		//delete traverse tail and finish function
			traverse_tail=NULL;
			return;
		}
		delete traverse_tail;			//if list is not finished delete traverse tail and continue to traverse
		traverse_tail=NULL;
		counter=traverse->counterpart;
	}
}

void WorkPlan::add(Task *task)
{
	
	Task *temp= new Task;															//create a new task struct pointer
	char *temp_name= (char*) malloc((strlen(task->name)+1)*sizeof(char));			//copy the sent pointers name
	for(int unsigned i=0 ; i<(strlen(task->name)+1) ; i++){							
		*(temp_name+i)=*(task->name+i) ;
	}
	temp->day=task->day;															//transfer the sent tasks info's to our created node and make NULL its realtions
	temp->name=temp_name;
	temp->time=task->time;
	temp->priority=task->priority;
	temp->previous=NULL;
	temp->next=NULL;
	temp->counterpart=NULL;
	Task *delay =NULL;
	if(head!=NULL){																	//if head is not NULL 
		delay=getTask(temp->day,temp->time);										//check if the wanted day and time exist or not
		if(delay!=NULL){															//if wanted day and time already taken we should look priority
			if(delay->priority>=temp->priority){									//if taken by higher priority 
				checkAvailableNextTimesFortoAdd(temp);								//check next available time for sent node
				temp->day=usable_day;
				temp->time=usable_time;
			}else{																	//if taken by lower priority node then put sent node there 
				Task *delayed = new Task;											//create a new struct to carry node with lower priority
				checkAvailableNextTimesFortoAdd(delay);								//find next available time for delay and delay iit to that point
				delayed->day=usable_day;											
				delayed->time=usable_time;
				delayed->priority=delay->priority;
				delayed->name=delay->name;
				delay->name=NULL;
				remove(delay);
				add(delayed);
			}
			
		}
	}
	if(head == NULL){												//If there is no task yet this is the first task 
		head=temp;
		temp->next=temp;
		temp->previous=temp;
	}else{															//If there exist already some tasks
		Task *traverse = head;
		if(temp->day == head->day){									//if the task is in the first day
			Task *tail = traverse;
			if(traverse->time > temp->time ){						//if the new task is the first task of that day
				if(traverse->next == traverse ){					//if there is only one day
					head=temp;										//make new task head
					temp->next=temp;								
					temp->previous=temp;
					temp->counterpart=traverse;
					traverse->next=NULL;
					traverse->previous=NULL;
				}else{												//if there exist days more than one
					temp->counterpart = traverse;
					temp->next = traverse->next;
					temp->previous=traverse->previous;
					temp->next->previous=temp;
					temp->previous->next=temp;
					traverse->next=NULL;
					traverse->previous=NULL;
					head=temp;
				}
			}else{													//if task is somewhere in between 2 any tasks in first day
				while(traverse->time < temp->time){					//find true place to put the new task
					tail = traverse;
					traverse = traverse->counterpart;
					if(traverse==NULL){								//if come to last point in that day stop searching
						break;
					}
				}
				tail->counterpart=temp;								//put the task to counterpart of tail
				temp->counterpart=traverse;
			}

		}else{															//if task is somewhere else except first day
			traverse=head->next;										//transfer traverse to next node
			while(traverse->day != temp->day && traverse != head){		//search for tasks day in linked list
				traverse=traverse->next;
			}
			if(traverse == head){										//That means tasks day is not in the linked list
				if(temp->day < head->day){								//if tasks day smaller than head day we must update our head and assign it to the new task
					temp->previous=head->previous;
					temp->next=head;
					head->previous->next=temp;
					head->previous=temp;
					head=temp;
					
				}else{													//if the new task is not our new head 
					Task *tail=traverse;
					traverse=traverse->next;
					
					while(temp->day > traverse->day && traverse!=head){//search for the new tasks day
						tail=traverse;
						traverse= traverse->next;
					}
					if(traverse==head){									//tasks day is greater than all days in linked list
						temp->previous=head->previous;
						temp->next=head;
						head->previous->next=temp;
						head->previous=temp;
					}else{												//if the task is somewhere except between head and last day
						temp->previous=tail;
						temp->next=traverse;
						traverse->previous=temp;
						tail->next=temp;
					}
				}
			}else{														//if tasks day already exists in linked list
			
				Task *tail=traverse;
				if(traverse->time > temp->time ){						//if task is the first task of that day
					temp->counterpart=traverse;
					temp->next=traverse->next;
					temp->previous=traverse->previous;
					temp->next->previous=temp;
					temp->previous->next=temp;
					traverse->next=NULL;
					traverse->previous=NULL;
				}else{													//if task is somewhere between any 2 tasks in that day
					while(traverse->time < temp->time ){				//find true place to put the new task in that day
						tail = traverse;
						traverse = traverse->counterpart;
						if(traverse==NULL){								//if traverse is NULL it means the new task is latest task of that day
							break;
						}
					}
					tail->counterpart=temp;								//put the task to the last point in that day
					temp->counterpart=traverse;
				}
			}
		}
		

	}
	
	
} 

Task * WorkPlan::getTask(int day, int time)
{	
	Task *traverse;												//create a pointer to traverse linked list
	if(head->day==day){											//if wanted task is in first day
		traverse=head;											//dont search for day
	}else{														//if wanted day is not in the first day
		traverse=head->next;
		while(traverse->day!=day && traverse!=head){			//search for wanted tasks day
			traverse=traverse->next;
		}
		if(traverse==head){										//if traverse returns back to head it means that day is not in the linked list
			return NULL;
		}
	}
	
	while(traverse->time!=time){								//after finding day of wanted task than search for time of wanted task
		traverse=traverse->counterpart;
		if(traverse==NULL){										//if couldnt found in counterparts of that day it means that task is naot in the listed list
			return NULL;
		}
	}
	return traverse;


}


void WorkPlan::checkAvailableNextTimesFortoAdd(Task *delayed)//I needed a new function to find available time when I adding because available time algorith works different when I adding and when I delaying
{
	
	usable_day=delayed->day;							//make usable day and time equal to sent tasks values
	usable_time=delayed->time;
	
	bool checker=true;
	
	while(checker){										//perform operation
		usable_time++;									//increase delayeds time
		if(usable_time==17){							//if it passes last hour we increase day and set hour as 8
			usable_day++;
			usable_time=8;
		}
		Task *check = getTask(usable_day,usable_time);	//we get increased hour and day

		if(check==NULL){								//if the increased day and time is available than we found and finish the function
			break;
		}else{											//increased time or day is not available
			if(check->priority >= delayed->priority){	//increased time already taken by a task which has higher priority than we continue to search
				continue;
			}else{										//increased time already taken by a task which has lower priority so we delay here than finish the function
				checker=false; 
				
			}
		}

	}

}


void WorkPlan::checkAvailableNextTimesFor(Task *delayed)	
{
	Task *traverse=head;								//create a pointer which eqaul to head
	Task *tail=NULL;									
	if(delayed->day!=traverse->day){					//if delayed day is not head day 
		traverse=traverse->next;
		while(traverse->day!=delayed->day){				//search for delayeds day
			traverse=traverse->next;
		}
	}	
	traverse=traverse->next;
	int last_hour=traverse->time;
	tail=traverse;
	while(tail!=NULL){									//search for delays time 
		tail=tail->counterpart;
		if(tail!=NULL){									//if we did not reach the last task of that day we should update last hour
			last_hour=tail->time;
		}
	}
	usable_day=traverse->day;							//firstly make usable day equal to that day we may change it later
	usable_time=8;										//we will try to put first hour of that day
	Task *check=NULL;
	while(true){										//perform operations
		if(usable_time>=last_hour){						//if usable time passes last hour we should increase the day 
			traverse=traverse->next;
			usable_day=traverse->day;
			tail=traverse;
			usable_time=8;
			while(tail!=NULL){							//when we update the day we must also update our last day for our new day					
				tail=tail->counterpart;
				if(tail!=NULL){
					last_hour=tail->time;
				}
			}
		}
		check=getTask(usable_day,usable_time);			//when we find a day and time we check is it taken or not
		
		if(check==NULL){								//if it is not taken it means we found and finish function
			break;
		}
		usable_time++;									//if couldnt found we increase time and continue to search
	}
		

}


void WorkPlan::delayAllTasksOfDay(int day)
{
	Task *traverse=head;
	Task *tail=NULL;
	if(traverse->day!=day){									//first we find the days first tasks pointer
		traverse=traverse->next;
		while(traverse->day!=day && traverse!=head){
			traverse=traverse->next;
		}
	}
	tail=traverse;			
	while(traverse!=NULL){									//Until reach last point of that day perform operations
		Task *temp= new Task;								//Allocate a new Task to delete old one and add new one
		tail=traverse->counterpart;		
		checkAvailableNextTimesFor(traverse);				//find available time for that task
		temp->day=usable_day;								//fill the informations of temp with traverses informations
		temp->time=usable_time;
		temp->priority=traverse->priority;
		temp->name=traverse->name;
		traverse->name=NULL;
		remove(traverse);									//delete old node and add new node
		add(temp);
		traverse=tail;
	}
}

void WorkPlan::remove(Task *target)
{
	Task *traverse=target;									
	Task *tail=NULL;
	traverse=head;
	bool isfirstday=true;									//a checker for is task is in first day or not
	bool ishead=false;										//a checker for is task is head or not
	if(target!=head){										//if target is not in head day
		isfirstday=false;									
		while(target->day!=traverse->day){					//search for its day
			traverse=traverse->next;
		}
	}
	tail=traverse;
	if(traverse!=target){									//target is counterpart
		while(tail->counterpart!=target){					//find its place
			tail=tail->counterpart;
		}
		tail->counterpart=target->counterpart;				//when found delete it
		delete target;
		target=NULL;

	}else{													//target is first task of a day
		ishead=true;
		if(ishead && isfirstday){							//if target is first day and first task than its head it means we should update our head
			if(target->counterpart!=NULL){					//if targets counterpart iis not NULL than head becomes its counterpart
				head=target->counterpart;					
			}else{											//if target is only one task of head day that head becomes targets next node
				head=target->next;
			}
		}
		if(target->counterpart==NULL){						//if target is only one task of that day than we connect its next node and previous node
			traverse->next->previous=traverse->previous;
			traverse->previous->next=traverse->next;
			delete traverse;
			target=NULL;
		}else{												//if target is not only one node of that day than we connect its next, counterpart and previous than delete target
			target->next->previous=target->counterpart;
			target->counterpart->next=target->next;
			target->previous->next=target->counterpart;
			target->counterpart->previous=target->previous;
			target->next=NULL;
			target->previous=NULL;
			target->counterpart=NULL;
			delete target;
			target=NULL;
		}
		
		
	}
	
	
	
}

bool WorkPlan::checkCycledList()
{				
	Task *pivot=new Task();
	pivot=head;
	int patient=100;
	bool r=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->previous;
		if(pivot==head)
		{
			r=true;
			break;
		}
	}
	cout<<"("<<100-patient<<")";
	patient=100;
	bool l=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->next;
		if(pivot==head)
		{
			l=true;
			break;
		}
	}
	return r&l;
}
