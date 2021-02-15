#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
//===========================================================
//    TREE IMPLEMENTATIONS
//===========================================================
struct songBstNode{
    char*name;
    int length;
    struct songBstNode*left;
    struct songBstNode*right;
};
typedef struct songBstNode songBstNode;
struct songBinarySearchTree{
    int size;
    songBstNode*root;
};
typedef struct songBinarySearchTree songBinarySearchTree;


struct albumBstNode{
    songBinarySearchTree*songs;
    char*title;
    int year;
    char*singer;
    struct albumBstNode*left;
    struct albumBstNode*right;
};
typedef struct albumBstNode albumBstNode;
struct albumBinarySearchTree{
    int size;
    albumBstNode*root;
};
typedef struct albumBinarySearchTree albumBinarySearchTree;
//===========================================================
//    TREE METHODS
//===========================================================
void moveNext(){
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
int isNumber(char*buffer){
    int n = (int)strlen(buffer);
    for(int i=0;i<n;i++)
        if(!isdigit(buffer[i]))
            return 0;
    return 1;
}
void printError(){
    moveNext();
    printf("***************************************\n");
    printf("*  <HappyPlayer>                      *\n");
    printf("***************************************\n");
    printf("*             .-\"\"\"\"\"\"-.              *\n");
    printf("*           .'          '.            *\n");
    printf("*          /   X      X   \\           *\n");
    printf("*         :                :          *\n");
    printf("*         |                |          *\n");
    printf("*         :    .------.    :          *\n");
    printf("*          \\  '        '  /           *\n");
    printf("*           '.          .'            *\n");
    printf("*             '-......-'              *\n");
    printf("*                                     *\n");
    printf("*[AN ERROR ACCURRED.]                 *\n");
    printf("*[YOU ENTERED AN INVALID OPTION.]     *\n");
    printf("*[PLEASE PRESS ENTER TO GO BACK.]     *\n");
    printf("***************************************\n");
    char*busy = (char*)malloc(sizeof(char)*30);
    scanf("%s",busy);
}
songBstNode*createSongBstNode(char*name,int length){
    songBstNode*temp = (songBstNode*)malloc(sizeof(songBstNode));
    temp->left = NULL;
    temp->right = NULL;
    temp->length = length;
    temp->name = name;
    return temp;
}
albumBstNode*createAlbumBstNode(char*albumTitle,char*singerName,int releaseYear){
    albumBstNode*newAlbum = (albumBstNode*)malloc(sizeof(albumBstNode));
    newAlbum->left = NULL;
    newAlbum->right = NULL;
    newAlbum->singer = singerName;
    newAlbum->songs = (songBinarySearchTree*)malloc(sizeof(songBinarySearchTree));
    newAlbum->songs->size = 0;
    newAlbum->songs->root = NULL;
    newAlbum->title = albumTitle;
    newAlbum->year = releaseYear;
    return newAlbum;
}
songBstNode*getMinSong(songBstNode*songNode){
    songBstNode*temp = songNode;
    while(temp->left!=NULL){
        temp = temp->left;
    }
    return temp;
}
songBstNode*helpAddSong(songBstNode*songNode,char*songName,int songLength,int*error){
    if(songNode){
        int comp = strcmp(songNode->name,songName);
        if(comp>0)
            songNode->left = helpAddSong(songNode->left,songName,songLength,error);
        else if(comp<0)
            songNode->right = helpAddSong(songNode->right,songName,songLength,error);
        else
            *error = 1;
    }
    else{return createSongBstNode(songName,songLength);}
    return songNode;
}
songBstNode*helpRemoveSong(songBstNode*songNode,char*songName,int*error){
    if(songNode){
        int comp = strcmp(songNode->name,songName);
        if(comp>0)
            songNode->left = helpRemoveSong(songNode->left,songName,error);
        else if(comp<0)
            songNode->right = helpRemoveSong(songNode->right,songName,error);
        else{
            *error = 0;
            songBstNode*newSong=NULL;
            songBstNode*temp = songNode;
            if(songNode->left!=NULL&&songNode->right!=NULL){
                newSong = getMinSong(songNode->right);
                newSong->left = songNode->left;
                newSong->right = songNode->right;
                newSong->right = helpRemoveSong(newSong->right,newSong->name,error);
            }
            else if(songNode->left!=NULL){
                newSong = songNode->left;
            }
            else if(songNode->right!=NULL){
                newSong = songNode->right;
            }
            free(temp);
            return newSong;
        }
    }
    return songNode;
}
albumBstNode*getMinAlbum(albumBstNode*albumNode){
    albumBstNode*result = albumNode;
    while(result->left!=NULL){
        result = result->left;
    }
    return result;
}
int checkTitleExist(albumBstNode*albumNode,char*albumTitle){
    if(albumNode){
        if(strcmp(albumNode->title,albumTitle)==0){return 1;}
        return (checkTitleExist(albumNode->left,albumTitle) || checkTitleExist(albumNode->right,albumTitle))? 1 : 0;
    }
    return 0;
}
void songsOfAlbum(songBstNode*songNode){
    if(songNode){
        songsOfAlbum(songNode->left);
        printf("*%s,",songNode->name);
        printf("%d\n",songNode->length);
        songsOfAlbum(songNode->right);
    }
}
albumBstNode*addAlbum(albumBstNode*albumNode,char*albumTitle,char*singerName,int releaseYear){
    if(albumNode){
        if(albumNode->year > releaseYear){
            albumNode->left = addAlbum(albumNode->left,albumTitle,singerName,releaseYear);
        }
        else{
            albumNode->right = addAlbum(albumNode->right,albumTitle,singerName,releaseYear);
        }
    }
    else{return createAlbumBstNode(albumTitle,singerName,releaseYear);}
    return albumNode;
}
albumBstNode*removeAlbum(albumBstNode*albumNode,int*error,char*albumTitle){
    if(albumNode){
        if(strcmp(albumTitle,albumNode->title)==0){
            *error = 0;
            albumBstNode*temp = albumNode;
            albumBstNode*newAlbum = NULL;
            if(albumNode->left!=NULL&&albumNode->right!=NULL){
                newAlbum = getMinAlbum(albumNode->right);
                newAlbum->left = albumNode->left;
                newAlbum->right = albumNode->right;
                newAlbum->right = removeAlbum(newAlbum->right,error,newAlbum->title);
                return newAlbum;
            }
            else if(albumNode->left!=NULL){
                newAlbum = albumNode->left;
            }
            else if(albumNode->right!=NULL){
                newAlbum = albumNode->right;
            }
            free(temp);
            return newAlbum;
        }
        albumNode->left = removeAlbum(albumNode->left,error,albumTitle);
        albumNode->right = removeAlbum(albumNode->right,error,albumTitle);
    }
    return albumNode;
}
void showAllAlbums(albumBstNode*albumNode){
    if(albumNode){
        showAllAlbums(albumNode->left);
        printf("*");
        printf("%s,",albumNode->title);
        printf("%d,",albumNode->year);
        printf("%s\n",albumNode->singer);
        showAllAlbums(albumNode->right);
    }
}
void showAlbumDetails(albumBstNode*albumNode,char*albumTitle){
    if(albumNode){
        if(strcmp(albumTitle,albumNode->title)==0){
            printf("***************************************\n");
            printf("* [SHOWING THE DETAILS OF THE ALBUM]  *\n");
            printf("***************************************\n");
            printf("*Name of the album:%s\n",albumNode->title);
            printf("*Release year of the album:%d\n",albumNode->year);
            printf("*Singer:%s\n",albumNode->singer);
            printf("*Songs:\n");
            songsOfAlbum(albumNode->songs->root);
            return;
        }
        showAlbumDetails(albumNode->left,albumTitle);
        showAlbumDetails(albumNode->right,albumTitle);
    }
}
void addSong(albumBstNode*albumNode,char*albumTitle,char*songName,int songLength,int*error){
    if(albumNode){
        if(strcmp(albumNode->title,albumTitle)==0){
            albumNode->songs->root = helpAddSong(albumNode->songs->root,songName,songLength,error);
            return;
        }
        addSong(albumNode->left,albumTitle,songName,songLength,error);
        addSong(albumNode->right,albumTitle,songName,songLength,error);
    }
}
void removeSong(albumBstNode*albumNode,char*albumTitle,char*songName,int*error){
    if(albumNode){
        if(strcmp(albumNode->title,albumTitle)==0){
            albumNode->songs->root = helpRemoveSong(albumNode->songs->root,songName,error);
            return;
        }
        removeSong(albumNode->left,albumTitle,songName,error);
        removeSong(albumNode->right,albumTitle,songName,error);
    }
}
void showListOfAllSongs(albumBstNode*albumNode){
    if(albumNode){
        showListOfAllSongs(albumNode->left);
        songsOfAlbum(albumNode->songs->root);
        showListOfAllSongs(albumNode->right);
    }
}
void showAlbumsInBetweenParticularYears(albumBstNode*albumNode,int lowerBoundaryYear,int upperBoundaryYear,int*error){
    if(albumNode){
        if(albumNode->year >= lowerBoundaryYear)
            showAlbumsInBetweenParticularYears(albumNode->left,lowerBoundaryYear,upperBoundaryYear,error);
        if(albumNode->year>=lowerBoundaryYear&&albumNode->year<=upperBoundaryYear){
            *error = 0;
            printf("*");
            printf("%s,",albumNode->title);
            printf("%d,",albumNode->year);
            printf("%s\n",albumNode->singer);
        }
        if(albumNode->year <= upperBoundaryYear){
            showAlbumsInBetweenParticularYears(albumNode->right,lowerBoundaryYear,upperBoundaryYear,error);
        }
    }
}
void songsWithParticularLength(songBstNode*songNode,int lowerBoundary,int upperBoundary,char*singer,int*error){
    if(songNode){
        songsWithParticularLength(songNode->left,lowerBoundary,upperBoundary,singer,error);
        if(songNode->length>=lowerBoundary&&songNode->length<=upperBoundary){
            printf("*%s,%d,%s\n",songNode->name,songNode->length,singer);
            *error = 0;
        }
        songsWithParticularLength(songNode->right,lowerBoundary,upperBoundary,singer,error);
    }
}
void showSongsWithParticularLength(albumBstNode*albumNode,int lowerBoundary,int upperBoundary,int*error){
    if(albumNode){
        showSongsWithParticularLength(albumNode->left,lowerBoundary,upperBoundary,error);
        songsWithParticularLength(albumNode->songs->root,lowerBoundary,upperBoundary,albumNode->singer,error);
        showSongsWithParticularLength(albumNode->right,lowerBoundary,upperBoundary,error);
    }
}
//===========================================================
//    INT MAIN
//===========================================================
int main(){
    albumBinarySearchTree*musicBox = (albumBinarySearchTree*)malloc(sizeof(albumBinarySearchTree));
    musicBox->root = NULL;
    musicBox->size = 0;
    while(1){
        moveNext();
        printf("***************************************\n");
        printf("*  <HappyPlayer>                      *\n");
        printf("***************************************\n");
        printf("*    _,,._                            *\n");
        printf("*  ,'     `.__,--.                    *\n");
        printf("* /   .--.        |           ,,,     *\n");
        printf("* | [=========|==|==|=|==|=|==___]    *\n");
        printf("* \\   \"--\"  __    |           '''     *\n");
        printf("*  `._   _,'  `--'                    *\n");
        printf("*     \"\"'                             *\n");
        printf("***************************************\n");
        printf("*-------------------------------------*\n");
        printf("*1. Add an album                      *\n");
        printf("*-------------------------------------*\n");
        printf("*2. Remove an album                   *\n");
        printf("*-------------------------------------*\n");
        printf("*3. Show the list of albums           *\n");
        printf("*-------------------------------------*\n");
        printf("*4. Show detailed information about a *\n");
        printf("*particular album                     *\n");
        printf("*-------------------------------------*\n");
        printf("*5. Add a song to the song list of an *\n");
        printf("*album                                *\n");
        printf("*-------------------------------------*\n");
        printf("*6. Remove a song from the song list  *\n");
        printf("*of an album                          *\n");
        printf("*-------------------------------------*\n");
        printf("*7. Show the list of all songs        *\n");
        printf("*-------------------------------------*\n");
        printf("*8. Query the albums which are/were   *\n");
        printf("*released between particular years    *\n");
        printf("*-------------------------------------*\n");
        printf("*9. Query the songs whose lengths     *\n");
        printf("*are in a particular scope            *\n");
        printf("*-------------------------------------*\n");
        printf("*10. Exit                             *\n");
        printf("*-------------------------------------*\n");
        printf("* [!] PLEASE ENTER AN OPTION:\n");
        char*choice = (char*)malloc(sizeof(char)*80);
        int number=0;
        scanf("%s",choice);
        if(!isNumber(choice)){
            printError();
            continue;
        }
        else{
            number = atoi(choice);
            if(number<1||number>10){
                printError();
                continue;
            }
        }
        char*checkInput = (char*)malloc(sizeof(char)*30);
        switch(number){
            case 1:{//add album
                char*albumTitle = (char*)malloc(sizeof(char)*30);
                char*singerName = (char*)malloc(sizeof(char)*30);
                int releaseYear=0;
                moveNext();
                printf("***************************************\n");
                printf("*  [ADD ALBUM]                        *\n");
                printf("***************************************\n");
                printf("*  PLEASE ENTER THE NAME OF THE ALBUM:\n");
                scanf("%s",albumTitle);
                moveNext();
                printf("***************************************\n");
                printf("*  [ADD ALBUM]                        *\n");
                printf("***************************************\n");
                printf("*PLEASE ENTER THE SINGER NAME:\n");
                scanf("%s",singerName);
                moveNext();
                printf("***************************************\n");
                printf("*  [ADD ALBUM]                        *\n");
                printf("***************************************\n");
                printf("*PLEASE ENTER THE RELEASE YEAR:\n");
                scanf("%s",checkInput);
                if(!isNumber(checkInput)){
                    printError();
                    break;
                }
                else{
                    releaseYear = atoi(checkInput);
                    moveNext();
                }
                if(checkTitleExist(musicBox->root,albumTitle)){
                    printf("***************************************\n");
                    printf("*  [ADD ALBUM]                        *\n");
                    printf("***************************************\n");
                    printf("*  [!]THE ALBUM ALREADY EXIST[!]      *\n");
                    printf("***************************************\n");
                    printf("*  [PLEASE PRESS ENTER TO GO BACK.]   *\n");
                    printf("***************************************\n");
                    scanf("%s",checkInput);
                    break;
                }
                musicBox->root = addAlbum(musicBox->root,albumTitle,singerName,releaseYear);
                musicBox->size++;
                printf("***************************************\n");
                printf("*  [ADD ALBUM]                        *\n");
                printf("***************************************\n");
                printf("*  The album:                         *\n");
                printf("* %-36s*\n",albumTitle);
                printf("* Has been added. Please press ENTER  *\n");
                printf("* to go back to the menu.             *\n");
                printf("***************************************\n");
                scanf("%s",checkInput);
                break;}
            case 2:{//remove album
                moveNext();
                printf("***************************************\n");
                printf("*  [REMOVE ALBUM]                     *\n");
                printf("***************************************\n");
                printf("*  PLEASE ENTER THE NAME OF THE\nALBUM THAT YOU WANTO REMOVE:\n");
                char*albumTitle = (char*)malloc(sizeof(char)*30);
                scanf("%s",albumTitle);
                int error = 1;
                musicBox->root = removeAlbum(musicBox->root,&error,albumTitle);
                moveNext();
                if(error){
                    printf("***************************************\n");
                    printf("*  [REMOVE ALBUM]                     *\n");
                    printf("***************************************\n");
                    printf("*  [!]THE ALBUM DOES NOT EXIST[!]     *\n");
                    printf("***************************************\n");
                    printf("*  [PLEASE PRESS ENTER TO GO BACK.]   *\n");
                    printf("***************************************\n");
                    scanf("%s",checkInput);
                }
                else{
                    musicBox->size--;
                    printf("***************************************\n");
                    printf("*  [REMOVE ALBUM]                     *\n");
                    printf("***************************************\n");
                    printf("*  The album:                         *\n");
                    printf("* %-36s*\n",albumTitle);
                    printf("* Has been successfully deleted.      *\n");
                    printf("* Please press ENTER to go back to    *\n");
                    printf("* the menu.                           *\n");
                    printf("***************************************\n");
                    scanf("%s",checkInput);
                }
                break;}
            case 3:{//show all albums
                moveNext();
                printf("***************************************\n");
                printf("*  [SHOWING ALL THE ALBUMS]           *\n");
                printf("***************************************\n");
                showAllAlbums(musicBox->root);
                if(musicBox->root==NULL){
                    printf("* --none--\n");
                }
                printf("***************************************\n");
                printf("* PLEASE PRESS ENTER TO BACK TO THE   *\n");
                printf("* MENU.                               *\n");
                printf("***************************************\n");
                char*busy = (char*)malloc(sizeof(char)*30);
                scanf("%s",busy);
                break;}
            case 4:{//show album details
                moveNext();
                printf("***************************************\n");
                printf("* [SHOWING THE DETAILS OF THE ALBUM]  *\n");
                printf("***************************************\n");
                printf("* PLEASE ENTER THE NAME OF THE ALBUM\n");
                printf("* THAT YOU WANT TO SEE:\n");
                char*albumTitle = (char*)malloc(sizeof(char)*30);
                scanf("%s",albumTitle);
                if(checkTitleExist(musicBox->root,albumTitle)==0){
                    moveNext();
                    printf("***************************************\n");
                    printf("* [SHOWING THE DETAILS OF THE ALBUM]  *\n");
                    printf("***************************************\n");
                    printf("*  [!]THE ALBUM DOES NOT EXIST[!]     *\n");
                    printf("***************************************\n");
                    printf("*  [PLEASE PRESS ENTER TO GO BACK.]   *\n");
                    printf("***************************************\n");
                    scanf("%s",checkInput);
                    break;
                }
                moveNext();
                showAlbumDetails(musicBox->root,albumTitle);
                printf("***************************************\n");
                printf("*  [PLEASE PRESS ENTER TO GO BACK.]   *\n");
                printf("***************************************\n");
                scanf("%s",checkInput);
                break;}
            case 5:{//addSong
                char*albumTitle = (char*)malloc(sizeof(albumTitle)*30);
                char*songName = (char*)malloc(sizeof(char)*30);
                int songLength = 0;
                moveNext();
                printf("***************************************\n");
                printf("*  [ADD SONG]                         *\n");
                printf("***************************************\n");
                printf("*  PLEASE ENTER THE NAME OF THE ALBUM\n");
                printf("*  THAT WILL CONTAIN THE SONG:\n");
                scanf("%s",albumTitle);
                if(checkTitleExist(musicBox->root,albumTitle)==0){
                    moveNext();
                    printf("***************************************\n");
                    printf("* [ADD SONG]                          *\n");
                    printf("***************************************\n");
                    printf("*  [!]THE ALBUM DOES NOT EXIST[!]     *\n");
                    printf("***************************************\n");
                    printf("*  [PLEASE PRESS ENTER TO GO BACK.]   *\n");
                    printf("***************************************\n");
                    scanf("%s",checkInput);
                    break;
                }
                moveNext();
                printf("***************************************\n");
                printf("*  [ADD SONG]                         *\n");
                printf("***************************************\n");
                printf("*  PLEASE ENTER THE NAME OF THE SONG:\n");
                scanf("%s",songName);
                moveNext();
                printf("***************************************\n");
                printf("*  [ADD SONG]                         *\n");
                printf("***************************************\n");
                printf("*  PLEASE ENTER THE LENGTH OF THE SONG:\n");
                scanf("%s",checkInput);
                if(!isNumber(checkInput)){
                    printError();
                    break;
                }
                else{
                    songLength = atoi(checkInput);
                }
                int error = 0;
                addSong(musicBox->root,albumTitle,songName,songLength,&error);
                moveNext();
                if(error){
                    printf("***************************************\n");
                    printf("*  [ADD SONG]                         *\n");
                    printf("***************************************\n");
                    printf("*  [!]THE SONG ALREADY EXISTS[!]      *\n");
                    printf("***************************************\n");
                    printf("*  [PLEASE PRESS ENTER TO GO BACK.]   *\n");
                    printf("***************************************\n");
                    scanf("%s",checkInput);
                }
                else{
                    musicBox->root->songs->size++;
                    printf("***************************************\n");
                    printf("*  [ADD SONG]                         *\n");
                    printf("***************************************\n");
                    printf("*  The song:                          *\n");
                    printf("* %-36s*\n",songName);
                    printf("* Has been added. Please press ENTER  *\n");
                    printf("* to go back to the menu.             *\n");
                    printf("***************************************\n");
                    scanf("%s",checkInput);
                }
                break;}
            case 6:{//removeSong
                char*albumTitle = (char*)malloc(sizeof(albumTitle)*30);
                char*songName = (char*)malloc(sizeof(char)*30);
                moveNext();
                printf("***************************************\n");
                printf("*  [REMOVE SONG]                       *\n");
                printf("***************************************\n");
                printf("*  PLEASE ENTER THE NAME OF THE ALBUM\n");
                printf("*  THAT CONTAINS THE SONG TO BE REMOVED:\n");
                scanf("%s",albumTitle);
                if(checkTitleExist(musicBox->root,albumTitle)==0){
                    moveNext();
                    printf("***************************************\n");
                    printf("* [REMOVE SONG]                       *\n");
                    printf("***************************************\n");
                    printf("*  [!]THE ALBUM DOES NOT EXIST[!]     *\n");
                    printf("***************************************\n");
                    printf("*  [PLEASE PRESS ENTER TO GO BACK.]   *\n");
                    printf("***************************************\n");
                    scanf("%s",checkInput);
                    break;
                }
                moveNext();
                printf("***************************************\n");
                printf("*  [REMOVE SONG]                      *\n");
                printf("***************************************\n");
                printf("*  PLEASE ENTER THE NAME OF THE SONG:\n");
                scanf("%s",songName);
                int error=1;
                removeSong(musicBox->root,albumTitle,songName,&error);
                moveNext();
                if(error){
                    printf("***************************************\n");
                    printf("*  [REMOVE SONG]                      *\n");
                    printf("***************************************\n");
                    printf("* [!]THE SONG ALREADY DOESNT EXIST[!] *\n");
                    printf("***************************************\n");
                    printf("*  [PLEASE PRESS ENTER TO GO BACK.]   *\n");
                    printf("***************************************\n");
                    scanf("%s",checkInput);
                }
                else{
                    musicBox->root->songs->size--;
                    printf("***************************************\n");
                    printf("*  [REMOVE SONG]                      *\n");
                    printf("***************************************\n");
                    printf("*  The song:                          *\n");
                    printf("* %-36s*\n",songName);
                    printf("* Has been successfully deleted.      *\n");
                    printf("Please press ENTER to go back to menu.*\n");
                    printf("***************************************\n");
                    scanf("%s",checkInput);
                }
                break;}
            case 7:{//showListOfAllSongs
                moveNext();
                printf("***************************************\n");
                printf("* [SHOWING THE LIST OF ALL THE SONGS] *\n");
                printf("***************************************\n");
                showListOfAllSongs(musicBox->root);
                if(musicBox->root==NULL)
                    printf("* --none--\n");
                printf("***************************************\n");
                printf("*  [PLEASE PRESS ENTER TO GO BACK.]   *\n");
                printf("***************************************\n");
                scanf("%s",checkInput);
                break;}
            case 8:{//showAlbumsInBetweenParticularYears
                int lowerBoundYear=0;
                int upperBoundYear=0;
                moveNext();
                printf("***************************************\n");
                printf("*    [SHOWING ALBUMS IN BETWEEN]      *\n");
                printf("*    [    PARTICULAR YEARS     ]      *\n");
                printf("***************************************\n");
                printf("*PLEASE ENTER THE LOWER BOUND YEAR:\n");
                scanf("%s",checkInput);
                if(!isNumber(checkInput)){
                    printError();
                    break;
                }
                else{
                    lowerBoundYear = atoi(checkInput);
                    moveNext();
                }
                printf("***************************************\n");
                printf("*    [SHOWING ALBUMS IN BETWEEN]      *\n");
                printf("*    [    PARTICULAR YEARS     ]      *\n");
                printf("***************************************\n");
                printf("*PLEASE ENTER THE UPPER BOUND YEAR:\n");
                scanf("%s",checkInput);
                if(!isNumber(checkInput)){
                    printError();
                    break;
                }
                else{
                    upperBoundYear = atoi(checkInput);
                    moveNext();
                }
                printf("***************************************\n");
                printf("*    [SHOWING ALBUMS IN BETWEEN]      *\n");
                printf("*    [    PARTICULAR YEARS     ]      *\n");
                printf("***************************************\n");
                if(lowerBoundYear>upperBoundYear){
                    printf("* [!]UPPER BOUND CANNOT BE SMALLER[!] *\n");
                }
                else{
                    int error = 1;
                    showAlbumsInBetweenParticularYears(musicBox->root,lowerBoundYear,upperBoundYear,&error);
                    if(error){
                        printf("* --none--\n");
                    }
                }
                printf("***************************************\n");
                printf("*  [PLEASE PRESS ENTER TO GO BACK.]   *\n");
                printf("***************************************\n");
                scanf("%s",checkInput);
                break;}
            case 9:{//showSongsWithParticularLength
                int lowerBoundYear=0;
                int upperBoundYear=0;
                moveNext();
                printf("***************************************\n");
                printf("*       [SHOWING SONGS WITH]          *\n");
                printf("*       [PARTICULAR LENGTHS]          *\n");
                printf("***************************************\n");
                printf("*PLEASE ENTER THE LOWER BOUND LENGTH:\n");
                scanf("%s",checkInput);
                if(!isNumber(checkInput)){
                    printError();
                    break;
                }
                else{
                    lowerBoundYear = atoi(checkInput);
                    moveNext();
                }
                printf("***************************************\n");
                printf("*       [SHOWING SONGS WITH]          *\n");
                printf("*       [PARTICULAR LENGTHS]          *\n");
                printf("***************************************\n");
                printf("*PLEASE ENTER THE UPPER BOUND LENGTH:\n");
                scanf("%s",checkInput);
                if(!isNumber(checkInput)){
                    printError();
                    break;
                }
                else{
                    upperBoundYear = atoi(checkInput);
                    moveNext();
                }
                printf("***************************************\n");
                printf("*       [SHOWING SONGS WITH]          *\n");
                printf("*       [PARTICULAR LENGTHS]          *\n");
                printf("***************************************\n");
                if(lowerBoundYear>upperBoundYear){
                    printf("* [!]UPPER BOUND CANNOT BE SMALLER[!] *\n");
                }
                else{
                    int error = 1;
                    showSongsWithParticularLength(musicBox->root,lowerBoundYear,upperBoundYear,&error);
                    if(error){
                        printf("* --none--\n");
                    }
                }
                printf("***************************************\n");
                printf("*  [PLEASE PRESS ENTER TO GO BACK.]   *\n");
                printf("***************************************\n");
                scanf("%s",checkInput);
                break;}
            case 10:{
                return 0;
                break;}
            default:{
                printError();
            }
        }
    }
    return 0;
}
