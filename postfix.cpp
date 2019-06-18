// Program that transforms normal form of mathematical expressions to
// postfix (Polish) form...
// Author : Merdan Durdiyev, 25/10/2000
// The info is extracted from <postfix.txt> file....

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 100

// Prototypes
int priority (char);                  // Function which returns Priorities
char *toPostfix (char *);             // Transforming to Postfix form
void fillTheValues (void);            // Getting values of vriables from file
float value (char);                   // Func. than returns value of a var.
float calculate (float, float, char); // Calculating an expression
float sumItUp (char *);               // Evaluating Postfix form

// Global Variables
float values[MAX][2];

void main (void)
{
	FILE *f=fopen("postfix.txt","r");
	if (f==NULL)
	{
		printf ("Zapor v Vhodnom Fayle!!!\n");
		exit(1);
	}

	char s[MAX];
	char res[MAX];

	fscanf(f, " %s", s);
	fclose(f);

	fillTheValues();

	printf ("\nThe formula [%s] ", s);
	strcpy(res, toPostfix(s));
	printf ("is written in postfix form as [%s]\n", res);
	printf ("And the result for values:\n");

	for (int i=0; values[i][0]>0; i++)
	{
		printf ("%c = %.1f\n", (int)values[i][0], values[i][1]);
	}

	for (i=0; s[i]; i++)
	{
		if ((s[i]>='A' && s[i]<='Z') || (s[i]>='z' && s[i]<='z'))
		{
			printf ("%.1f",value(s[i]));
		}
		else printf ("%c",s[i]);
	}
	printf (" = ");
	printf ("%.1f\n", sumItUp(res));
}

char *toPostfix (char *s)
{
	char stack[MAX/2];
	char result[MAX];
	int j = 0;
	int c = 0;

	stack[0] = 0;
	result[0] = 0;

	for (int i=0; i<strlen(s); i++)
	{
		if (s[i]=='(')
		{
			j++;
			stack[j]=s[i];
		}
		else if (s[i]==')')
		{
			while (stack[j] != '(')
			{
				result[c++]=stack[j];
				j--;
			}
			j--;
		}
		else if ((s[i]>='A' && s[i]<='Z') || (s[i]>='z' && s[i]<='z'))
		{
			result[c++] = s[i];
		}
		else if (s[i]=='+' || s[i]=='-' || s[i]=='*' || s[i]=='/' || s[i]=='^')
		{
			if (j>0)
			while (priority(s[i])<=priority(stack[j]) && stack[j])
			{
				result[c++] = stack[j];
				j--;
			}
			j++;
			stack[j] = s[i];
		}
		else
		{
			printf ("\n!!!!!-= In Trouble =-!!!!!\n");
			exit(1);
		}
	}

	while (j>0)
	{
		result [c++] = stack[j];
		j--;
	}
	result[c] = 0;

	return result;
}

float sumItUp (char *s)
{
	float stack[MAX];
	int j=0;
	float t1, t2;

	for (int i=0; s[i]; i++)
	{
		if ((s[i]>='A' && s[i]<='Z') || (s[i]>='z' && s[i]<='z'))
		{
			j++;
			stack[j] = value(s[i]);
		}
		else
		{
			t1 = stack[j-1];
			t2 = stack[j];
			stack[--j] = calculate (t1, t2, s[i]);
		}
	}

	return stack[j];
}

float value (char c)
{
	for (int i=0; values[i][0]>0; i++)
	{
		if ((int)values[i][0]==c) return values[i][1];
	}

	printf ("In trouble !!!\n");
	exit(1);
	return -1;
}

float calculate (float a, float b, char operand)
{
	switch (operand)
	{
		case '+': return a+b;
		case '-': return a-b;
		case '*': return a*b;
		case '/': return a/b;
		case '^': return pow(a,b);
	}
	return 0;
}

void fillTheValues (void)
{
	FILE *f = fopen ("postfix.txt","r");
	if (f==NULL)
	{
		printf ("Problem with the file !!!\n");
		exit(1);
	}

	int k=0;
	char musor[MAX];	// garbage
	char a;
	float b;

	fscanf (f," %s", musor);
	while (fscanf(f, " %c = %f", &a, &b)==2)
	{
		values[k][0] = (int)a;
		values[k++][1] = b;
		printf ("%f %f\n",a,b);
	}
	values[k][0] = -1;

	fclose (f);
}

int priority (char c)
{
	if (c=='(' || c==')') return 0;
	if (c=='-' || c=='+') return 1;
	if (c=='*' || c=='/') return 2;
	return 3;
}
