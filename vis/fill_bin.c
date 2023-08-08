
# include <stdio.h>
# include <unistd.h>

int		main(int ac, char **av)
{
	FILE *fp;

	if((fp=fopen("data", "wb+"))==NULL) {
    	printf("Ошибка при открытии файла.\n");
    	return 0;
  	}
	double sizeof_parameters =  sizeof(double)*8;
	//сколько байт на параметры (double sizeof = 8)
	fwrite(&sizeof_parameters, sizeof(double), 1, fp);

    double r[3] = {0.01, 0.02, 0.006};
    fwrite(&r, sizeof(r), 1, fp);
	fclose(fp);
}
