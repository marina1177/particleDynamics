
#include "../includes/verle.h"

int			handle_error(char *s)
{
	printf("%s\n", s);
	//free_particles(&(trap->particles));
	//free_trap(&trap);
	exit(1);
}

void read_and_print(){
	FILE *fp2;
	double *buffer;

	if((fp2=fopen("calc", "rb+"))==NULL) {
    	printf("Ошибка при повторном открытии файла.\n");
    	exit(1);
  	}
	fseek(fp2, 0, SEEK_END);
	int filelen2 = ftell(fp2);
	printf("\n\n\nFile length is %d\n\n", filelen2);
	rewind(fp2);


	buffer = (double *)malloc((filelen2+1)*sizeof(double));
	fread(buffer, filelen2, 1, fp2);
	for(int i=0; i<50 ; i++)
	{
		printf("\n %d = %lf", i, buffer[i]);
	}

	fclose(fp2);
}

int		main(int ac, char **av)
{
	FILE *fp;
	t_trap	*trap;
	t_vis	vis;
	int		amount_of_particles = 3;

	if((fp=fopen("calc", "wb+"))==NULL) {
    	printf("Ошибка при открытии файла.\n");
    	exit(1);
  	}

	printf("#####	PRECONDITIONS	#####\n\n");
	!(trap = (t_trap *)malloc(sizeof(t_trap))) ? handle_error(ERR_ALLOC) : 0;
	if(start_parameters_generator(fp, ac, av, &trap) <0){
		printf("start parameter error!\n");
		exit(0);
	}

// !TODO запись начальных параметрв и положений частиц в файл
	init_trap(fp, &trap);

	printf("#####	VERLET	#####\n\n");
	verlet(&trap,fp);

	fclose(fp);
	//read_and_print();
	return 0;
}
