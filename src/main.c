
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
	t_arrv3 crds;
	t_arrv3 vlcts;
	t_arrv3 props;



	return 0;
}
