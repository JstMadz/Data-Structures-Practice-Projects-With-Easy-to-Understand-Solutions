#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct node{
	struct node*next;
	int val;
};
typedef struct node node;
struct linkedList{
	node*head;
	int size;
};
typedef struct linkedList linkedList;
linkedList*createLinkedList(){
	linkedList*temp = (linkedList*)malloc(sizeof(linkedList));
	temp->size = 0;
	temp->head = NULL;
	return temp;
}
node*createNode(int val){
	node*temp = (node*)malloc(sizeof(node));
	temp->next = NULL;
	temp->val = val;
	return temp;
}
void pushBack(linkedList*list,int val){
	if(list->head==NULL){
		list->head = createNode(val);
	}
	else{
		node*temp = list->head;
		while(temp->next!=NULL){
			temp = temp->next;
		}
		temp->next = createNode(val);
	}
	list->size++;
}
void display(linkedList*list){
	int n = list->size;
	node*temp = list->head;
	int array[n+1];
	int i;
	for(i=n-1;i>=0;i--){
		array[i] = temp->val;
		temp = temp->next;
	}
	for(i=0;i<n;i++){
		printf("%d",array[i]);
	}
	printf("\n");
}
int main(){
	while(1){
		char*firstNum = (char*)malloc(sizeof(char)*255);
		char*secondNum = (char*)malloc(sizeof(char)*255);
		printf("Please enter the first number:\n");
		scanf("%s",firstNum);
		printf("Please enter the second number:\n");
		scanf("%s",secondNum);
		int firstSize = strlen(firstNum);
		printf("firstSize:%d\n",firstSize);
		int secondSize = strlen(secondNum);
		printf("secondSize:%d\n",secondSize);
		linkedList*first = createLinkedList();
		linkedList*second = createLinkedList();
		int i;
		for(i=firstSize-1;i>=0;i--){
			pushBack(first,(int)(firstNum[i]-'0'));
		}
		for(i=secondSize-1;i>=0;i--){
			pushBack(second,(int)(secondNum[i]-'0'));
		}
		node*above = first->head;
		node*below = second->head;
		node*result = createNode(0);
		node*rewind = result;
		int start = 0;
		while(below){
			result = rewind;
			for(i=0;i<start;i++){result = result->next;}
			int remMulti = 0;
			int remSum = 0;
			while(above||remMulti){
				int curr = ((above==NULL)? 0 : above->val) * below->val + remMulti;
				remMulti = curr/10;
				int sum = ((result->next==NULL)? 0 : result->next->val) + curr%10 + remSum;
				if(result->next==NULL)
					result->next = createNode(sum%10);
				else
					result->next->val = sum%10;
				remSum = sum/10;
				if(above){above = above->next;}
				result = result->next;
			}
			while(remSum){
				int sum = ((result->next==NULL)? 0 : result->next->val) + remSum;
				remSum = sum/10;
				if(result->next==NULL){
					result->next = createNode(sum%10);
				}
				else{
					result->next->val = sum%10;
				}
				result = result->next;
			}
			above = first->head;
			below = below->next;
			start++;
		}
		linkedList*answer = createLinkedList();
		node*temp = rewind->next;
		while(temp){
			pushBack(answer,temp->val);
			temp = temp->next;
		}
		printf("answer: ");
		display(answer);
		printf("--------------------------------------\n");
	}
	return 0;
}
