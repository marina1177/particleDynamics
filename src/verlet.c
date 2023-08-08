#include "../includes/verle.h"

void mg_calc(t_trap	*trap,  int p_indx){

	trap->particles[p_indx].p_forces->F_mg[0] = 0;
	trap->particles[p_indx].p_forces->F_mg[1] = -G_CONST * trap->particles[p_indx].m;
	trap->particles[p_indx].p_forces->F_mg[2] = 0;
	//printf("calc F_mg:	(%.3e, %.3e, %.2e)\n", trap->particles[p_indx].p_forces->F_mg[0], trap->particles[p_indx].p_forces->F_mg[1], trap->particles[p_indx].p_forces->F_mg[2]);
}

void f_Stokes_calc(t_trap	*trap,  int p_indx){
	int i = 0;
	while(i < 3){
		trap->particles[p_indx].p_forces->F_st[i] = -6 * M_PI * trap->particles[p_indx].r[i] * trap->nu * trap->particles[p_indx].v[i];
		i++;
	}
	//printf("calc F_St:	(%.3e, %.3e, %.3e)\n", trap->particles[p_indx].p_forces->F_st[0], trap->particles[p_indx].p_forces->F_st[1], trap->particles[p_indx].p_forces->F_st[2]);
}

void interparticle_Columb_calc(t_trap	*trap,  int p_indx, double t){
	double rdist[3];
	int i = 0;
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
}

void particle_acceleration_calc(t_trap	*trap,  int p_indx, double t){

//	double acc[3];
	trap->particles[p_indx].a[0] = -(2 * trap->particles[p_indx].q / (trap->particles[p_indx].m * pow(trap->ra / 2,2))) * (trap->V + trap->Ua * cos(2 * M_PI * trap->freq * t)) * trap->particles[p_indx].r[0];;
	trap->particles[p_indx].a[1]= (2 * trap->particles[p_indx].q / (trap->particles[p_indx].m * pow(trap->ra / 2,2))) * (trap->V + trap->Ua * cos(2 * M_PI * trap->freq * t)) * trap->particles[p_indx].r[1];
	trap->particles[p_indx].a[2]= 0;
}

void f_trap_calc(t_trap	*trap,  int p_indx){

// Trap force
	trap->particles[p_indx].p_forces->F_tr[0] = trap->particles[p_indx].a[0] * trap->particles[p_indx].m;
	trap->particles[p_indx].p_forces->F_tr[1] = trap->particles[p_indx].a[1] * trap->particles[p_indx].m;
	//printf("calc F_tr:	(%.3e, %.3e)\n", trap->particles[p_indx].p_forces->F_tr[0], trap->particles[p_indx].p_forces->F_tr[1]);

}

void	calc_forces(t_trap	*trap, int p_indx, double t){

	// mg (horizontal trap)
	mg_calc(trap,p_indx);

	// Stokes
	f_Stokes_calc(trap, p_indx);

	// interparticle Columb interaction
	interparticle_Columb_calc(trap, p_indx,t);

	particle_acceleration_calc(trap, p_indx, t);

	// Trap force
	f_trap_calc(trap, p_indx);

}



int verlet(t_trap **trap, FILE *fp){

	double dt= 1 / (*trap)->freq / 250;	// шаг времени
	double t_fin = 20 / (*trap)->freq;	//[s] время окончания расчетов

	int nh = t_fin / dt; //количество шагов
	printf("freq:	%f\n", (*trap)->freq);
	printf("calculation time[s]:	%f, amount of steps:	%d\n", t_fin, nh);
	//printf("#############################\n\n");


	int t = 0;
	while( t <= nh){
		(*trap)->tcurr = t*dt;
		int i=0;
		while(i <(*trap)->amount_of_particles){

			(*trap)->particles[i].r[0] += (*trap)->particles[i].v[0]*dt + 0.5*(*trap)->particles[i].a[0]*pow(dt,2);
			(*trap)->particles[i].r[1] += (*trap)->particles[i].v[1]*dt + 0.5*(*trap)->particles[i].a[1]*pow(dt,2);

			// %проверка граничных условий
      		if ((*trap)->particles[i].r[0] < -0.02 || (*trap)->particles[i].r[0] > 0.02
	  			|| (*trap)->particles[i].r[1] < -0.02 || (*trap)->particles[i].r[1] > 0.02){

		//запись завершается по вылету одной частицы из ловушки
		//!TODO надо прсто исключить ее из расчета
		//vis.end_game = 1;
				printf("save last step #%d\n",t);
		//save_step(t, &trap, &trap.particles[i], &vis);
				return 0;
		}

			// вычисление скорости со старым ускорением
			(*trap)->particles[i].v[0] += 0.5*(*trap)->particles[i].a[0]*dt;
			(*trap)->particles[i].v[1] += 0.5*(*trap)->particles[i].a[1]*dt;

			// вычисление сил & нового ускорения
			calc_forces(*trap, i, t*dt);

			// вычисление скорости с новым ускорением
			(*trap)->particles[i].v[0] += 0.5*(*trap)->particles[i].a[0]*dt;
			(*trap)->particles[i].v[1] += 0.5*(*trap)->particles[i].a[1]*dt;

			i++;
	}

		// сохранение шага
		if(t == nh){
			printf("save last step #%d\n",t);
			//vis.end_game = 1;
		}

		fp_save_step(trap, fp);
		t++;


	}

}
