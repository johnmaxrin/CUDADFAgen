#include "regextodfa.h"

StateList *createStateList()
{
    StateList *stateList = (StateList *)calloc(1, sizeof(StateList));
    stateList->currentState = 0;
    stateList->states = (State **)malloc(sizeof(State) * 10); // Global
    return stateList;
}

void sort(int *a, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = i + 1; j < size; ++j)
        {
        
            if(a[i] > a[j])
            {
                int temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }
    }
}

int isState(int *array, StateList *stateList)
{
    int isPresent = 0, arraySize = 0, listSize = 0;

    if (stateList->currentState == 0)
        return 0;

    while (array[arraySize++] != 0)
        ;
    --arraySize;

    int i, j;
    for (i = 0; i < stateList->currentState; ++i) // For each list in stateList->states
    {

        listSize = 0;
        isPresent = 0;

        while (stateList->states[i]->list[listSize++] != 0)
            ;

        --listSize;

        sort(array,arraySize);
        sort(stateList->states[i]->list,listSize);

        if (listSize == arraySize)
        {
            for (j = 0; j < arraySize; ++j)
            {
                if (array[j] != stateList->states[i]->list[j])
                    break;
            }

            if (j == arraySize)
            {
                if (i == 0)
                    return 99; // Global
                else
                    return stateList->states[i]->s;
            }
        }
    }

    return 0;
}

State *createState(int *array)
{
    State *state = (State *)malloc(sizeof(State));
    state->list = array;
    state->isMarked = 0;
    state->s = 0;

    return state;
}

State *findUnmarkedState(StateList *stateList)
{
    State *unmarkedState;
    int i = 0, flag = 0;

    while (i < stateList->currentState)
    {
        if (!stateList->states[i]->isMarked)
            return stateList->states[i];

        ++i;
    }

    // Add comment here to debug. 
    return NULL;
}

void createInputSymbolTable(Node * root, int *inputSymbol, int *count)
{
    
    if(root == NULL)
        return;
    
    createInputSymbolTable(root->left,inputSymbol,count);
    inputSymbol[root->pos] = root->uid;
    createInputSymbolTable(root->right,inputSymbol,count);    
}

void createRegexAlphArray(int *symbolList, int *regexAlphArray, int size)
{
    for(int i=0; i<size; ++i)
        regexAlphArray[symbolList[i]] = i;
}


void createSymbolList(int *inputSymbol, int *symbolList, int *count)
{
    int i = 1,tempi=0,symbolListSize = 0,j=0,tempCount = 0;
    int temp[20] = {0}; // Global

    for(int i=0; i<20; ++i) // Global
        symbolList[i] = -1; 

    while (inputSymbol[i] != -1)
    {
        temp[j++] = inputSymbol[i];
        ++i;
    }

    --i;


    sort(temp,i);
    while(tempi<i)
    {
        if(symbolList[tempCount-1]<temp[tempi])   //Potential error prone area. 
            symbolList[tempCount++] = temp[tempi];
        ++tempi;
    }

    *count = tempCount;

//    *count = symbolListSize;   
//    printf("Size: %d\n",symbolListSize); 
    
}




void makeZero(int *array, int size)
{
    for (int i = 0; i < size; ++i)
        array[i] = 0;
}
int **buildTable(int *startState, int (*followPos)[50], StateList *stateList, int *inputSymbol,int *symbolList,int count)
{
    int **dTran = (int **)calloc(50, sizeof(int *));
    for (int i = 0; i < 50; ++i)
        dTran[i] = (int *)calloc(1, sizeof(int) * 2);

    if (stateList->currentState == 0) // Adding first state
    {
        State *newState = createState(startState); // Create new state.
        newState->s = 1;
        stateList->states[stateList->currentState++] = newState;
    }

    int sizeofUnmarkedStateArray = 0, temp[10], tempsize, newStateArraySize; // Global

    while (findUnmarkedState(stateList) != NULL)
    {
        State *unmarkedState = findUnmarkedState(stateList), *newState;
        unmarkedState->isMarked = 1;

        sizeofUnmarkedStateArray = 0;
        while (unmarkedState->list[++sizeofUnmarkedStateArray] != 0)
            ;

        // For each input symbol x. In our case right now we have 2 symbols. a and b so:
        for (int i = 0; i < count; ++i)
        {
            makeZero(temp, 10); // Global
            tempsize = 0;
            for (int j = 0; j < sizeofUnmarkedStateArray; ++j)
                if (inputSymbol[unmarkedState->list[j]] == symbolList[i])
                    temp[tempsize++] = unmarkedState->list[j]; // eg. 1 2 3 (positions in state representing each symbols)

            int *newStateArray = makeunion(tempsize, temp);
            newStateArraySize = 0;
            while (newStateArray[newStateArraySize++] != 0)
                ;
            --newStateArraySize;

            int a = 0;
            if (!(a = isState(newStateArray, stateList))) // It is a new state array.
            {

                newState = createState(newStateArray);
                newState->s = stateList->currentState + 1;
                stateList->states[stateList->currentState++] = newState;

                dTran[unmarkedState->s][symbolList[i]] = newState->s;
            }

            else
            {

                if (a == 99)
                    dTran[unmarkedState->s][i] = 1;
                else
                    dTran[unmarkedState->s][i] = a;
            }

        }
    }

    return dTran;
}

int isDuplicate(int *array, int element)
{
    int size = 0, isDupli = 0;
    while (array[size++] != 0)
        ;
    --size;
    for (int i = 0; i < size; ++i)
    {
        if (array[i] == element)
        {
            isDupli = 1;
            break;
        }
    }

    return isDupli;
}

void merge(int *mergeArray, int *array1) // Global
{

    int size0 = 0, size1 = 0;

    while (mergeArray[size0++] != 0)
        ;

    --size0;

    while (array1[size1++] != 0)
        ;
    for (int i = 0; i < size1; ++i)
    {
        if (!isDuplicate(mergeArray, array1[i]))
            mergeArray[size0++] = array1[i];
    }
}

int *makeunion(int nargs, int *tempArray)
{

    int *sentArray = (int *)calloc(10, sizeof(int)); // Global
    for (int i = 0; i < nargs; ++i)
        merge(sentArray, followPos[tempArray[i]]);

    return sentArray;
}
