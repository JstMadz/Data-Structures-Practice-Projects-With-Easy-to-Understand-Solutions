#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
//==============================================================
//==============================================================
//BINARY SEARCH TREE
struct bstNode{
    struct bstNode*left;
    struct bstNode*right;
    char*data;
    int key;
};
typedef struct bstNode bstNode;
struct stringSet{
    int size;
    bstNode*root;
};
typedef struct stringSet stringSet;
bstNode*insertToBstHelp(bstNode*current,bstNode*temp,int*check){
    if(current){
        int comp = strcmp(current->data,temp->data);
        if(comp>0)
            current->left = insertToBstHelp(current->left,temp,check);
        else if(comp<0)
            current->right = insertToBstHelp(current->right,temp,check);
        else{
            *check=1;
            return current;
        }
    }
    else{return temp;}
    return current;
}
bstNode*createBstNode(char*data){
    bstNode*temp = (bstNode*)malloc(sizeof(bstNode));
    temp->left = NULL;
    temp->right = NULL;
    temp->data = data;
    temp->key = 0;
    return temp;
}
void insertToBst(stringSet*tree,char*data){
    int check = 0;
    bstNode*temp = createBstNode(data);
    tree->root = insertToBstHelp(tree->root,temp,&check);
    if(!check){tree->size++;}
}
stringSet*createTree(){
    stringSet*temp = (stringSet*)malloc(sizeof(stringSet));
    temp->root = NULL;
    temp->size = 0;
    return temp;
}
char*subString(char*buffer,int left,int right){
    int n = right-left;
    char*temp = (char*)malloc(sizeof(char)*(n+1));
    for(int i=left+1;i<right;i++){
        temp[i-left-1] = buffer[i];
    }
    temp[n]='\0';
    char*ret = temp;
    return ret;
}
void inorderPrint(bstNode*current){
    if(current){
        inorderPrint(current->left);
        printf("%s ",current->data);
        inorderPrint(current->right);
    }
}
void initializeKeys(bstNode*current,int*index){
    if(current){
        initializeKeys(current->left,index);
        current->key = *index;
        *index = *index + 1;
        initializeKeys(current->right,index);
    }
}
int getKey(bstNode*current,char*data){
    if(current){
        int comp = strcmp(current->data,data);
        if(comp>0)
            return getKey(current->left,data);
        else if(comp<0)
            return getKey(current->right,data);
        else
            return current->key;
    }
    return 0;
}
char* getData(bstNode*current,int key){
    if(current){
        if(current->key>key)
            return getData(current->left,key);
        else if(current->key<key)
            return getData(current->right,key);
        else
            return current->data;
    }
    return NULL;
}
//==============================================================
//==============================================================
// STACK
struct node{
    int val;
    struct node*next;
};
typedef struct node node;
struct stack{
    int size;
    node*top;
};
typedef struct stack stack;
stack*createStack(){
    stack*temp = (stack*)malloc(sizeof(stack));
    temp->size = 0;
    temp->top = NULL;
    return temp;
}
node*createNode(int val){
    node*temp = (node*)malloc(sizeof(node));
    temp->val = val;
    temp->next = NULL;
    return temp;
}
void pushStack(stack*s,int val){
    s->size++;
    node*newTop = createNode(val);
    newTop->next = s->top;
    s->top = newTop;
}
void popStack(stack*s){
    s->size--;
    node*temp = s->top;
    s->top = s->top->next;
    free(temp);
}
//==============================================================
//==============================================================
// INT MAIN
int main(){
    stringSet tree = *createTree();
    FILE*input = fopen("input.txt","r");
    while(!feof(input)){
        char line[256];
        fgets(line, 256, input);
        int start=0,end;
        while(line[start]!=':'){start++;}
        for(end = start;line[end]!='\n'&&line[end]!='\0';end++){
            if(line[end]==','){
                insertToBst(&tree,subString(line,start,end));
                start = end;
            }
        }
        insertToBst(&tree,subString(line,start,end));
    }
    int n = tree.size;
    int index = 0;
    initializeKeys(tree.root,&index);
    int graph[n][n];
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            graph[i][j]=0;
    rewind(input);
    while(!feof(input)){
        char line[256];
        fgets(line, 256, input);
        int start=0,end;
        while(line[start]!=':'){start++;}
        int size = 1;
        for(end = start;line[end]!='\n'&&line[end]!='\0';end++){
            if(line[end]==','){size++;}
        }
        int adjCourses[size];
        size = 0;
        for(end = start;line[end]!='\n'&&line[end]!='\0';end++){
            if(line[end]==','){
                adjCourses[size++] = getKey(tree.root,subString(line,start,end));
                start = end;
            }
        }
        adjCourses[size++] = getKey(tree.root,subString(line,start,end));
        for(int i=0;i<size;i++){
            for(int j=i+1;j<size;j++){
                graph[adjCourses[i]][adjCourses[j]] = 1;
                graph[adjCourses[j]][adjCourses[i]] = 1;
            }
        }
    }
    //|---------| 9
    printf("[ADJANCENCY MATRIX]\n");
    for(int j=0;j<=(n+1)*10;j++){
        printf("-");
    }
    printf("\n|         ");
    for(int j=0;j<n;j++){
        printf("|%-9.8s",getData(tree.root,j));
    }
    printf("|\n");
    for(int j=0;j<=(n+1)*10;j++){
        printf("-");
    }
    printf("\n");
    for(int i=0;i<n;i++){
        printf("|%-9.8s",getData(tree.root,i));
        for(int j=0;j<n;j++){
            printf("|    ");
            printf("%d",graph[i][j]);
            printf("    ");
        }
        printf("|\n");
        for(int j=0;j<=(n+1)*10;j++)
            printf("-");
        printf("\n");
    }
    printf("\n\n");
    // DFS graph coloring
    int colors[n];
    for(int i=0;i<n;i++)
        colors[i]=-1;
    int visited[n];
    for(int i=0;i<n;i++)
        visited[i] = 0;
    stack stackk = *createStack();
    pushStack(&stackk,0);
    while(stackk.size){
        int top = stackk.top->val;
        popStack(&stackk);
        visited[top]=1;
        int availableColor[n];
        for(int j=0;j<n;j++)
            availableColor[j]=1;
        for(int j=n-1;j>=0;j--){
            if(graph[top][j]==0){continue;}
            if(colors[j]!=-1){availableColor[colors[j]]=0;}
            if(!visited[j]){pushStack(&stackk,j);}
        }
        for(int j=0;j<n;j++){
            if(availableColor[j]){
                colors[top] = j;
                break;
            }
        }
    }
    printf("[SCHEDULE]\n");
    int time = 1;
    for(int color=0;color<n;color++){
        int exist = 0;
        for(int i=0;i<n;i++){
            if(colors[i]==color){
                exist = 1;
                break;
            }
        }
        if(exist==0){continue;}
        printf("The exams in %02d",time++);
        printf(".02.2021 -> ");
        for(int i=0;i<n;i++){
            if(colors[i]==color){
                printf("%s ",getData(tree.root,i));
            }
        }
        printf("\n");
    }
    printf("\n\n");
    return 0;
}
//==============================================================
//==============================================================
