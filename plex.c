// Auto generated lexical analyser for MA14
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<omp.h>

struct Regex
{
	int **table;
	int *end;
	int *symbolList;
	int *alpArray;
};

enum Keywords
{
	NONE,
	APE,
	JOHN
};

#ifndef REGEX1
#define REGEX1 11
#endif

static int dfaTable[9][7] = {
	{4, -1, -1, -1, 1, -1, -1},
	{-1, -1, -1, -1, -1, 2, -1},
	{-1, -1, -1, -1, -1, -1, 3},
	{-1, -1, -1, -1, -1, -1, -1},
	{-1, 5, -1, -1, -1, -1, -1},
	{-1, -1, 6, -1, -1, -1, -1},
	{-1, -1, -1, 7, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1},

};

static int outputArray[50] = {
	0, 0, 0, 1, 0, 
	0, 0, 2, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	
};

static int alpArray[26] = {
	4, -1, -1, -1, 6, 
	-1, -1, 2, -1, 0, 
	-1, -1, -1, 3, 1, 
	5, -1, -1, -1, -1, 
	-1, -1, -1, -1, -1, 
	-1, 
};

static int dTran[1][50][50] = {
						{
							{0, 0},
							{2, 1},
							{2, 3},
							{2, 4},
							{2, 1},
							{0, 0},
							{0, 0},
							{0, 0},
							{0, 0},
							{0, 0}
						},
};

static int symbolList[1][26] = {
						{ 0,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0 },

};

static int regexAlphArray[1][26] = {
						{ 0,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },

};

static int counts[1][1] = {
						{ 2 }
};

static int finalStates[1][5] = {
						{ 4 }
};


int checkKeyword(char *temp)
{
	int cstate = 0;
	for(int i=0; temp[i] != '\0'; ++i)
	{
		int ch = alpArray[temp[i]-'a'];
		if(ch == -1 || cstate == -1)
			return 0;
		
		cstate = dfaTable[cstate][ch];
	}
	
	return outputArray[cstate];
}

int checkRegex(char *temp){

int i=0, currentState = 1;
while (temp[i] != '\0')
{
	    currentState = dTran[0][currentState][symbolList[0][regexAlphArray[0][temp[i]-'a']]];
	++i;
}

if(finalStates[0][0] == currentState)
	return REGEX1;
else
	return 0;
}

int check(char *str)
{
	int (*functions[])() = { checkKeyword, checkRegex };
	int result = 0;

	for(int i=0; i<2; ++i)
	{
		result = functions[i](str);
		if(result!=0)
			return result;
	}
	
	return 0;
}


int main(int argc, char *argv[])
{
	double start, end;

	char *buffer[50];


	FILE *file = fopen(argv[1],"r"), *file2 = fopen("result.txt","w");
	
	if(file == NULL)
		{printf("[Error] No file named %s\n",argv[1]); return 0;}

	else
	{

		long int count = 0;
		char tempStr[50];
		// Read and store in buffer
		while(fscanf(file,"%s",tempStr) == 1)
			count++;

		char **buffer;
		
		buffer = (char **)malloc(sizeof(char *) * count);
		for(int i=0; i<count; ++i)
			buffer[i] = (char *)malloc(sizeof(char) * 50);

		int tempcount = 0;
		fseek(file,0,0);
		while(fscanf(file,"%s",tempStr) == 1)
			strcpy(buffer[tempcount++],tempStr);


		int *res;
		res = (int *)malloc(sizeof(int) * count);
		
		start = omp_get_wtime();

		int nthrds = atoi(argv[2]);

		#pragma omp parallel num_threads(nthrds)
		{
			int threads = omp_get_num_threads();		
			for(int i=0; i<count; i += threads)
					check(buffer[i]);	
		}
		end = omp_get_wtime();
		
		/*int temp = 0;
		#pragma omp parallel num_threads(1) private(temp, tempStr)
		{
			while (fscanf(file,"%s",tempStr) == 1)
			{
				#pragma omp task
				{
					temp = check(tempStr);
					fprintf(file2,"%d\n",temp);
				}
			
			}
		}*/
		
		free(res);
	}	



	printf("\nCompute time: %f seconds\n",end - start);

	fclose(file);
	return 0;
}