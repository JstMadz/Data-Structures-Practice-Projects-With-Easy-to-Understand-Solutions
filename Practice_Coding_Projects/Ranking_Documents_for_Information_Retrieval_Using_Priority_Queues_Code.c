#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<limits.h>
#include<dirent.h>
#include<unistd.h>
#include<sys/types.h>
/*
*
*   ___  _   _ _   _ ____   ____    _    _   _
*  / _ \| \ | | | | |  _ \ / ___|  / \  | \ | |
* | | | |  \| | | | | |_) | |     / _ \ |  \| |
* | |_| | |\  | |_| |  _ <| |___ / ___ \| |\  |
*  \___/|_| \_|\___/|_|_\_\\____/_/   \_\_| \_|
* |_ _/ ___|| |   | ____|  _ \
*  | |\___ \| |   |  _| | |_) |
*  | | ___) | |___| |___|  _ <
* |___|____/|_____|_____|_| \_\__  ____
*  / ___/ ___|| ____| |___ \|___ \| ___|
* | |   \___ \|  _|     __) | __) |___ \
* | |___ ___) | |___   / __/ / __/ ___) |
*  \____|____/|_____| |_____|_____|____/___
* |  _ \|  _ \ / _ \    | | ____/ ___|_   _|
* | |_) | |_) | | | |_  | |  _|| |     | |
* |  __/|  _ <| |_| | |_| | |__| |___  | |
* |_|   |_| \_\\___/ \___/|_____\____| |_|
*  ___   ___ ___  __
* |__ \ / _ \__ \/_ |
*    ) | | | | ) || |
*   / /| | | |/ / | |
*  / /_| |_| / /_ | |
* |____|\___/____||_|
*
***************************
* ONURCAN ISLER 150120825 *
* CSE225 PROJECT#2        *
***************************
*
*
*
*
*******************************************************
*	DEAR USER THE OUTPUT MAY NOT FIT ON CONSOLE SO I  *
* RECOMMEND YOU TO OPEN output.txt FILE TO SEE THE    *
* OUTPUT OF THE PROGRAM, THANK YOU.                   *
*******************************************************
*
*/
/*
 *******************************************************
 *[How the program works?]                             *
 *Here is the steps:                                   *
 *                                                     *
 *1.WE ASK USER TO ENTER A KEYWORD.                    *
 *                                                     *
 *2.WE FIND THE FREQUENCY OF THE GIVEN KEYWORD IN EACH *
 *DOCUMENT, AND WE ENQUEUE BOTH THE FREQUENCY OF THE   *
 *KEYWORD AND THE NAME OF THE DOCUMENT INTO A          *
 *BINOMIAL HEAP.                                       *
 *                                                     *
 *3.WE ASK USER TO ENTER THE NUMBER OF RELEVANT        *
 *DOCUMENTS.                                           *
 *                                                     *
 *4.WE DEQUEUE 'NUMBER OF RELEVANT' TIMES.             *
 *******************************************************
 */
struct document{
    char*name;
    char*text;
};
typedef struct document document;

//******************************************************
//**           BINOMIAL HEAP IMPLEMENTATION           **
//******************************************************
struct binomialTree{
    document*doc;
    int frequency;
    int k;
//********************************************************
//*IMPORTANT:                                            *
//*In this project we apply only dequeu and enqueue      *
//*operations so WE DON'T NEED TO USE PARENT POINTER!    *
//*I won't use 'parent' so that I can save some space:)  *
//********************************************************
    struct binomialTree*children;
    struct binomialTree*sibling;
};
typedef struct binomialTree binomialTree;
struct binomialHeap{
    binomialTree*head;
    int size;
};
typedef struct binomialHeap binomialHeap;
binomialHeap*createBinomialHeap(void);
binomialTree*createBinomialTree(int val,document*doc);
void attachSecondToFirst(binomialTree*first,binomialTree*second);
binomialTree*mergeSortRoots(binomialTree*first,binomialTree*second);
void mergeHeaps(binomialHeap*heap1,binomialHeap*heap2);
void enqueue(binomialHeap*heap,int val,document*doc);
int getMax(binomialHeap*heap);
binomialTree*reverseChildren(binomialTree*curr,binomialTree*prev);
binomialTree*dequeue(binomialHeap*heap);
void printTree(binomialTree*tree);
void printHeap(binomialHeap*heap);
//******************************************************
//**               USEFUL METHODS                     **
//******************************************************
char*subString(char*buffer,int start,int end);
void toLowerCase(char*word);
//******************************************************
//**                  INT MAIN                        **
//******************************************************
int main(){
	FILE* output = fopen("output.txt","w");
	int i = 0;
    char*currentDirectory = (char*)malloc(sizeof(char)*(500));
    getcwd(currentDirectory,400);
    strcat(currentDirectory,"/files");
    //currentDirectory is the path name of the 'files' folder.
    
    int stop = (int)strlen(currentDirectory);
    DIR*thefile = opendir(currentDirectory);
    //directory is opened(I hope...).
    
    struct dirent*currDocument = NULL;
    if(thefile==NULL){
        printf("***********************************\n");
        fprintf(output,"***********************************\n");
        printf("*  (o_O) OH NO!                   *\n");
        fprintf(output,"*  (o_O) OH NO!                   *\n");
        printf("*Please make sure that there is a *\n");
        fprintf(output,"*Please make sure that there is a *\n");
        printf("*directory named as 'files'.      *\n");
        fprintf(output,"*directory named as 'files'.      *\n");
        printf("***********************************\n");
		fprintf(output,"***********************************\n");
    }
    
    //=================================================
    //GETTING THE KEYWORD FROM THE USER
    char*keyword = (char*)malloc(sizeof(char)*(200));
    printf("[PLEASE ENTER THE KEYWORD]:\n");
    fprintf(output,"[PLEASE ENTER THE KEYWORD]:\n");
    printf("=>");
    fprintf(output,"=>");
    scanf("%s",keyword);
    fprintf(output,"%s\n",keyword);
    toLowerCase(keyword);
    //=================================================
    
    //*****************************************************
    //**SINCE WE NEED MOST FREQUENT WORDS WE USE MAX HEAP**
    //*****************************************************
    binomialHeap*priorityQueue = createBinomialHeap();
    //in java style:
    //priorityQueue<document> myqueue = new binomialHeap<document>(greater<>);
    
    //Reading each document one by one.
    while((currDocument=readdir(thefile))!=NULL){
        document*doc = (document*)malloc(sizeof(document));
        doc->name = NULL;
        doc->text = NULL;
        //I created a document object so that the code will be
        //more understandable.
        
        doc->name = currDocument->d_name;
        if(doc->name[0]=='.'){continue;}//go to next document.
        //If the file name starts with '.' then it
        //is not a content file and we shouldn't open it.
        
        strcat(currentDirectory,"/");
        strcat(currentDirectory,doc->name);
        FILE*input = fopen(currentDirectory,"r");
        if(input==NULL){continue;}
        currentDirectory[stop] = '\0';
        //The document is opened.
        
        fseek(input,0L,SEEK_END);
        int size = (int)ftell(input);
        //Getting the size of the document. I do this because
        //reading line by line is not a good idea for this
        //project:)
        
        char*buffer = (char*)malloc(sizeof(char)*(size+1));
        buffer[size] = '\0';
        rewind(input);
        for(i=0;i<size;i++){
            buffer[i] = fgetc(input);
        }
        doc->text = buffer;
        //We have copied the whole text.
        
        int freq = 0;
        //How many 'keywords' are there in the current document?
        
        int start = -1,end=0;
        for(end=0;end<=size;end++){
            if(!isalnum(buffer[end])&&buffer[end]!='\''){
                if(start+1<end){
                    char*word = subString(buffer,start,end);
                    toLowerCase(word);
                    if(strcmp(word,keyword)==0){
                        freq++;
                    }
                }
                start = end;
            }
        }
        //We now know the frequency of the keyword in that document.
        //We enqueu both freq and doc.
        
        enqueue(priorityQueue, freq, doc);
    }
    
    //=================================================
    int numberOfRelevant;
    printf("\n*************************************************\n");
    fprintf(output,"\n*************************************************\n");
    printf("*THANK YOU. THE PRIORITY QUEUE HAS BEEN CREATED.*\n");
    fprintf(output,"*THANK YOU. THE PRIORITY QUEUE HAS BEEN CREATED.*\n");
    printf("*NOW, ENTER HOW MANY DOCUMENTS DO YOU NEED.     *\n");
    fprintf(output,"*NOW, ENTER HOW MANY DOCUMENTS DO YOU NEED.     *\n");
    printf("*************************************************\n");
    fprintf(output,"*************************************************\n");
    printf("=>");
    fprintf(output,"=>");
    scanf("%d",&numberOfRelevant);
    fprintf(output,"%d\n",numberOfRelevant);
    //=================================================
    
    binomialTree*answer[numberOfRelevant];
    for(i=0;i<numberOfRelevant;i++)
        answer[i] = NULL;
    //This array will help us to keep the most frequent documents so that
    //we can print them easily.
    
    //We pull the documents.
    for(i=0;i<numberOfRelevant;i++){
        answer[i] = dequeue(priorityQueue);
    }
    
    //Printing the relevance order...
    printf("\n");
    fprintf(output,"\n");
    printf("*************************************************\n");
    fprintf(output,"*************************************************\n");
    printf("*THANK YOU. THE RELEVANCE ORDER IS AS FOLLOWS:  *\n");
    fprintf(output,"*THANK YOU. THE RELEVANCE ORDER IS AS FOLLOWS:  *\n");
    printf("*************************************************\n");
    fprintf(output,"*************************************************\n");
    for(i=0;i<numberOfRelevant;i++){
        printf("%s(%d)",answer[i]->doc->name,answer[i]->frequency);
        fprintf(output,"%s(%d)",answer[i]->doc->name,answer[i]->frequency);
        if(i!=numberOfRelevant-1){
            printf(",");
            fprintf(output,",");
        }
    }
    printf("\n\n");
    fprintf(output,"\n\n");
    printf("*************************************************\n");
    fprintf(output,"*************************************************\n");
    printf("*             PRINTING THE DOCUMENTS            *\n");
    fprintf(output,"*             PRINTING THE DOCUMENTS            *\n");
    printf("*************************************************\n");
    fprintf(output,"*************************************************\n");
    //Printing the texts...
    for(i=0;i<numberOfRelevant;i++){
        printf("%d.[%s|%d]:\n",i+1,answer[i]->doc->name,answer[i]->frequency);
        fprintf(output,"%d.[%s|%d]:\n",i+1,answer[i]->doc->name,answer[i]->frequency);
        printf("%s\n\n\n\n\n",answer[i]->doc->text);
        fprintf(output,"%s\n\n\n\n\n",answer[i]->doc->text);
    }
    return 0;
}
//******************************************************
//**              BINOMIAL HEAP METHODS               **
//******************************************************
binomialHeap*createBinomialHeap(){//O(1)
    //To initialize the binomial heaps we have created.
    binomialHeap*temp = (binomialHeap*)malloc(sizeof(binomialHeap));
    temp->head = NULL;
    temp->size = 0;
    return temp;
}
binomialTree*createBinomialTree(int freq,document*doc){//O(1)
    //To initialize the binomial trees we have created.
    binomialTree*temp = (binomialTree*)malloc(sizeof(binomialTree));
    temp->children = NULL;
    temp->sibling = NULL;
    temp->k = 0;
    temp->frequency = freq;
    temp->doc = doc;
    return temp;
}
void attachSecondToFirst(binomialTree*first,binomialTree*second){//O(1)
    //We attach the nodes having the same degree. It is really easy.
    //Second becomes a child.
    second->sibling = first->children;
    first->children = second;
    first->k++;
}
binomialTree*mergeSortRoots(binomialTree*first,binomialTree*second){//O(logn+logm)
    //This function takes root list of the binomial trees as a parameter.
    //The algorithm of this function is called 'mergesort'. We merge
    //two sorted root lists using two pointers. The resulting sorted root list
    //is also in non-decreasing order.
    //Since, in the worst case, there could be log2(n) number of roots at the top;
    //its execution time is O(logn + logm) where the first has n elements and
    //the second has m elements.
    //The worst case occurs when n = 2^k - 1 where k is a positive integer.
    
    binomialTree*result = createBinomialTree(0,NULL);
    //When I read the algorithm in the CLRS book, I saw that there
    //were many edge cases we had to check:( Instead of all these checks,
    //I have used a temporary tree node so that we can apply operations directly.
    //This is my code, my choices:)
    
    binomialTree*tail = result;
    //'tail' will help us to insert the nodes. At the end of the root list.
    
    while(first!=NULL||second!=NULL){
        if(first==NULL){
            tail->sibling = second;
            break;
        }
        else if(second==NULL){
            tail->sibling = first;
            break;
        }
        else if(second->k < first->k){
            tail->sibling = second;
            second = second->sibling;
        }
        else{
            tail->sibling = first;
            first = first->sibling;
        }
        //We insert the node having smaller degree.
        //'tail' helps us to insert the node in O(1) time:)
        tail = tail->sibling;
    }
    return result->sibling;
    //result was temporary node...
}
void mergeHeaps(binomialHeap*heap1,binomialHeap*heap2){//O(logn + logm)
    //It is similiar to merging the forests. We only play with the roots
    //of the binomial trees. Since in a binomial heap with n nodes there
    //could be logn tree roots in the worst case, it has logarithmic
    //execution time.
    if(heap2->head==NULL){return;}
    //if heap2 is empty return heap1.
    heap1->size += heap2->size;
    //In my implementation, I want to keep the size of the heap.
    
    binomialTree*uniqueKs = mergeSortRoots(heap1->head,heap2->head);
    //Resulting tree node after merging forests. It is in non-decreasing
    //order but there could be multiple tree nodes having the same degree so
    //in following code will we find these nodes and we will merge them.
    
    heap2->head = NULL;
    heap2->size = 0;
    //We delete heap2. We don't need it anymore...
    
    binomialTree*prev = NULL;
    binomialTree*curr = uniqueKs;
    binomialTree*next = curr->sibling;
    //These guys will help us to detect consecutive nodes having the same
    //degree k.
    
    while(next!=NULL){
        if((next->k!=curr->k)||(next->sibling!=NULL&&curr->k==next->k&&next->k==next->sibling->k)){
            prev = curr;
            curr = next;
            //There are three consecutive nodes having the same degree or the
            //current pair have different degrees. So there is no problem
            //we can continue.
            //I want to share the reason why we skip when there are three
            //consecutive nodes.
            //Suppose degrees are like this:
            // 1 2 2 2 6 7 8
            //If we merge previous pair, we would have:
            // 1 3 2 6 7 8
            //Its degrees are not in non-decreasing order. It violates
            //binomial heap property.
            //We should merge the last pair:
            // 1 2 3 6 7 8
        }
        else{
            //Well... We have a problem now. We have detected consecutive pair having
            //the same degree. But we can't just merge them because we don't know
            //which root should be at the top and which root should be a child of
            //the other. Since we want to implement MAX BINOMIAL HEAP,
            //the node having BIGGER VALUE should be parent. The other should be child.
            
            //FUN FACT: Just changing that sign '>' to '<' would cause our heap to be
            //a minimum binomial heap.
            if(curr->frequency > next->frequency){
                curr->sibling = next->sibling;
                //Curr becomes the root of the next. The code below is important
                //since when next becomes child we would lose the pointer of the
                //next->sibling and we wouldn't be able to continue to process.
                attachSecondToFirst(curr,next);
            }
            else{
                if(prev==NULL){uniqueKs = next;}
                else{prev->sibling = next;}
                //When curr becomes child, the sibling of the prev
                //should be 'next' otherwise prev would point dangerous nodes (O_O)
                attachSecondToFirst(next,curr);
                curr = next;
            }
        }
        next = curr->sibling;
    }
    heap1->head = uniqueKs;
    //The binomial tree nodes are in non-decreasing order and unique.
}
void enqueue(binomialHeap*heap,int freq,document*doc){//O(logn)
    //We use merging to enqueue a single document.
    //As we have discussed earlier, merging takes O(logn) time. It is because,
    //in the worst case there would be log2(n) nodes at the top. The worst case
    //occurs when a binomial heap with n elements has the all possible trees:
    //B[0],B[1],B[2],B[3],...,B[log2(n)-1]
    //so worst case happens when n = 2^k - 1 where k is a positive integer.
    binomialHeap*temp = createBinomialHeap();
    temp->head = createBinomialTree(freq,doc);
    temp->size = 1;
    mergeHeaps(heap,temp);
}
int getMax(binomialHeap*heap){//O(logn)
    //To get the maximum frequency in the heap.
    int max = INT_MIN;
    binomialTree*maxFinder = heap->head;
    while(maxFinder!=NULL){
        if(max < maxFinder->frequency){
            max = maxFinder->frequency;
        }
        maxFinder = maxFinder->sibling;
    }
    return max;
}
binomialTree*reverseChildren(binomialTree*curr,binomialTree*prev){
    if(curr!=NULL){
        binomialTree*temp = curr->sibling;
        curr->sibling = prev;
        return reverseChildren(temp,curr);
    }
    return prev;
    //Reversing the children is important because each binomial
    //tree has decreasing ordered children. If we delete
    //the max element we have to reverse the children list so
    //that we can put children nodes back into the heap. If we
    //merge without reversing children, we wouldn't be able to use
    //merge sort algorithm consequently we wouldn't be able to put them
    //back into the heap.
}
binomialTree*dequeue(binomialHeap*heap){//O(logn)
    int maxValue = getMax(heap);
    //Well this is my code and my choices... In CLRS book there
    //were two extra pointers holding the previous nodes of the
    //node to be deleted. But instead, I have used my own way. I got
    //the node to be deleted before the operations. So we can
    //delete it easily.
    //IT DOESN'T CHANGE THE EXECUTION TIME since O(logn+logn)=O(logn)
    
    binomialTree*maxNode = heap->head;
    binomialTree*tempHead = createBinomialTree(0,NULL);
    //It will help us to keep previous node.
    
    tempHead->sibling = maxNode;
    binomialTree*prev = tempHead;
    while(maxNode->frequency!=maxValue){
        maxNode = maxNode->sibling;
        prev = prev->sibling;
    }
    //Both max node and the previous of the max node is found.
    
    prev->sibling = maxNode->sibling;
    //Max node has been removed from the root list. But we should put its children
    //back into our heap.
    
    heap->head = tempHead->sibling;
    binomialTree*childrenOfMin = reverseChildren(maxNode->children,NULL);
    //Children has been reversed and now degrees are in non-decreasing order.
    
    binomialHeap*helpMerge = createBinomialHeap();
    //A temporary heap to merge the children.
    
    helpMerge->head = childrenOfMin;
    mergeHeaps(heap,helpMerge);
    //Children have been put back into the heap.
    heap->size--;
    return maxNode;
}
//******************************************************
//**                HELPFUL METHODS                   **
//******************************************************
char*subString(char*buffer,int start,int end){
    int size = end-start-1;
    char*word = (char*)malloc(sizeof(char)*(size+1));
    word[size] = '\0';
    int i = start+1;
    for(i=start+1;i<end;i++){
        word[i-start-1] = buffer[i];
    }
    return word;
}
void toLowerCase(char*word){
    int size = (int)strlen(word);
    int i = 0;
    for(i=0;i<size;i++){
        word[i] = tolower(word[i]);
    }
}
