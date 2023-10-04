#include "../includes/verle.h"


int malloc_2d_double_array(double **arr, int x, int y){

	if(arr = (double **)malloc(x*sizeof(double *))){
		for(int i = 0; i < x; i++) {
			arr[i] = (double *)malloc(y*sizeof(double));
		}
		return(1);
	}
	printf("malloc of 2d array error!\n");
	return(0);
}


int malloc_particle_array(double **arr, int x, int y){

	if(arr = (double **)malloc(x*sizeof(double *))){
		for(int i = 0; i < x; i++) {
			arr[i] = (double *)malloc(y*sizeof(double));
		}
		return(1);
	}
	printf("malloc of 2d array error!\n");
	return(0);
}
