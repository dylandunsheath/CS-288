#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[], char *envp[])
{
	char delimiter[] = "=";
	int i;
	int j;
	char *temp;
	// sorting via bubble sort 
	for (i = 0; envp[i] != NULL; i++) 
	{
		// separate variable name and its value
		strtok(envp[i], delimiter); 
		for (j = i + 1; envp[j] != NULL; j++) 
		{
			// separate next variable name and its value (index after index i)
			strtok(envp[j], delimiter); 
			// if str1 is greater than str2
			// swap them
			if (strcmp(envp[i], envp[j]) > 0)
			{
				temp = envp[i];
				envp[i] = envp[j];
				envp[j] = temp;
			}
		}
	}

	printf("Sorted Environmental Variables. . .\n");
	for (i = 0; envp[i] != NULL; i++)
	{
		envp[i][strlen(envp[i])] = '='; 
		printf("%s\n\n", envp[i]);
	}
	return 0;
}

