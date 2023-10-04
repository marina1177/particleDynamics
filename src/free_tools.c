#include "../includes/verle.h"

int		free_2d_double_array(double **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr[i]);
	free(arr);
	return (1);
}

void	ft_free2d(void **array, int len)
{
	int i;

	i = -1;
	while (++i < len)
		free(array[i]);
	free(array);
}

void	free_particles(t_prtcl **prtcls, int size){
	printf("\nneed free %d particles\n", size);
	int i;
	t_prtcl *p;

	i = 0;
	while(i < size){
		printf("before free %d force\n", i);
		free((*prtcls)[i].p_forces);
		//printf("before free %d prtcl\n", i);
		free(prtcls[i]);
		i++;
	}
}

void	free_trap(t_trap **trap){

	t_prtcl **prtcl;
	prtcl = &((*trap)->particles);

	int i = 0;
	while(i <(*trap)->amount_of_particles){
		printf("FROM FREE(%d): particle index = %d\n",i,prtcl[i]->p_indx);
		free(prtcl[i]->p_forces);
		free(prtcl[i]);
		i++;
	}
	//free(trap);
}
