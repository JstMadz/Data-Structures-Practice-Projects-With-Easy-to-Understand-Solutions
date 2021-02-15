/*
 Author:
   ___  _   _ _   _ ____   ____    _    _   _
  / _ \| \ | | | | |  _ \ / ___|  / \  | \ | |
 | | | |  \| | | | | |_) | |     / _ \ |  \| |
 | |_| | |\  | |_| |  _ <| |___ / ___ \| |\  |
  \___/|_| \_|\___/|_|_\_\\____/_/   \_\_| \_|
 |_ _/ ___|| |   | ____|  _ \
  | |\___ \| |   |  _| | |_) |
  | | ___) | |___| |___|  _ <
 |___|____/|_____|_____|_|_\_\ ___ ____  ____
 / | ___| / _ \/ |___ \ / _ \ ( _ )___ \| ___|
 | |___ \| | | | | __) | | | |/ _ \ __) |___ \
 | |___) | |_| | |/ __/| |_| | (_) / __/ ___) |
 |_|____/ \___/|_|_____|\___/ \___/_____|____/
 Onurcan ISLER 150120825
 _____________________________________________
 || TEXT CLASSIFICATION USING BST STRUCTURE ||
 ---------------------------------------------
   _____  _____ ______ ___   ___ ___  _____
  / ____|/ ____|  ____|__ \ / _ \__ \| ____|
 | |    | (___ | |__     ) | | | | ) | |__
 | |     \___ \|  __|   / /| | | |/ /|___ \
 | |____ ____) | |____ / /_| |_| / /_ ___) |
  \_____|_____/|______|____|\___/____|____/
 PROJECT 2020
 
 
 PLEASE, READ ME:
     1)SOMETIMES THE OUTPUT MAY NOT FIT ON THE
     CONSOLE SINCE PRINTING A TREE DIAGRAM
     REQUIRES A LOT OF SPACE. TO SEE THE OUTPUT
     PLEASE GO TO 'output.txt' FILE.
 
     2)MAKE SURE THE FONT YOU ARE USING HAS SAME
     WIDTH FOR ALL CHARACTERS
     (i.e. widhtof(' ')==widthof('A')==widhtof('-')...)
     I RECOMMEND YOU TO USE 'courier' FONT TO SEE THE
     OUTPUT. I HAVE PRINTED TREES USING TWO DIFFERENT
     STYLES. YOU CAN FIND DETAILED EXPLANATION IN LINE-200.
 
     3)WHILE COMPARING THE KEYS I USED ONLY LETTERS.
     FOR EXAMPLE I HAVE ASSUMED:
     "hello" < "hi"
     "hello" == "H E LL O"
     "helloo" < "hello there"
     THE WAY WE COMPARE THE WORDS DIRECTLY EFFECTS THE OUTPUTS
     WE GET SINCE WE INSERT THE WORDS USING THE COMPARISON FUNCTION.
     THANK YOU FOR YOUR ATTENTION.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <time.h>
struct treeNode{
    char*word;
    int freq;
    struct treeNode*left;
    struct treeNode*right;
};
typedef struct treeNode treeNode;
struct tree{
    int size;
    treeNode*root;
};
typedef struct tree tree;

//-----------------------------------------------------------------------------
// GENERAL METHODS
int compareKeys(treeNode*first,treeNode*second);
char*getString(int first,int second,char*buffer);
//----------------------------------------------------------------------------
// HELPER CHAR POINTERS
char*minus = "-";
char*slash = "/";
char*bslash = "\\";
char*space = " ";
char*blank = "";
char*stripe = "|";
//-----------------------------------------------------------------------------
// METHODS OF TREE
void printBinaryTreeHorizontal(tree*mytree,FILE*output);
int getHeight(treeNode*root);
tree*createTree(void);
treeNode*createTreeNode(char*word,int freq);
void insert(tree*t,char*word,int freq);
void constructHorizontalMatrix(treeNode*currNode,int h,char*horizontalPrint[][6*h+1],int left,int right,int level);
treeNode*helperInsert(treeNode*root,treeNode*temp);
void totalAccessTime(treeNode*root,int level,int*result);
void rightStickHorizontal(int h,int mid,int right,int col,char*horizontalPrint[][6*h+1]);
void leftStickHorizontal(int h,int mid,int left,int col,char*horizontalPrint[][6*h+1]);
void printBinaryTreeVertical(char*prefix,treeNode*currNode,int isLeft,FILE*output);
void initializeKeys(treeNode*root,treeNode*keys[],int*index);
treeNode*constructOBST(int n,int rootTable[][n+1],int i,int j,treeNode* keys[]);
void printLNR(treeNode*root);
//------------------------------------------------------------------------------

// INT MAIN
int main(){
    tree treeA = *createTree();//initializing the tree given in a)
    tree treeC = *createTree();//initializing the tree given in c)
    FILE*input = fopen("input.txt","r");
    FILE*output = fopen("output.txt","w");
    if(input==NULL){
        printf("The file couldn't be opened.:(\n");
        fprintf(output,"The file couldn't be opened.:(\n");
        printf("Make sure there exists a txt file named as 'input'.\n");
        fprintf(output,"Make sure there exists a txt file named as 'input'.\n");
        return 0;
    }
    while(!feof(input)){
        char line[256];//It is safe to read the input line by line
        fgets(line,256,input);
        int start = 0;
        while(line[start]!=','){start++;}
        int end = start+1;
        while(line[end]!=','){end++;}
        char*word = getString(start+1,end-1,line);
        start = end;
        while(line[end]!='\n'&&line[end]!='\0'){end++;}
        int freq = atoi(getString(start+1,end-1,line));
        insert(&treeA,word,freq);
    }
    //The tree given in a) has been found...
    /*
     Now we will find the tree we are asked in part c).
     We should come up with a nice algorithm that finds the best tree.
     Of course, the best tree must also be a BST. Otherwise will have to
     traverse all of the treenodes to find a particular key in that tree:)
     But being a BST isn't enough to be the best binary tree. We should
     consider the frequency also:) We can't apply bruteforce because
     considering all the possible binary trees will take O(4^n / n^1.5)
     time and that is really bad.
     At this point we come up with dynamic programing... We find the
     min cost (total access time of a OBST) of each possible subtrees
     using a 2D array.
    */
    int n = treeA.size;//number of treenodes...
    int costTable[n+5][n+5];
    //costTable[i][j] is the best tree with the keys ki,...,kj
    int rootTable[n+1][n+1];
    //rootTable[i][j] is the root of best tree with the keys ki,...,kj
    treeNode* keys[n+1];
    //'keys' is an array of lexicographically ordered words. ki->ith word.
    //we don't store words but treenodes having these words as key.
    //Doing that way provides us to get the treenode having ith key in O(1) time.
    keys[0]=NULL;//since there is no key numbered zero.
    int index = 1;
    int i,d;
    //Thanks to TREEA's being a BST, we can get lexicographically ordered treenodes.
    initializeKeys(treeA.root,keys,&index);//LNR traversal to get 'keys'
    for(i=1;i<=n;i++){
        costTable[i][i-1]=0;
        costTable[i][i]=keys[i]->freq;
        rootTable[i][i]=i;
    }
    costTable[n+1][n]=0;
    //cost and root tables have been initialized so we now get into algorithm.
    /*
     The way algorithm works is a bit complicated. We want to find the root of
     the OBST with the keys ki,...,kj such that it will have smallest cost.
     To find the perfect root of these keys ki,...,kj we should try each of them.
     After this process we will make the treenode having smallest cost, root of
     the keys ki,...,kj. That is R[i][j]=l where l is the key of perfect root.
     */
    //our general formula is: COST[i][j] = minCostRoot + SUM(freqof(ki,...,kj))
    //detailed explanation could be found in my report.
    for(d=1;d<n;d++){
    //i have explained why we should traverse diagonals in my report by using graphs
        for(i=1;i<=n-d;i++){//keys starting with i
            int j=d+i;//keys ending with j
            int min = INT_MAX;
            int sum = 0;
            int l;
            for(l=i;l<=j;l++){//l is all the keys between i and j (ki<=kl<=kj)
                sum+=keys[l]->freq;
                //when l becomes root, total accesstime of OBST will increase by SUM
                int curr = costTable[i][l-1] + costTable[l+1][j];
                if(curr<min){
                    min = curr;
                    rootTable[i][j] = l;
                }
            }
            costTable[i][j] = min + sum;
        }
    }
    /*
     Since cost and root tables are ready we can now start constucting
     the best optimized binary search tree. It will be easy to do since
     we know which treenode will be the root of the each subtree.
     We will use recursion.
    */
    treeC.root = constructOBST(n,rootTable,1,n,keys);
    treeC.size = n;
    /*
     BOTH TREES ARE READY TO DISPLAY:)!!!YEEEEYYY!!
     But here is the question: How we should print them?
     Using LNR?
     Using LRN?
     Using NLR?
     NOOOOOOOOOO!!!!!
     We should print the tree itself! Actually there are two ways to do
     that: one is printing it vertical the other printing it horizontal.
     When I started to this project I have printed trees in vertical form.
     But then I noticed that If the tree we want to print had a height of
     greater than 8, all the columns didn't fit on one line. I was to find
     another way and I had to delete all the codes I had written in hours:(
     
     But then I have found a nice two ways to print the trees on Internet.
     
     -The first one is:
        \---+[clock]
            |---+[class]
            |   |---+[board]
            |   |   |---o
            |   |   \---o
            |   |
            |   \---o
            |
            \---+[pencil]
                |---+[kitchen]
                |   |---o
                |   \---o
                |
                \---+[window]
                    |---+[team]
                    |   |---o
                    |   \---o
                    |
                    \---o
     'o' represents null nodes here.
     right tree always down and starts with \
     left tree always up.
     
     -The second one is:
     
               /-----*[board]
               |
         /-----*[class]
         |
         |
         |
         *[clock]
         |
         |     /-----*[kitchen]
         |     |
         \-----*[pencil]
               |     /-----*[team]
               \-----*[window]
     
     left hand side is up,right hand side is down.
          
     Actually priting trees using first one is easier there is no need to
     explain it in detail. But the second one is really hard. I want to explain
     how do we print binary tree in horizontal.
     
     PRINTING TREE IN HORIZONTAL, HOW?
     We will have a 2D array of strings that is going to help us to know where
     a treenode should be placed.
     
        Suppose we have a tree like this:
     
            1
           / \
          2   3
         / \
        4   5
       /
      6
     
     1)First we will convert it to the this form:
     "",  "",  "", "",  "", "", "", "1", "",  "",  "",  "",  "", "", ""
     "",  "",  "", "2",  "", "", "", "", "",  "",  "",  "3",  "", "", ""
     "",  "4",  "", "",  "", "5", "", "", "",  "",  "",  "",  "", "", ""
     "6",  "",  "", "",  "", "", "", "", "",  "",  "",  "",  "", "", ""
     As we can see there are h rows where h is the hight of the tree.
     And there are (2^h-1) columns where h is the hight of the tree.
     
     2)Second we will convert it to the this horizontal form:
     "",  "",  "", "",  "", "", "", "", "",  "",  "",  "",  "", "", ""
     "",  "",  "", "",  "", "", "", "", "",  "",  "",  "",  "", "", ""
     "",  "",  "", "",  "", "", "", "", "",  "",  "",  "",  "", "", ""
     "|",  "-",  "-", "3",  "", "", "", "", "",  "",  "",  "",  "", "", ""
     "|",  "",  "", ",  "", "", "", "", "",  "",  "",  "",  "", "", ""
     "|",  "",  "", "",  "", "", "", "", "",  "",  "",  "",  "", "", ""
     "|",  "",  "", "",  "", "", "", "", "",  "",  "",  "",  "", "", ""
     "1",  "",  "", "",  "", "", "", "", "",  "",  "",  "",  "", "", ""
     "|",  "",  "", "",  "", "", "", "", "",  "",  "",  "",  "", "", ""
     "|",  "",  "", "|",  "-", "-", "5", "", "",  "",  "",  "",  "", "", ""
     "|",  "",  "", "|",  "", "", "", "", "",  "",  "",  "",  "", "", ""
     "|",  "-",  "-", "2",  "", "", "", "", "",  "",  "",  "",  "", "", ""
     "",  "",  "", "|",  "", "", "", "", "",  "",  "",  "",  "", "", ""
     "",  "",  "", "|",  "-", "-", "4", "", "",  "",  "",  "",  "", "", ""
     "",  "",  "", "",  "", "", "", "|", "-",  "6",  "",  "",  "", "", ""
     as we can see we have 2^h-1 rows but 3*h rows. We multiply the h by
     3. But in my code I will use 6*h version.
     
     We will have a function which will handle all these process...
     You can find that function below. Its name is 'printBinaryTreeHorizontal'.
     
     [!]LITTLE NOTE FOR THE READER:
     PRINTING A BINARY TREE WITH A HEIGHT OF GREATER THAN 10, MAY NOT BE A
     GOOD IDEA. I HAVE WRITTEN A STATEMENT WHICH WILL PREVENT US PRINTING VERY
     LARGE TREES. WHEN THE TREE IS TOO LARGE, WE WILL ONLY PRINT IT USING
     VERTICAL DIAGRAM:) IF YOU REALLY WANT TO SEE HOW THE TREE LOOKS LIKE YOU
     CAN DELETE THAT STATEMENT.
     THANK YOU:)
     */
    char prefix[400];
    prefix[0]='\0';
    printf("    _    ____ ____ _____ ____ ____    _____ ___ __  __ _____ ____      \n");
    printf("   / \\  / ___/ ___| ____/ ___/ ___|  |_   _|_ _|  \\/  | ____/ ___|   _ \n");
    printf("  / _ \\| |  | |   |  _| \\___ \\___ \\    | |  | || |\\/| |  _| \\___ \\  (_)\n");
    printf(" / ___ \\ |__| |___| |___ ___) |__) |   | |  | || |  | | |___ ___) |  _ \n");
    printf("/_/   \\_\\____\\____|_____|____/____/    |_| |___|_|  |_|_____|____/  (_)\n");
    printf("-------------------------------------------------------\n");
    fprintf(output,"    _    ____ ____ _____ ____ ____    _____ ___ __  __ _____ ____      \n");
    fprintf(output,"   / \\  / ___/ ___| ____/ ___/ ___|  |_   _|_ _|  \\/  | ____/ ___|   _ \n");
    fprintf(output,"  / _ \\| |  | |   |  _| \\___ \\___ \\    | |  | || |\\/| |  _| \\___ \\  (_)\n");
    fprintf(output," / ___ \\ |__| |___| |___ ___) |__) |   | |  | || |  | | |___ ___) |  _ \n");
    fprintf(output,"/_/   \\_\\____\\____|_____|____/____/    |_| |___|_|  |_|_____|____/  (_)\n");
    fprintf(output,"-------------------------------------------------------\n");
    int totAccessTime = 0;
    totalAccessTime(treeA.root, 0, &totAccessTime);
    printf("TOTAL ACCESS TIME OF THE TREE GIVEN IN A:%d\n",totAccessTime);
    printf("AVERAGE ACCESS TIME OF THE TREE GIVEN IN A:%f\n",(double)((double)totAccessTime/(double)treeA.size));
    printf("-------------------------------------------------------\n");
    fprintf(output,"TOTAL ACCESS TIME OF THE TREE GIVEN IN A:%d\n",totAccessTime);
    fprintf(output,"AVERAGE ACCESS TIME OF THE TREE GIVEN IN A:%f\n",(double)((double)totAccessTime/(double)treeA.size));
    fprintf(output,"-------------------------------------------------------\n");
    totAccessTime = 0;
    totalAccessTime(treeC.root, 0, &totAccessTime);
    printf("TOTAL ACCESS TIME OF THE TREE GIVEN IN C:%d\n",totAccessTime);
    printf("AVERAGE ACCESS TIME OF THE TREE GIVEN IN C:%f\n",(double)((double)totAccessTime/(double)treeC.size));
    printf("-------------------------------------------------------\n\n");
    printf("Please scroll down to see the trees:)\n");
    printf("If you can't see the trees, please reduce the size of the font you are using.\n");
    printf("Thank you:)\nAuthor:Onurcan ISLER\n\n");
    
    printf(" _______ _____  ______ ______                     \n");
    printf("|__   __|  __ \\|  ____|  ____|     /\\      _ \n");
    printf("   | |  | |__) | |__  | |__       /  \\    (_)\n");
    printf("   | |  |  _  /|  __| |  __|     / /\\ \\    _ \n");
    printf("   | |  | | \\ \\| |____| |____   / ____ \\  (_)\n");
    printf("   |_|  |_|  \\_\\______|______| /_/    \\_\\         \n");
    
    fprintf(output,"TOTAL ACCESS TIME OF THE TREE GIVEN IN C:%d\n",totAccessTime);
    fprintf(output,"AVERAGE ACCESS TIME OF THE TREE GIVEN IN C:%f\n",(double)((double)totAccessTime/(double)treeC.size));
    fprintf(output,"-------------------------------------------------------\n\n");
    fprintf(output,"Please scroll down to see the trees:)\n");
    fprintf(output,"I recommend you to use 'courier' font. Because it has same width for all\n");
    fprintf(output,"characters.\n");
    fprintf(output,"If you can't see the trees, please reduce the size of the font you are using.\n");
    fprintf(output,"Thank you:)\nAuthor:Onurcan ISLER\n\n");
    
    fprintf(output," _______ _____  ______ ______                     \n");
    fprintf(output,"|__   __|  __ \\|  ____|  ____|     /\\      _ \n");
    fprintf(output,"   | |  | |__) | |__  | |__       /  \\    (_)\n");
    fprintf(output,"   | |  |  _  /|  __| |  __|     / /\\ \\    _ \n");
    fprintf(output,"   | |  | | \\ \\| |____| |____   / ____ \\  (_)\n");
    fprintf(output,"   |_|  |_|  \\_\\______|______| /_/    \\_\\         \n");
    
    //YOU CAN DELETE THAT STATEMENT IF YOU WANT:
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    if(getHeight(treeA.root)<=10){printBinaryTreeHorizontal(&treeA,output);}
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    
    printf("----------------------------------------------------------------\n");
    fprintf(output,"----------------------------------------------------------------\n");
    printf("PRINTING BINARY TREE IN VERTICAL:\n('o' represents null nodes.)\n\n\n");
    fprintf(output,"PRINTING BINARY TREE IN VERTICAL:\n('o' represents null nodes.)\n\n\n");
    printBinaryTreeVertical(prefix,treeA.root,0,output);
    
    
    printf("\n\n\n\n\n\n\n");
    printf(" _______ _____  ______ ______      _____\n");
    printf("|__   __|  __ \\|  ____|  ____|    / ____|  _\n");
    printf("   | |  | |__) | |__  | |__      | |      (_)\n");
    printf("   | |  |  _  /|  __| |  __|     | |\n");
    printf("   | |  | | \\ \\| |____| |____    | |____   _\n");
    printf("   |_|  |_|  \\_\\______|______|    \\_____| (_)\n");
    fprintf(output,"\n\n\n\n\n\n\n");
    fprintf(output," _______ _____  ______ ______      _____\n");
    fprintf(output,"|__   __|  __ \\|  ____|  ____|    / ____|  _\n");
    fprintf(output,"   | |  | |__) | |__  | |__      | |      (_)\n");
    fprintf(output,"   | |  |  _  /|  __| |  __|     | |\n");
    fprintf(output,"   | |  | | \\ \\| |____| |____    | |____   _\n");
    fprintf(output,"   |_|  |_|  \\_\\______|______|    \\_____| (_)\n");
    
    //YOU CAN DELETE THAT STATMENT IF YOU WANT:
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    if(getHeight(treeC.root)<=10){printBinaryTreeHorizontal(&treeC,output);}
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    
    printf("----------------------------------------------------------------\n");
    fprintf(output,"----------------------------------------------------------------\n");
    printf("PRINTING BINARY TREE IN VERTICAL:\n('o' represents null nodes.)\n\n\n");
    fprintf(output,"PRINTING BINARY TREE IN VERTICAL:\n('o' represents null nodes.)\n\n\n");
    
    printBinaryTreeVertical(prefix,treeC.root,0,output);
    printf("\nThis program is created by Onurcan ISLER.\nPlease scroll up to see the time analysis.\n");
    printf("Access time analysis is at the top of the page.\nThank you.");
    printf("\nPLEASE GO TO 'output.txt' FILE TO SEE THE OUTPUT. SOMETIMES CONSOLE MAY NOT\nALLOW US TO SEE ALL ");
    printf("THE OUTPUT. I RECOMMEND YOU TO USE 'courier' FONT.\nTHANK YOU.");
    printf("\n\n\n");
    
    fprintf(output,"\nThis program is created by Onurcan ISLER.\nPlease scroll up to see the time analysis.\n");
    fprintf(output,"Access time analysis is at the top of the page.\nThank you.");
    fprintf(output,"\n\n\n");
    fclose(input);
    fclose(output);
    return 0;
}
//-----------------------------------------------------------------------------------
// METHODS OF TREE
treeNode*constructOBST(int n,int rootTable[][n+1],int i,int j,treeNode* keys[]){
    if(i>j){return NULL;}//As we know ki<=kj.
    
    treeNode*currNode = keys[rootTable[i][j]];//the root of OBST with keys ki,...,kj
    
    currNode->left = constructOBST(n, rootTable, i, rootTable[i][j]-1, keys);
    
    currNode->right = constructOBST(n, rootTable, rootTable[i][j]+1, j, keys);
    
    return currNode;
}
void printBinaryTreeHorizontal(tree*mytree,FILE*output){
    int h = getHeight(mytree->root);
    int rowSize = pow(2,h)-1;
    char* horizontalPrint[rowSize][6*h+1];
    int i,j;
    for(i=0;i<rowSize;i++)
        for(j=0;j<=6*h;j++)
            horizontalPrint[i][j] = space;
    constructHorizontalMatrix(mytree->root,h,horizontalPrint,0,rowSize,0);
    printf("\n\nPRINTING TREE IN HORIZONTAL:\nLeft hand side is up, right hand side is down.\n(Please reduce the font size if you can't see it very well.)\n\n");
    fprintf(output,"\n\nPRINTING TREE IN HORIZONTAL:\nLeft hand side is up, right hand side is down.\n(Please reduce the font size if you can't see it very well.)\n\n");
    for(i=0;i<rowSize;i++){
        for(j=0;j<=6*h;j++){
            int n = (int)strlen(horizontalPrint[i][j]);
            if(n>1){
                printf("*[");
                fprintf(output,"*[");
            }
            printf("%s",horizontalPrint[i][j]);
            fprintf(output,"%s",horizontalPrint[i][j]);
            if(n>1){
                printf("]");
                fprintf(output,"]");
            }
        }
        printf("\n");
        fprintf(output,"\n");
    }
    printf("\n\n");
    fprintf(output,"\n\n");
}
void constructHorizontalMatrix(treeNode*currNode,int h,char*horizontalPrint[][6*h+1],int left,int right,int level){
    if(left<right&&currNode!=NULL){
        int mid = (right+left)/2;
        horizontalPrint[mid][6*level] = currNode->word;
        if(currNode->left)
            leftStickHorizontal(h,left,mid,6*level,horizontalPrint);
        if(currNode->right)
            rightStickHorizontal(h,mid+1,right,6*level,horizontalPrint);
        constructHorizontalMatrix(currNode->left, h, horizontalPrint, left, mid, level+1);
        constructHorizontalMatrix(currNode->right, h, horizontalPrint, mid+1, right, level+1);
    }
}
void leftStickHorizontal(int h,int left,int right,int col,char*horizontalPrint[][6*h+1]){
    int row = (left+right)/2;
    int i;
    for(i=right-1;i>row;i--)
        horizontalPrint[i][col]=stripe;
    horizontalPrint[row][col]=slash;
    for(i=1;i<6;i++)
        horizontalPrint[row][col+i]=minus;
}
void rightStickHorizontal(int h,int left,int right,int col,char*horizontalPrint[][6*h+1]){
    int row = (left+right)/2;
    int i;
    for(i=left;i<row;i++)
        horizontalPrint[i][col]=stripe;
    horizontalPrint[row][col]=bslash;
    for(i=1;i<6;i++)
        horizontalPrint[row][col+i]=minus;
}
int getHeight(treeNode*curr){//O(logn) where n is numof nodes
    if(curr){
        int left = getHeight(curr->left);
        int right = getHeight(curr->right);
        return 1 + ((left>right)?left:right);
    }
    return 0;
}
void printLNR(treeNode*root){//O(n) where n is the numof nodes
    //Since TREEA and TREEC are BST printing them in LNR won't help;)
    if(root){
        printLNR(root->left);
        printf("%s ",root->word);
        printLNR(root->right);
    }
}
void initializeKeys(treeNode*root,treeNode*keys[],int*index){//O(n) where n is the #treenodes
    //we get lexicographically ordered keys using this LNR travere function.
    if(root){
        initializeKeys(root->left,keys,index);
        keys[*index] = createTreeNode(root->word,root->freq);
        *index = *index + 1;
        initializeKeys(root->right,keys,index);
    }
}
void totalAccessTime(treeNode*root,int depth,int*result){//O(n) where n is the #treenodes
    if(root){
        *result +=(depth+1)*root->freq;
        //This formula works if and only if the tree is a BST.
        //Otherwise formula would be *result+= root->freq * NumOfTreeNodes
        //Which will give us millions. That's why TREEC must be a BST.
        totalAccessTime(root->left,depth+1,result);
        totalAccessTime(root->right,depth+1,result);
    }
}
tree*createTree(){//O(1)
    //instead of writing the same code over and over again...
    //it is going to be kind a constructor like in C++
    tree*temp = (tree*)malloc(sizeof(tree));
    temp->root = NULL;
    temp->size = 0;
    return temp;
}
treeNode*createTreeNode(char*word,int freq){//O(1)
    //instead of writing the same code over and over again...
    treeNode*temp = (treeNode*)malloc(sizeof(treeNode));
    temp->left = NULL;
    temp->right = NULL;
    temp->freq = freq;
    temp->word = word;
    return temp;
}
treeNode*helperInsert(treeNode*currNode,treeNode*temp){//O(logn) where n is the numof nodes
    if(currNode){
        
        if(compareKeys(currNode,temp)>0)//check if the key of the temp is smaller...
            currNode->left = helperInsert(currNode->left,temp);
        
        else
            currNode->right = helperInsert(currNode->right,temp);
        
    }
    else{return temp;}
    return currNode;
}
void insert(tree*t,char*word,int freq){//inserting in TREEA
    treeNode*temp = createTreeNode(word,freq);
    t->root = helperInsert(t->root,temp);
    t->size++;//incrementing the size
}
void printBinaryTreeVertical(char*prefix,treeNode*currNode,int isLeft,FILE*output){
    //If the length of the keys are very small then the function is O(n)
    //Where n is the number of tree nodes.
    printf("%s",prefix);
    fprintf(output,"%s",prefix);
    char*helper=NULL;
    if(isLeft){
        helper = "|   ";
        printf("|---");
        fprintf(output,"|---");
    }
    else{
        helper = "    ";
        printf("\\---");
        fprintf(output,"\\---");
    }
    if(currNode){
        fprintf(output,"+[%s]\n",currNode->word);
        printf("+[%s]\n",currNode->word);
        char copy1[400];
        char copy2[400];
        int i;
        for(i=0;i<prefix[i]!='\0';i++){
            copy1[i] = prefix[i];
            copy2[i] = prefix[i];
        }
        int j;
        for(j=0;j<4;j++){
            copy1[i] = helper[j];
            copy2[i] = helper[j];
            i++;
        }
        copy1[i]='\0';
        copy2[i]='\0';
        printBinaryTreeVertical(copy1,currNode->left,1,output);
        printBinaryTreeVertical(copy2,currNode->right,0,output);
    }
    else{
        printf("o\n");
        fprintf(output,"o\n");
        if(isLeft==0){
            printf("%s\n",prefix);
            fprintf(output,"%s\n",prefix);
        }
    }
}
//---------------------------------------------------------------------
//GENERAL METHODS
char*getString(int first,int second,char*buffer){//O(n) where n is distance between first and second
    int n = second-first+2;
    char*result = (char*)malloc(sizeof(char)*n);
    int index = first;
    while(index<=second){
        result[index-first]=buffer[index];
        index++;
    }
    result[n-1]='\0';
    return result;
}
int compareKeys(treeNode*first,treeNode*second){//O(n+m) where n and m are the lengths of the strings
    /*
     IN MY CODE I IGNORED SPACE CHARACTERS. I ONLY CONSIDERED
     THE LETTERS WHEN COMPARING THESE WORDS.
     SO;
     IT WILL RETURN -1 WHEN str1=="helloa" and st2=="hello there"
     IT WILL RETURN 0 WHEN str1=="hello" and str2=="H e L L o"
     */
    char*str1 = first->word;
    char*str2 = second->word;
    int i=0,j=0;
    for(i=0,j=0;str1[i]!='\0'&&str2[j]!='\0';i++,j++){
        char a = str1[i];
        char b = str2[j];
        if(a==' '){j--;}
        else if(b==' '){i--;}
        else{
            a = (a>='A'&&a<='Z')? a+32 : a;
            b = (b>='A'&&b<='Z')? b+32 : b;
            if(a==b){continue;}
            return a>b ? 1 : -1;
        }
    }
    return (str1[i]=='\0'&&str2[j]=='\0') ? 0 : (str1[i]=='\0') ? -1 : 1;
}
