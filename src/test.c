# include <stdlib.h>
# include <stdio.h>
# include <string.h>

void pmalloc(int **p){

if(*p = (int*)malloc(10*sizeof(int))){

	printf("add = %p\n", p);
	for(int i=0; i<10; i++){
		p[0][i] = i;
	}
}
}

int		main(int ac, char **av)
{

	int *p;

	printf("add0 = %p\n", &p);

	pmalloc(&p);


	printf("add2 = %p\n", &(p[0]));

	for(int i=0; i<10; i++){

		printf("p=%d\n", p[i]);
	}

// for(int i =0; i<ac; i++){

// 	puts(*av++);

// }

}
