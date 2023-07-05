#ifndef REGEX_TO_DFA
#define REGEX_TO_DFA


// Includes
#include <stdio.h>
#include <stdlib.h>
// End

// Strucures 
typedef struct
{
    int *list;
    int s;
    int isMarked;
} State;

typedef struct
{
    State **states;
    int currentState;
} StateList;

typedef struct Node
{
    char data;
    int nullable;
    int pos;
    struct Node *left;
    struct Node *right;
    int uid;
    int *firstPos;
    int *lastPos;
    int *followPos;
}Node;
// End

// Function declaration
Node *createNode(char data);
StateList *createStateList();
State *createState(int *);
void buildPosArray(int *);
int **buildTable(int *, int (*)[50], StateList *, int *, int *, int);
void createSymbolList(int *, int *, int *);
void createInputSymbolTable(Node *, int *, int *);
void createRegexAlphArray(int *, int *, int);
int checkRegex(char *,int **, int *, int *, int, int);
int isState(int *, StateList *);
int isDuplicate(int *, int);
int isAlpha(char);
void incrementState(StateList *);
State *findUnmarkedState(StateList *);
int *makeunion(int, int *);
void initFollowPos();
char *augment(char *);
char *postfix(char *);
int precedence(char);
int *genFirstPos(Node *);
int *genLastPos(Node *);
void genFollowPos(Node *);
Node *generateSyntaxTree(FILE *, char *);
void markPos(Node *);
void sort(int *, int);
void mergeSet(int *, int *, int *);
void merge(int *, int *);
void makeZero(int *, int);
int markNullable(Node *);
extern int pos, followPos[50][50]; // Change this in global
extern char pattern[50], augmentedPattern[50], postfixString[50];
// void printFollow();  Debug Materials
// void printTree(Node *); Debug Materials
// End

#endif