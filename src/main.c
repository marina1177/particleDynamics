
#include "../includes/verle.h"

void	init_particle(t_prtcl	*particle){

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

	printf("init coordinates:	(%.2f, %.2f)\n", particle->r[0], particle->r[1]);
	printf("init velocity:		(%.2f, %.2f)\n", particle->v[0], particle->v[1]);
	printf("init acceleration:	(%.2f, %.2f)\n", particle->a[0], particle->a[1]);

// phisics properties
	particle->rho = 3.95e3;
	particle->d = 2*10e-5;
	particle->m = particle->rho*(4/3)*M_PI *pow(particle->d/2, 3);
	particle->q = 5e-16;

	printf("\nparticle mass[kg]:	%.3e\n", particle->m);
	printf("particle size[m]:	%.3e\n", particle->d);
	printf("particle charge[e]:	%.3e\n", particle->q);
	printf("\nq/m:	%.3e\n", particle->q/particle->m);

	if (particle->p_forces = (t_frcs*)malloc(sizeof(t_frcs))){
		init_forces(&particle);
	}
}
void	init_forces(t_prtcl	*p){

}

void	init_trap(t_trap	*trap, int	amount_of_particles){

	trap->a = 0.004;
	trap->ra = 0.019;
	trap->Ua = 5000;
	trap->freq = 50;
	trap->nu = 18.27*pow(10,-6);
	trap->amount_of_particles = amount_of_particles;

	if(trap->particles = (t_prtcl *)malloc(amount_of_particles*sizeof(t_prtcl))){
		int i =0;
		while(i < amount_of_particles){
			init_particle(&trap->particles[i]);
		}
	}else{
		printf("array of particles malloc error\n");
	}
}



int		main(int ac, char **av)
{
	//t_prtcl	particle;
	t_trap	trap;
	//t_frcs	forces;
	t_vis	vis;
	int		amount_of_particles = 2;

	printf("#####	PRECONDITIONS	#####\n\n");
	init_trap(&trap, amount_of_particles);


	double dt= 1 / trap.freq / 250;	// шаг времени
	double t_fin = 20 / trap.freq;	//[s] время окончания расчетов

	int nh = t_fin / dt; //количество шагов
	printf("calculation time[s]:	%f, amount of steps:	%d\n", t_fin, nh);
	printf("#############################\n\n");

	int t = 0;
	while( t <= nh){

		int i=0;
		while(i < trap.amount_of_particles){

		trap.particles[i].r[0] += trap.particles[i].v[0]*dt + 0.5*trap.particles[i].a[0]*pow(dt,2);
		trap.particles[i].r[1] += trap.particles[i].v[1]*dt + 0.5*trap.particles[i].a[1]*pow(dt,2);

	// %проверка граничных условий - они должны быть периодическими
      if (trap.particles[i].r[0] < -0.02 || trap.particles[i].r[0] > 0.02
	  || trap.particles[i].r[1] < -0.02 || trap.particles[i].r[1] > 0.02){

		//запись завершается по вылету одной частицы из ловушки
		//!! надо прсто исключить ее из расчета
		vis.end_game = 1;
		printf("save last step #%d\n",t);
		save_step(t, &trap, &trap.particles[i], &vis);
		return 0;
		}

	// вычисление скорости со старым ускорением
		trap.particles[i].v[0] += 0.5*trap.particles[i].a[0]*dt;
		trap.particles[i].v[1] += 0.5*trap.particles[i].a[1]*dt;

	// вычисление нового ускорения
		// a = F(r)/m
		//particle.a = calc_acceleration(&forces);
		trap.particles[i].a[0] += 0.01;
		trap.particles[i].a[1] += 0.02;

	// вычисление скорости с новым ускорением
		trap.particles[i].v[0] += 0.5*trap.particles[i].a[0]*dt;
		trap.particles[i].v[1] += 0.5*trap.particles[i].a[1]*dt;

		i++;
	}

	// сохранение шага
		if(t == nh){
			printf("save last step #%d\n",t);
			vis.end_game = 1;
		}
		//save_step(t, &trap, &vis);
		t++;


	}

	return 0;
}
