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
	FILE *fp;
  double d = 12.23;
  int i = 101;
  long l = 123023L;

    char *satoi = "d 567  -f";

  printf("\"%s\" after atoi: %ld\n", satoi, atol(satoi));

  // if((fp=fopen("test", "wb+"))==NULL) {
  //   printf("Ошибка при открытии файла.\n");
  //   exit(1);
  // }

  // fwrite(&d, sizeof(double), 1, fp);
  // fwrite(&i, sizeof(int), 1, fp);
  // fwrite(&l, sizeof(long), 1, fp);

  // rewind(fp);

  // fread(&d, sizeof(double), 1, fp);
  // fread(&i, sizeof(int), 1, fp);
  // fread(&l, sizeof(long), 1, fp);

  // printf("%f %d %ld\n", d, i, l);

  // fclose(fp);

  return 0;

	// int *p;

	// printf("add0 = %p\n", &p);

	// pmalloc(&p);


	// printf("add2 = %p\n", &(p[0]));

	// for(int i=0; i<10; i++){

	// 	printf("p=%d\n", p[i]);
	// }

// for(int i =0; i<ac; i++){

// 	puts(*av++);

// }

}
