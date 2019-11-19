/* @Author
Student Name: Muhammed Salih YILDIZ
Student ID: 150180012
Date: 22-10-2019 */

#include <iostream>
#include <fstream>

using namespace std;

struct node {
    int size;
    int quant;
    node *next;
};

struct stock {
    node *head;
    void create();
    void add_stock(int);
    void sell(int);
    void current_stock();
    void clear();
};

void stock::create(){
    head = NULL; 
}

void stock::add_stock(int size){
    bool result = false;//it will be used for checking.
    node * traverse = head;
    node * tail=NULL;
    while(traverse!=NULL){//traverses in linked list    
        if(traverse->size==size){//checks for if that size exists in linked list of not
            result=true;
            break;
        }
        tail = traverse;
        traverse = traverse->next;
    }
    if(result){//if exists increases quant
        traverse->quant++;
    }else{//if not creates a new node
        node * newnode;
        newnode = new node;
        newnode->size = size;
        newnode->quant = 1;
        newnode->next = NULL;
        traverse=head;
        tail=NULL;
        while(traverse->size < size){//traverse to place newnode to correct place
            tail=traverse;
            traverse=traverse->next;
            if(traverse==NULL){
                break;
            }   
        }
        
        if(traverse == NULL){//if all sizes smaller than newnode than newnode becomes last node
            tail->next=newnode;
            newnode->next=NULL; 
            
        }else if(tail == NULL){//if all sizes greater than newnode than newnode becomes head
            head=newnode;
            newnode->next=traverse;
            
        }else{//if newnode somewhere between two nodes
            tail->next=newnode;
            newnode->next=traverse;
        }
        
    }
    
}

void stock::sell(int size){
    if(head==NULL){//if linked list is empty print directly NO_STOCK
        cout<<"NO_STOCK\n";
        return;
    }
    bool result = false;//it will be used for checking.
    node * traverse = head;
    node * tail = NULL ;
    while(traverse!=NULL){//traverse in linked list
        if(traverse->size == size){
            result=true;
            break;
        }
        tail = traverse;
        traverse = traverse->next;
    }
    if(result){//if size that we search exists 
        if(traverse->quant != 1){//if that sizes quant is not one descrease its quant
            traverse->quant--;
        } else {//if quant is one we will delete that size
            if(tail==NULL){//if searched size is head and its quant is one than we change head and delete old head
                head=head->next;
                delete traverse;
                traverse=NULL;
            }else{//if searched size is not head we delete it and tails next becomes its next
            tail->next = traverse->next;
            delete traverse;
            traverse = NULL;
            }
        }     
    } else {//if searched size is not in the linked list print NO_STOCK
        cout<<"NO_STOCK"<<endl;    
    }

}

void stock::current_stock(){
    node * traverse = head;
    while(traverse != NULL){//traverse all nodes and print their size and quant
        cout<<traverse->size<<":"<<traverse->quant<<endl;
        traverse=traverse->next;
    }
}


void stock::clear(){
    node * traverse=head;
    node * tail=NULL;
    while(traverse != NULL){//traverse all nodes and delete tail
        tail = traverse;
        traverse = traverse->next;
        delete tail;
        tail=NULL; 
    }
    delete tail;//there left one node and delete it here
    tail = NULL;
}


typedef stock datast;

datast shoe;



int main(int argc,char* argv[]){
    shoe.create();

    int temp=-1;//temp becomes -1
    FILE * fpt= NULL;
    char* text=argv[1];
    fpt=fopen(text,"r");
    while(temp<=0){//temp assigned -1 so it must enter at least one time this while(This while to create head node)
        fscanf(fpt,"%d", &temp);//reads from text 
        if(temp>0){//if temp is not minus or 0 create a newnode and it becomes head
            shoe.head = new node;
            shoe.head->size=temp;
            shoe.head->quant=1;
            shoe.head->next=NULL;
        }else{//if temp is zero or minus print NO_STOCK
            cout<<"NO_STOCK"<<endl;
        }
    }
    while(!feof(fpt)){//reads from text untill end
        fscanf(fpt,"%d", &temp);
        if(temp > 0){//if temp is positive call add_stock function     
            shoe.add_stock(temp);
        }else if(temp == 0){//if temp is zero call current_stock function
            shoe.current_stock();
        }else if(temp < 0){//if temp is negative call sell function
            shoe.sell(temp*(-1));
        }
    }
    
    shoe.clear();//delete all nodes that have been allocated



    return 0;
}