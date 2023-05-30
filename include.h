typedef enum{KEYWORDNODE} nodeEnum;

typedef struct 
{
    char value[50];
}keywordNode;

typedef struct NODE_TYPE_TAG
{
    nodeEnum type;
    keywordNode keyword;

}nodeType;