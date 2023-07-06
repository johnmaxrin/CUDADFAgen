/*
a|b*.a.a.b.#
ab*|a.a.b.#.
*/

#include "regextodfa.h"

int pos = 0, followPos[50][50];
char pattern[50], augmentedPattern[50], postfixString[50];

char *augment(char *pattern)
{
    int i = 0, ai = 0;
    while (pattern[i] != '\0')
    {

        if (isAlpha(pattern[i]) && isAlpha(pattern[i + 1]))
        {
            augmentedPattern[ai++] = pattern[i];
            augmentedPattern[ai++] = '.';
        }
        else if (pattern[i] == '*')
        {
            augmentedPattern[ai++] = pattern[i];
            augmentedPattern[ai++] = '.';
        }
        else
        {
            augmentedPattern[ai++] = pattern[i];
        }

        ++i;
    }

    augmentedPattern[ai++] = '.';
    augmentedPattern[ai++] = '#';
    augmentedPattern[ai++] = '\0';
    return augmentedPattern;
}



char *postfix(char *pattern)
{
    int i = 0;
    char stack[50];
    int top = -1, psi = 0;

    while (pattern[i] != '\0')
    {
        if (isAlpha(pattern[i]))
            postfixString[psi++] = pattern[i];

        else if (pattern[i] == '(')
            stack[++top] = pattern[i];

        else if (pattern[i] == ')')
        {
            while (stack[top] != '(')
            {
                postfixString[psi++] = stack[top];
                --top;
            }
            --top;
        }
        else
        {
            while ((top != -1) && precedence(pattern[i]) <= precedence(stack[top]))
            {
                postfixString[psi++] = stack[top--];
            }
            stack[++top] = pattern[i];
        }

        ++i;
    }

    while (top != -1)
        postfixString[psi++] = stack[top--];

    return postfixString;
}

Node *generateSyntaxTree(char *pattern)
{
    int i = 0, top = 0;
    Node *stack[50];

    while (pattern[i] != '\0')
    {
        if (isAlpha(pattern[i]))
            stack[top++] = createNode(pattern[i]);

        else if (pattern[i] == '*')
        {
            Node *temp = createNode('*');
            temp->left = stack[top - 1];
            stack[top - 1] = temp;
        }
        else
        {
            Node *temp = createNode(pattern[i]);
            temp->left = stack[top - 2];
            temp->right = stack[top - 1];
            stack[top - 2] = temp;
            top = top - 1;
        }

        ++i;
    }

    return stack[top - 1];
}

int isAlpha(char a)
{
    if ((a >= 'a' && a <= 'z') || a == '#')
        return 1;
    else
        return 0;
}

Node *createNode(char data)
{
    Node *node = (struct Node *)malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->nullable = 0;
    node->pos = 0;
    node->firstPos = (int *)calloc(50, sizeof(int)); // Add Global Here
    node->lastPos = (int *)calloc(50, sizeof(int));  // Add Global Here

    if(isAlpha(data) && data != '#')
            node->uid =  data - 'a';
    else
        node->uid = -1;

    return node;
}


/* Debug Materials */

/*void printTree(Node *root)
{
    if (root == NULL)
        return;

    printTree(root->left);
    printf("%c(POS:%d NULL:%d)\n", root->data, root->pos, root->nullable);

    printf("First Set\n");
    int i = 0;
    while (root->firstPos[i] != 0)
        printf("%d ", root->firstPos[i++]);
    printf("\n\n");

    printTree(root->right);
}*/

int precedence(char a)
{
    if (a == '*')
        return 2;
    else if (a == '|' || a == '.')
        return 1;
    else
        return 0;
}

void markPos(Node *root)
{
    if (root == NULL)
        return;
    markPos(root->left);
    if (isAlpha(root->data))
        root->pos = ++pos;
    markPos(root->right);
}

int markNullable(Node *root)
{
    if (root == NULL)
        return 0;

    else if (root->pos != 0)
    {
        root->nullable = 0;
        return 0;
    }
    else if (root->data == '*')
    {
        root->nullable = 1;
        return 1;
    }
    else if (root->data == '|')
    {
        root->nullable = markNullable(root->left) || markNullable(root->right);
        return root->nullable;
    }
    else if (root->data == '.')
    {
        root->nullable = markNullable(root->left) && markNullable(root->right);
        return root->nullable;
    }
    else
        return 0;
}

int *genFirstPos(Node *root)
{

    if (root == NULL)
        return NULL;

    else if (root->pos != 0)
    {

        int temp = 0;
        temp = root->pos;
        root->firstPos[0] = temp;

        return root->firstPos;
    }

    else if (root->data == '*')
    {
        int *leftset = genFirstPos(root->left);
        mergeSet(leftset, NULL, root->firstPos);

        return root->firstPos;
    }

    else if (root->data == '|')
    {
        int *leftset = genFirstPos(root->left);
        int *rightset = genFirstPos(root->right);

        mergeSet(leftset, rightset, root->firstPos);
        return root->firstPos;
    }
    else if (root->data == '.')
    {
        int *leftset = genFirstPos(root->left);
        int *rightset = genFirstPos(root->right);
        if (root->left->nullable)
        {
            mergeSet(leftset, rightset, root->firstPos);
            return root->firstPos;
        }
        else
        {

            root->firstPos = leftset;
            return root->firstPos;
        }
    }
}

int *genLastPos(Node *root)
{

    if (root == NULL)
        return NULL;

    else if (root->pos != 0)
    {

        int temp = 0;
        temp = root->pos;
        root->lastPos[0] = temp;

        return root->lastPos;
    }

    else if (root->data == '*')
    {
        int *leftset = genLastPos(root->left);
        mergeSet(leftset, NULL, root->lastPos);

        return root->lastPos;
    }

    else if (root->data == '|')
    {
        int *leftset = genLastPos(root->left);
        int *rightset = genLastPos(root->right);

        mergeSet(leftset, rightset, root->lastPos);
        return root->lastPos;
    }
    else if (root->data == '.')
    {
        int *leftset = genLastPos(root->left);
        int *rightset = genLastPos(root->right);
        if (root->right->nullable)
        {
            mergeSet(leftset, rightset, root->lastPos);
            return root->lastPos;
        }
        else
        {

            root->lastPos = rightset;
            return root->lastPos;
        }
    }
}

void mergeSet(int *left, int *right, int *dest)
{
    int ls = 0, rs = 0, isCopy = 0, k = 0;
    while (left[ls] != 0)
        ++ls;

    if (right != NULL)
        while (right[rs] != 0)
            ++rs;

    for (int i = 0; i < ls; ++i)
        dest[i] = left[i];

    k = ls;
    if (right != NULL)
        for (int i = 0; i < rs; ++i)
        {
            isCopy = 0;
            for (int j = 0; j < ls; ++j)
            {
                if (right[i] == left[j])
                    isCopy = 1;
                break;
            }

            if (!isCopy)
                dest[k++] = right[i];
        }
}

void initFollowPos() // Change this in global.
{
    for (int i = 0; i < 50; ++i)
    {
        for (int j = 0; j < 50; ++j)
        {
            followPos[i][j] == 0;
        }
    }
}

void genFollowPos(Node *root)
{

    if (root == NULL)
        return;

    else if (root->data == '.')
    {
        int *c1LastPos = root->left->lastPos;
        int *c2FirstPos = root->right->firstPos;

        int i = 0, j = 0, k = 0;
        while (c1LastPos[i] != 0)
        {
            j = 0;

            k = 0;
            while (followPos[c1LastPos[i]][k++] != 0)
                ;

            --k;

            while (c2FirstPos[j] != 0)
            {
                followPos[c1LastPos[i]][k] = c2FirstPos[j];
                ++k;
                ++j;
            }

            ++i;
        }
    }

    else if (root->data == '*')
    {
        int *firstPos = root->firstPos;
        int *lastPos = root->lastPos;

        int i = 0, k = 0, j = 0;
        while (lastPos[i] != 0)
        {
            k = 0;
            while (followPos[lastPos[i]][k++] != 0)
                ;
            --k;

            j = 0;
            while (lastPos[j] != 0)
            {
                followPos[lastPos[i]][k] = firstPos[j];
                ++k;
                ++j;
            }
            ++i;
        }
    }
    genFollowPos(root->left);
    genFirstPos(root->right);

    return;
}




/*Debug Materials */

/* void printFollow()
{
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            printf("%d ", followPos[i][j]);
        }

        printf("\n");
    }
    return;
} */


int checkRegex(char *string, int **dTran, int *symbolList, int *regexAlphArray, int size, int finalState)
{

    int i=0, currentState = 1;
    while (string[i] != '\0')
    {   
        currentState = dTran[currentState][symbolList[regexAlphArray[string[i]-'a']]];
        ++i;
    }

    if(finalState == currentState)
        return 1;
    

    return 0;
}

void generateRegex(FILE *file, char *pattern)
{

    int **dTran;
    char *newPattern = augment(pattern);
    char *postfixPattern = postfix(newPattern);
    Node *root = generateSyntaxTree(postfixPattern);

    markPos(root);
    markNullable(root);

    genFirstPos(root);
    genLastPos(root);
    genFollowPos(root);

    // Table Generating phase

    StateList *stateList = createStateList();
    //int inputSymbol[8] = {0, 0, 1, 0, 1, 1, 6};
    int inputSymbol[20] = {0}, symbolList[26];  //Global

    // Initializing input Symbol array with -1. 
    for(int i=0; i<20; ++i)
        inputSymbol[i]  = -1;

    int count=0;

    createInputSymbolTable(root,inputSymbol,&count);
    createSymbolList(inputSymbol,symbolList,&count);

    int *start = root->firstPos;

    int regexAlphArray[26] = {0};
    for(int i=0; i<26; ++i)
        regexAlphArray[i] = -1;
    createRegexAlphArray(symbolList,regexAlphArray,count); 

    dTran = buildTable(start, followPos, stateList,inputSymbol, symbolList,count);


    printf("\n");
    for(int i=0; i<10; ++i)
    {
        for(int j=0; j<count; ++j)
        {
            printf("%d ",dTran[i][j]);
        }
        printf("\n");
    }

    printf("\nEnter a string to check\n");
    char string[50];
    scanf("%s",string);

    int res = checkRegex(string,dTran,symbolList,regexAlphArray,count,stateList->currentState);

    if(res)
        printf("Accepted!\n");
    else
        printf("Rejectd!\n");
    

    return;
}
