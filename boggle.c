/* Ciara Routly */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 4

const int DIRECTIONS = 8;
const int DX[] = {-1,-1,-1,0,0,1,1,1};
const int DY[] = {-1,0,1,-1,1,-1,0,1};

// Trie struct
struct trie {
    int isWord;
    struct trie* next[26];
};

// Function prototypes for trie
struct trie* init();
void insertToTrie(struct trie* T, char words[], int d);
int searchMyTrie(struct trie* T, char words[], int d);
void freeMyDictionary(struct trie* T);

// Prototypes for finding words in board
char* GenerateWord(char** board, int row, int column, int direction, int length);
int torun(int horiz, int diag);

int main(){
    
    int num_words, a, b, c, d, i, j;
    int num_cases;
    
    // Call initilize trie
    struct trie* Dictionary = init();
    
    // Define & open dictionary
    FILE * fp1 = fopen("dictionary.txt", "r");
    
    // Base case for dictionary
    if(fp1 == NULL){
        printf("File cannot be opened.\n");
        perror("fopen");
        exit(1);
    }
    // Read in number of words, n
    fscanf(fp1, "%d", &num_words);
    
    // Read in/insert each word into trie
    for(a=0;a<num_words;a++){
        
        char words[100];
        
        fscanf(fp1, "%s", words);
        insertToTrie(Dictionary, words, 0);
    }
    
    // Portion of code to read in board
    scanf("%d", &num_cases);
    for(i=0;i<num_cases;i++){
        
        // Allocate space for board
        char** board = malloc(sizeof(char*) * SIZE);
        for(j=0;j<SIZE;j++){
            // Scan in each string
            board[j] = malloc(sizeof(char) * (SIZE + 1));
            scanf("%s", board[j]);
        }
        
        // Print for each run of the game
        printf("Words for game #%d:\n", i+1);
        
        // Try all 16 directions beginning at each starting letter. MIN=3, MAX=16
        for(a=0;a<SIZE;a++){
            for(b=0;b<SIZE;b++){
                for(c=0;c<DIRECTIONS;c++){
                    for(d=3;d<=16;d++){
                        
                        // Call function to generate all possible words
                        char* PossWord = GenerateWord(board, a, b, c, d);
                        // If possible word is generated, then search trie for word
                        if(PossWord != NULL){
                            
                            // When a word is found, we print it
                            if(searchMyTrie(Dictionary, PossWord, 0))
                                
                                printf("%s\n", PossWord);
                            
                            /* if(PossWord == NULL){
                                printf("No possible words can be formed.");
                            } */
                        }
                    }
                }
            }
        }
    
    int k, f;
    // Beginning to form prefixes
    for(j=0;k<SIZE;j++){
        for(f=0;f<SIZE;f++){
            for(a=0;a<DIRECTIONS;a++){
            for(k=0;k<4;k++){
                char* prefix = GenerateWord(board, j, f, a, k);
                if(prefix != NULL){
                    if(searchMyTrie(Dictionary, prefix, 0)){}
                        //printf("%s\n", prefix);
                        }
                    }
                }
            }
        }
    }
    // Close file and free all memory pointed to by tree
    fclose(fp1);
    freeMyDictionary(Dictionary);
    return 0;
}

// Function that generates all possible words in board
char* GenerateWord(char** board, int row, int column, int direction, int length){

    // Checking bounds. No words can be formed, return NULL
    if (!torun(row + (length-1) * DX[direction], column+(length-1) * DY[direction])){
        
        return NULL;
    }
    
    // Allocate space for where our word solutions will be stored
    char* solution = malloc(sizeof(char) * (length+1));
    
    int j;
    
    // Store all letters into char* solution
    for(j=0;j<length;j++)
        
        solution[j] = board[row + j * DX[direction]][column + j * DY[direction]]; solution[length] = '\0';
        return solution;
    
        // Recursive call
        return GenerateWord(board, row+1, column+1, direction, length +1);
}

// Function to check bounds
int torun(int horiz, int diag) {
    
    return horiz >= 0 && diag >= 0 && horiz < 4 && diag < 4;
    
}

// Initilize trie
struct trie* init(){
    
    struct trie* myT = malloc(sizeof(struct trie));
    myT->isWord = 0;
    
    int i;
    //set each ptr to NULL
    for(i=0;i<26;i++){
        myT->next[i] = NULL;
    }
    // Return ptr to new root
    return myT;
}

// Func to insert words into trie
void insertToTrie(struct trie* T, char words[], int d){
    
    // Insert word in end
    if(d == strlen(words)){
        T->isWord = 1;
        return;
    }
    
    // Advance to next spot. If it does not exist, create it
    int nextIndexPos = words[d] - 'a';
    if (T->next[nextIndexPos] == NULL){
        T->next[nextIndexPos] = init();
    }
    
    // Call
    insertToTrie(T->next[nextIndexPos], words, d+1);
}

// Func to search through trie
int searchMyTrie(struct trie* T, char words[], int d){
    
    // Insert word
    if (d == strlen(words)){
        return T->isWord;
    }
    
    // If next spot does not exist, the word is not in the dictionary
    int nextIndex = words[d] - 'a';
    if (T->next[nextIndex] == NULL){
        return 0;
    }
    
    // Recursive call
    return searchMyTrie(T->next[nextIndex], words, d+1);
    
}

// Func to free all memory that is being pointed to
void freeMyDictionary(struct trie* T){
    
    int i;
    
    for(i=0;i<26;i++){
        if(T->next[i] != NULL){
            freeMyDictionary(T->next[i]);
        }
    }
    
    free(T);
}
