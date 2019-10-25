/*
 *  Hash Table program
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 *  Struct to set up and maintain linked lists for multiple items
 *  with the same key
 */
typedef struct names{
    char name[256];
    struct names * next;
}node;

/* Array to hold the linked lists that make up the hash table */
node *hashTable[1000];

/* Chosen size of the hash table */
int hashTableSize = 100;

/* Uses the sum of the ASCII values to create a key */
int stringToKey(char name[]){
    int c,s = 0;
    for(int i = 0; i < stringLength(name); i++){
        c = name[i];
        s = s + c;
    }
    return s;
}

/* Calculates the length of a given string */
int stringLength(char *s){
    int i = 0;
    while(s[i] != '\0'){
        i++;
    }
    return i;
}

/* returns false if the strings are not the same */
bool stringCompare(char *s, char *s2){
    if(stringLength(s)!=stringLength(s2)){
        return false;
    }
    for(int i = 0; i < stringLength(s); i++){
        if(s[i] != s2[i]){
            return false;
        }
    }
    return true;
}

/* Adds an element to the HashTable */
void  addElement(char nname[]){
    int ascii, key;
    node *n, *n1;
    n = (node *)malloc(sizeof(node));
    strcpy(n->name,nname);
    n->next = NULL;
    ascii = stringToKey(nname);
    /* Ensures the key is within the HashTable itself */
    key = ascii % hashTableSize;
    /* If there isnt already a value in the hash table, enter the value */
    if(hashTable[key] == NULL){
        hashTable[key] = n;
    }
    else{
        /* Gets to the end of the linked list to add the value in the correct place */
        for(n1 = hashTable[key]; n1->next != NULL; n1 = n1->next);
        n1->next = n;
    }
    
}

/* Function to remove element from the HashTable */
void  removeElement(char nname[]){
    int ascii = stringToKey(nname);
    /* Finds the element by generating the key */
    int key = ascii % hashTableSize;
    node *n, *n1;
    
    if(stringCompare(hashTable[key]->name, nname)){
        n = hashTable[key];
        hashTable[key] = hashTable[key]->next;
        /* Frees the node to remove the element */
        free(n);
        return;
    }
    
    /* Readjusts for removal */
    for(n=hashTable[key]; n->next != NULL; n=n->next){
        n1 = n->next;
        if(stringCompare(n1->name,nname)){
            n->next = n1->next;
            free(n1);
            break;
        }
    }
}

/* Searches for a String in the HashTable */
bool searchElement(char nname[]){
    int ascii = stringToKey(nname);
    int key = ascii % hashTableSize;
    node *n;
    /* Goes through every item in the linkedList for the key */
    for(n = hashTable[key]; n!= NULL; n = n-> next){
        if(strcmp(n->name, nname) == 0){
            return true;
        }
    }
    return false;
}

/* Adds a signle char to the end of a string */
void append(char* s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
}

/* Imports names from names.txt to add to the HashTable */
void importNames(){
    char names[6000][256];
    char * filename = "names.txt";
    FILE * fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Can't find file. Continuing without adding names to table\n");
        return;
    }
    char c;
    int count = 0;
    char constructor[50];
    while((c = fgetc(fp)) != EOF)
    {
        if(c == ',' || c == '\n'){
            strcpy(names[count], constructor);
            strcpy(constructor,"");
            count++;
        }
        else{
            if(c != '"'){
                append(constructor,c);
            }
        }
    }
    strcpy(names[count], constructor);
    
    /* Adds all names to the HashTable */
    for(int i = 0; i < count; i++){
        addElement(names[i]);
        i++;
    }
    printf("The names from \"names.txt\" have been imported and added to the hashtable.\nThere are %d names.\n",count);
    fclose(fp);
}

/*
 *  Main function to import names and get user input
 */
int main(){
    
    /* Ensueres every entry of the hash table starts as null */
    for(int i = 0; i < hashTableSize; i++){
        hashTable[i] = NULL;
    }
    /* Imports the names from names.txt */
    importNames();
    
    /* Array to store user input */
    char input[256];
    char response[10];
    
    /* Gets initial user input */
    char statement[] = "What would you like to do?\n1. Add a string.\n2. Delete a string.\n3. Search for a string.\n";
    
    printf("%s",statement);
    scanf("%s",response);
    
    /* Allows user to continue altering the hashtable */
    while(true){
        if(stringCompare(response, "1")){
            
            printf("Enter the string you would like to add: ");
            scanf("%s",input);
            addElement(input);
            //addElement()
        }
        if (stringCompare(response, "2")){
            /* Uses searchElement to search for the element before attempting to delete */
            printf("Enter the word you would like to remove: ");
            scanf("%s",input);
            if(searchElement(input)){
                removeElement(input);
                printf("Removed.");
            }
            else{
                printf("The word you entered could not be found\n");
            }
        }
        if(stringCompare(response, "3")){
            printf("Enter the name of the string you want to search for: "); scanf("%s", input);
            bool exists = searchElement(input);
            if(exists){
                printf("%s exists in the table\n",input);
            }
            else{
                printf("%s doesn't exist in the table\n",input);
            }
        }
        /* Deals with invalid input */
        else{
            printf("Please enter either 1, 2 or 3.");
        }
        printf("%s",statement);
        scanf("%s",response);
    }
    return 0;
}
