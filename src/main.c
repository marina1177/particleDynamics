
#include "../includes/verle.h"

void	init_particle(t_prtcl	*particle){

// position
	particle->r[0] = 0;
	particle->r[1] = 0;
	particle->v[0] = 0;
	particle->v[1] = 0;
	particle->a[0] = 0;
	particle->a[1] = -G_CONST;

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
}

void	init_trap(t_trap	*trap){

	trap->a = 0.004;
	trap->ra = 0.019;
	trap->Ua = 5000;
	trap->freq = 50;
}

int		main(int ac, char **av)
{
	t_prtcl	particle;
	t_trap	trap;
	t_vis	vis;

	printf("#####	PRECONDITIONS	#####\n\n");
	init_particle(&particle);
	init_trap(&trap);

	double dt= 1 / trap.freq / 250;	// шаг времени
	double t_fin = 20 / trap.freq;	//[s] время окончания расчетов

	int nh = t_fin / dt; //количество шагов
	printf("calculation time[s]:	%f, amount of steps:	%d\n", t_fin, nh);
	printf("#############################\n\n");

	int t = 0;
	while( t <= nh){
		particle.r[0] += particle.v[0]*dt + 0.5*particle.a[0]*pow(dt,2);
		particle.r[1] += particle.v[1]*dt + 0.5*particle.a[1]*pow(dt,2);

	// %проверка граничных условий - они должны быть периодическими
      if (particle.r[0] < -0.02 || particle.r[0] > 0.02
	  || particle.r[1] < -0.02 || particle.r[1] > 0.02){

		vis.end_game = 1;
		printf("save last step #%d\n",t);
		save_step(t, &trap, &particle, &vis);
		return 0;
		}

	// вычисление скорости со старым ускорением
		particle.v[0] += 0.5*particle.a[0]*dt;
		particle.v[1] += 0.5*particle.a[1]*dt;

	// вычисление нового ускорения
		// a = F(r)/m
		particle.a[0] += 0.01;
		particle.a[1] += 0.02;

	// вычисление скорости с новым ускорением
		particle.v[0] += 0.5*particle.a[0]*dt;
		particle.v[1] += 0.5*particle.a[1]*dt;


	// сохранение шага
		if(t == nh){
			printf("save last step #%d\n",t);
			vis.end_game = 1;
		}
		save_step(t, &trap, &particle, &vis);

		t++;

	}




    return 0;

	// int x = 10;
    // int *p;
    // p = &x;
	// printf("p = %d \n", *p);
	// printf("*p = %p \n", p);

	// if ((str = reading(ac, av[1])) == NULL)
	// {
	// 	free(str);
	// 	ft_putendl("error");
	// 	return (0);
	// }
}
