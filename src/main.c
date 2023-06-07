
#include "../includes/verle.h"

int			handle_error(char *s)
{
	printf("%s\n", s);
	//free_particles(&(trap->particles));
	//free_trap(&trap);
	exit(1);
}

void	init_particle(t_prtcl	*particle){

//TODO считывать начальные параметры из файла

// position
	particle->r[0] = 0;
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
				printf("array of force malloc error for particle number #%d\n", (*trap)->particles[i].p_indx);
				handle_error(ERR_ALLOC);
			}

			i++;
		}
		return(0);
	}else{
		printf("array of particles malloc error\n");
		handle_error(ERR_ALLOC);
	}
}

int		main(int ac, char **av)
{
	t_prtcl	*particles;
	t_trap	*trap;
	t_vis	vis;
	int		amount_of_particles = 3;

	printf("#####	PRECONDITIONS	#####\n\n");
	!(trap = (t_trap *)malloc(sizeof(t_trap))) ? handle_error(ERR_ALLOC) : 0;
	//!(particles = (t_prtcl *)malloc(amount_of_particle*sizeof(t_prtcl))) ? handle_error(ERR_ALLOC) : 0;

	init_trap(&trap, amount_of_particles);

	int i = 0;
	while(i < trap->amount_of_particles){
		printf("FROM MAin(%d): particle index = %d\n",i,trap->particles[i].p_indx);
		i++;
	}
		
	verlet(&trap);

	return 0;
}
