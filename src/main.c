
#include "../includes/verle.h"

int			handle_error(char *s)
{
	printf("%s\n", s);
	exit(1);
}

void	calc_forces(t_trap	*trap, int p_indx, double t){
	// mg (horizontal trap)
	trap->particles[p_indx].p_forces->F_mg[0] = 0;
	trap->particles[p_indx].p_forces->F_mg[1] = -G_CONST * trap->particles[p_indx].m;
	trap->particles[p_indx].p_forces->F_mg[2] = 0;
	//printf("calc F_mg:	(%.3e, %.3e, %.2e)\n", trap->particles[p_indx].p_forces->F_mg[0], trap->particles[p_indx].p_forces->F_mg[1], trap->particles[p_indx].p_forces->F_mg[2]);

	// Stokes
	int i = 0;
	while(i < 3){
		trap->particles[p_indx].p_forces->F_st[i] = -6 * M_PI * trap->particles[p_indx].r[i] * trap->nu * trap->particles[p_indx].v[i];
		i++;
	}
	//printf("calc F_St:	(%.3e, %.3e, %.3e)\n", trap->particles[p_indx].p_forces->F_st[0], trap->particles[p_indx].p_forces->F_st[1], trap->particles[p_indx].p_forces->F_st[2]);

	// interparticle Columb interaction
	double rdist[3];
	i = 0;
	while(i < trap->amount_of_particles){
		if(i == p_indx){
			i++;
			continue;
		}
		rdist[0] = fabs(trap->particles[p_indx].r[0] - trap->particles[i].r[0]);
		rdist[1] = fabs(trap->particles[p_indx].r[1] - trap->particles[i].r[1]);
		rdist[2] = fabs(trap->particles[p_indx].r[2] - trap->particles[i].r[2]);
		//printf("calc rdist:	(%.3e, %.3e, %.3e)\n", rdist[0], rdist[1], rdist[2]);

		if(rdist[0] != 0){
			trap->particles[p_indx].p_forces->F_q[0] += (K_CONST * trap->particles[p_indx].q * trap->particles[i].q) / pow(rdist[0],2);
		}
		if(rdist[1] != 0){
			trap->particles[p_indx].p_forces->F_q[1] += (K_CONST * trap->particles[p_indx].q * trap->particles[i].q) / pow(rdist[1],2);
		}
		if(rdist[2] != 0){
			trap->particles[p_indx].p_forces->F_q[2] += (K_CONST * trap->particles[p_indx].q * trap->particles[i].q) / pow(rdist[2],2);
		}
		i++;
	}
	//printf("calc F_q:	(%.3e, %.3e, %.3e)\n", trap->particles[p_indx].p_forces->F_q[0], trap->particles[p_indx].p_forces->F_q[1], trap->particles[p_indx].p_forces->F_q[2]);

	// Trap force
	double acc[3];
	acc[0] = -(2 * trap->particles[p_indx].q / (trap->particles[p_indx].m * pow(trap->ra / 2,2))) * (trap->V + trap->Ua * cos(2 * M_PI * trap->freq * t)) * trap->particles[p_indx].r[0];;
	acc[1] = (2 * trap->particles[p_indx].q / (trap->particles[p_indx].m * pow(trap->ra / 2,2))) * (trap->V + trap->Ua * cos(2 * M_PI * trap->freq * t)) * trap->particles[p_indx].r[1];
	acc[2] = 0;

	trap->particles[p_indx].p_forces->F_tr[0] = acc[0] * trap->particles[p_indx].m;
	trap->particles[p_indx].p_forces->F_tr[1] = acc[1] * trap->particles[p_indx].m;
	//printf("calc F_tr:	(%.3e, %.3e)\n", trap->particles[p_indx].p_forces->F_tr[0], trap->particles[p_indx].p_forces->F_tr[1]);

}


void	init_particle(t_prtcl	*particle){

//TODO считывать начальные параметры из файла

// position
	particle->r[0] = 0;
	particle->r[1] = 0;
	particle->r[2] = 0;
// velocity
	particle->v[0] = 0;
	particle->v[1] = 0;
	particle->v[2] = 0;
// acceleration
	particle->a[0] = 0;
	particle->a[1] = -G_CONST;
	particle->a[2] = 0;

	// printf("init coordinates:	(%.2f, %.2f, %.2f)\n", particle->r[0], particle->r[1], particle->r[2]);
	// printf("init velocity:		(%.2f, %.2f, %.2f)\n", particle->v[0], particle->v[1], particle->v[2]);
	// printf("init acceleration:	(%.2f, %.2f, %.2f)\n", particle->a[0], particle->a[1], particle->a[2]);

// phisics properties
	particle->rho = 3.95e3;
	particle->d = 2*10e-5;
	particle->m = particle->rho*(4/3)*M_PI *pow(particle->d/2, 3);
	particle->q = 5e-16;

	// printf("\nparticle mass[kg]:	%.3e\n", particle->m);
	// printf("particle size[m]:	%.3e\n", particle->d);
	// printf("particle charge[e]:	%.3e\n", particle->q);
	// printf("\nq/m:	%.3e\n", particle->q/particle->m);
}

int	init_trap(t_trap	**trap, int	amount_of_particles){

//TODO считывать начальные параметры из файла
	(*trap)->a = 0.004;	// [m] диаметр электрода
	(*trap)->ra = 0.019;	// [m] расстояние между центрами электродов
	(*trap)->Ua = 5000;	// [V] амплитуда переменного напряжения на электродах
	(*trap)->V = 0;		// [V] амплитуда постянного напряжения на электродах
	(*trap)->freq = 50;
	(*trap)->nu = 18.27*pow(10,-6);	// вязкость среды (воздуха)
	(*trap)->amount_of_particles = amount_of_particles;

	(*trap)->tau = 0;
	//trap->a_param =;

	t_prtcl *particles = (*trap)->particles;

	if((*trap)->particles = (t_prtcl *)malloc((amount_of_particles+1)*sizeof(t_prtcl))){
		int i = 0;
		while(i < amount_of_particles){
			(*trap)->particles[i].p_indx =i;
			init_particle(&(*trap)->particles[i]);
			printf("\nPARTICLE NUMBER	#%d\n", (*trap)->particles[i].p_indx);
			if ((*trap)->particles[i].p_forces = (t_frcs*)malloc(sizeof(t_frcs))){
				calc_forces((*trap), i, 0);
			}
			else{
				return(-1);
			}

			i++;
		}
		return(-1);
	}else{
		printf("array of particles malloc error\n");
		return(-1);
	}
}

// int malloc_particle_array(t_prtcl **particle){

// }


int		main(int ac, char **av)
{
	//t_prtcl	particle;
	t_trap	*trap;
	//t_frcs	forces;
	t_vis	vis;
	int		amount_of_particles = 3;

	printf("#####	PRECONDITIONS	#####\n\n");
	 !(trap = (t_trap *)malloc(sizeof(t_trap))) ? handle_error(ERR_ALLOC) : 0;

	//!>
	if(init_trap(&trap, amount_of_particles) < 0){
	//|| malloc_particle_array(&(trap->particles), amount_of_particles) < 0){

		int i = 0;
		while(i < trap->amount_of_particles){
			printf("FROM MAin(%d): trap index = %d\n",i,trap->particles->p_indx);
			printf("FROM MAin(%d): particle index = %d\n",i,trap->particles->p_indx);
			i++;
		}
		free_trap(&trap);
		return 0;
	}
	verlet(&trap);

	return 0;
}
