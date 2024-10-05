#include <stdio.h>
#include <stdlib.h>

int main(){

	FILE *f;
	int dim;
        long int element;
	char filename[256];

	printf("Provide a filename.\n");
	scanf("%s", filename); //no ampersand sign before filename bc its array

	f = fopen(filename, "wb");

	if(f == NULL){
		printf("FILE DIDNT OPEN");
	}

	printf("Provide a size 'n' for the 3D array.\n");
	scanf("%d", &dim);

	printf("dim = %d\n", dim);
	int i;
	fwrite(&dim, sizeof(dim), 1, f); //what we're writing, the size of items, amt of items, the file. use man fwrite
	srandom(3);
	for(i =0; i < dim * dim * dim; i++){ // loop for 3D array
		element = random();
		//element = i;
		printf("element %d: %ld \n", i, element);
		fwrite(&element, sizeof(element), 1, f);
	}

	fclose(f);

	return 0;
}

