#include "../includes/verle.h"

void print_diff_old_new_particles(t_prtcl **new_prtcl_array, t_prtcl **old_prtcl_array, int old_size, int new_size)
{

	int cnt = 0;
	for (int i = 0; i < old_size; i++)
	{

		printf("i=%d\n", i);
		printf("index: old[d]  new[%d]\n", (*new_prtcl_array)[cnt].p_indx);
		if (cnt < new_size && (*old_prtcl_array[i]).p_indx == (*new_prtcl_array)[cnt].p_indx)
		{
			printf("index: old[%d]  new[%d]\n", (*old_prtcl_array[i]).p_indx, (*new_prtcl_array)[cnt].p_indx);
			printf("r[0]: old[%f]  new[%f]\n", (*old_prtcl_array[i]).r[0], (*new_prtcl_array)[cnt].r[0]);
			printf("r[1]: old[%f]  new[%f]\n", (*old_prtcl_array[i]).r[1], (*new_prtcl_array)[cnt].r[1]);
			// printf("F_q[0]: old[%f]  new[%f]\n",(*old_prtcl_array[i]).p_forces->F_q[0],(*new_prtcl_array)[cnt].p_forces->F_q[0]);

			cnt++;
		}
	}
}

void print_diff_old_new_particle(t_prtcl *new_prtcl_array, t_prtcl *old_prtcl_array){

	printf("index: old[%d]  new[%d]\n", (old_prtcl_array)->p_indx, (new_prtcl_array)->p_indx);
	printf("r[0]: old[%f]  new[%f]\n", (old_prtcl_array)->r[0], (new_prtcl_array)->r[0]);
	printf("r[1]: old[%f]  new[%f]\n", (old_prtcl_array)->r[1], (new_prtcl_array)->r[1]);
	// printf("F_q[0]: old[%f]  new[%f]\n",(*old_prtcl_array[i]).p_forces->F_q[0],(*new_prtcl_array)[cnt].p_forces->F_q[0]);
}

void rewrite_particle(t_prtcl *new_prtcl_array, t_prtcl *old_prtcl_array)
{

	(new_prtcl_array)->p_indx = (old_prtcl_array)->p_indx;
	(new_prtcl_array)->out = (old_prtcl_array)->out;
	(new_prtcl_array)->rho = (old_prtcl_array)->rho;
	(new_prtcl_array)->d = (old_prtcl_array)->d;
	(new_prtcl_array)->m = (old_prtcl_array)->m;
	(new_prtcl_array)->q = (old_prtcl_array)->q;

	(new_prtcl_array)->r[0] = (old_prtcl_array)->r[0];
	(new_prtcl_array)->r[1] = (old_prtcl_array)->r[1];
	(new_prtcl_array)->r[2] = (old_prtcl_array)->r[2];

	(new_prtcl_array)->v[0] = (old_prtcl_array)->v[0];
	(new_prtcl_array)->v[1] = (old_prtcl_array)->v[1];
	(new_prtcl_array)->v[2] = (old_prtcl_array)->v[2];

	(new_prtcl_array)->a[0] = (old_prtcl_array)->a[0];
	(new_prtcl_array)->a[1] = (old_prtcl_array)->a[1];
	(new_prtcl_array)->a[2] = (old_prtcl_array)->a[2];

	(new_prtcl_array)->p_forces = (t_frcs *)malloc(sizeof(t_frcs));


	(new_prtcl_array)->p_forces->F_mg[0] = (old_prtcl_array)->p_forces->F_mg[0];
	(new_prtcl_array)->p_forces->F_mg[1] = (old_prtcl_array)->p_forces->F_mg[1];
	(new_prtcl_array)->p_forces->F_mg[2] = (old_prtcl_array)->p_forces->F_mg[2];

	(new_prtcl_array)->p_forces->F_q[0] = (old_prtcl_array)->p_forces->F_q[0];
	(new_prtcl_array)->p_forces->F_q[1] = (old_prtcl_array)->p_forces->F_q[1];
	(new_prtcl_array)->p_forces->F_q[2] = (old_prtcl_array)->p_forces->F_q[2];

	(new_prtcl_array)->p_forces->F_st[0] = (old_prtcl_array)->p_forces->F_st[0];
	(new_prtcl_array)->p_forces->F_st[1] = (old_prtcl_array)->p_forces->F_st[1];
	(new_prtcl_array)->p_forces->F_st[2] = (old_prtcl_array)->p_forces->F_st[2];

	(new_prtcl_array)->p_forces->F_tr[0] = (old_prtcl_array)->p_forces->F_tr[0];
	(new_prtcl_array)->p_forces->F_tr[1] = (old_prtcl_array)->p_forces->F_tr[1];
	(new_prtcl_array)->p_forces->F_tr[2] = (old_prtcl_array)->p_forces->F_tr[2];

}

int exclude_out_particles(t_trap **trap)
{

	printf("exclude_out_particles\nlen = %f, amount = %f\n", (*trap)->length_of_prtcl_arr, (*trap)->amount_of_particles);

	int n;

	t_prtcl *new_prtcl_array;

	!(new_prtcl_array = (t_prtcl *)malloc(sizeof(t_prtcl) * (*trap)->length_of_prtcl_arr)) ? handle_error(ERR_ALLOC) : 0;

	n = (*trap)->amount_of_particles;
	int cnt = 0;
	for (int i = 0; i < n; i++)
	{

		if ((*trap)->particles[i].out == 0)
		{
			printf("#%d is rewriting to #%d!\n", i, cnt);
			rewrite_particle(&(new_prtcl_array[cnt]), &((*trap)->particles[i]));
			print_diff_old_new_particle(&(new_prtcl_array[cnt]), &((*trap)->particles[i]));

			cnt++;
		}
		else
		{
			printf("#%d is excluded!\n", i);
		}
	}

	//print_diff_old_new_particles(&new_prtcl_array, &((*trap)->particles), (*trap)->amount_of_particles, (*trap)->length_of_prtcl_arr);

	free_particles(&(*trap)->particles, n);
	(*trap)->particles = new_prtcl_array;
	(*trap)->amount_of_particles = (*trap)->length_of_prtcl_arr;

	return 0;
}
