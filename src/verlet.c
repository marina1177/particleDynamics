#include "../includes/verle.h"

void mg_calc(t_trap	*trap,  int p_indx){

	trap->particles[p_indx].p_forces->F_mg[0] = 0;

	trap->particles[p_indx].p_forces->F_mg[1] = -G_CONST * trap->particles[p_indx].m;
	(trap)->particles[p_indx].a[1] += trap->particles[p_indx].p_forces->F_mg[1] / trap->particles[p_indx].m;

	trap->particles[p_indx].p_forces->F_mg[2] = 0;
	//printf("calc F_mg:	(%.3e, %.3e, %.2e)\n", trap->particles[p_indx].p_forces->F_mg[0], trap->particles[p_indx].p_forces->F_mg[1], trap->particles[p_indx].p_forces->F_mg[2]);
}

void Stokes_calc(t_trap	*trap,  int p_indx){

	// printf("Stokes_r[%d]: %.10f\n",1, trap->particles[p_indx].r[1]);
	// printf("Stokes_v[%d]: %.10Lf\n",1, trap->particles[p_indx].v[1]);

	int i = 0;
	while(i < 3){


		trap->particles[p_indx].p_forces->F_st[i] = -6 * M_PI * trap->particles[p_indx].r[i] * trap->nu * trap->particles[p_indx].v[i];

		(trap)->particles[p_indx].a[i] += trap->particles[p_indx].p_forces->F_st[i] / trap->particles[p_indx].m;

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
			trap->particles[p_indx].a[0] += trap->particles[p_indx].p_forces->F_q[0] / trap->particles[p_indx].m;
		}
		if(rdist[1] != 0){
			trap->particles[p_indx].p_forces->F_q[1] += (K_CONST * trap->particles[p_indx].q * trap->particles[i].q) / pow(rdist[1],2);
			trap->particles[p_indx].a[1] += trap->particles[p_indx].p_forces->F_q[1] / trap->particles[p_indx].m;
		}
		if(rdist[2] != 0){
			trap->particles[p_indx].p_forces->F_q[2] += (K_CONST * trap->particles[p_indx].q * trap->particles[i].q) / pow(rdist[2],2);
			trap->particles[p_indx].a[2] += trap->particles[p_indx].p_forces->F_q[2] / trap->particles[p_indx].m;
		}
		i++;
	}
	//printf("calc F_q:	(%.3e, %.3e, %.3e)\n", trap->particles[p_indx].p_forces->F_q[0], trap->particles[p_indx].p_forces->F_q[1], trap->particles[p_indx].p_forces->F_q[2]);
}

void acc_trap_calc(t_trap	*trap,  int p_indx, double t){

//	double acc[3];
	trap->particles[p_indx].a[0] += -(2 * trap->particles[p_indx].q / (trap->particles[p_indx].m * pow(trap->ra / 2, 2))) * (trap->V + trap->Ua * cos(2 * M_PI * trap->freq * t)) * trap->particles[p_indx].r[0];
	trap->particles[p_indx].a[1] += (2 * trap->particles[p_indx].q / (trap->particles[p_indx].m * pow(trap->ra / 2, 2))) * (trap->V + trap->Ua * cos(2 * M_PI * trap->freq * t)) * trap->particles[p_indx].r[1];
	trap->particles[p_indx].a[2] += 0;

	//printf("calc F_tr:	(%.3e, %.3e)\n", trap->particles[p_indx].p_forces->F_tr[0], trap->particles[p_indx].p_forces->F_tr[1]);
}


void	calc_forces(t_trap	*trap, int p_indx, double t){

	//! Сделать константой mg (horizontal trap)
	mg_calc(trap,p_indx);
	//printf("mg_acc: %Lf %Lf %Lf\n", (trap)->particles[p_indx].a[0], (trap)->particles[p_indx].a[1], (trap)->particles[p_indx].a[2]);

	// Stokes
	Stokes_calc(trap, p_indx);
	//printf("Stokes_acc: %Lf %Lf %Lf\n", (trap)->particles[p_indx].a[0], (trap)->particles[p_indx].a[1], (trap)->particles[p_indx].a[2]);


	// interparticle Columb interaction
	interparticle_Columb_calc(trap, p_indx,t);
	//printf("Columb_acc: %Lf %Lf %Lf\n", (trap)->particles[p_indx].a[0], (trap)->particles[p_indx].a[1], (trap)->particles[p_indx].a[2]);


	// Trap force
	acc_trap_calc(trap, p_indx, t);
	//printf("trap_acc: %Lf %Lf %Lf\n", (trap)->particles[p_indx].a[0], (trap)->particles[p_indx].a[1], (trap)->particles[p_indx].a[2]);

}

int check_boarder_conditions(t_trap	**trap, int p_indx){

	if ((*trap)->particles[p_indx].out == 0 &&
			((*trap)->particles[p_indx].r[0] < -0.02 || (*trap)->particles[p_indx].r[0] > 0.02
	  			|| (*trap)->particles[p_indx].r[1] < -0.02 || (*trap)->particles[p_indx].r[1] > 0.02)){

				//!TODO надо прсто исключить ее из расчета
				printf("particle #%d is out!\n",(*trap)->particles[p_indx].p_indx);
				(*trap)->particles[p_indx].out = 1;

				return 1;
			}
	return 0;
}

void clean_acc(t_trap	*trap, int p_indx){

	trap->particles[p_indx].a[0] = 0;
	trap->particles[p_indx].a[1] = 0;
	trap->particles[p_indx].a[2] = 0;

}

void	calc_crdnts(t_trap	**trap, int partcl_num, double dt){

	(*trap)->particles[partcl_num].r[0] += (*trap)->particles[partcl_num].v[0]*dt + 0.5*(*trap)->particles[partcl_num].a[0]*pow(dt,2);
	(*trap)->particles[partcl_num].r[1] += (*trap)->particles[partcl_num].v[1]*dt + 0.5*(*trap)->particles[partcl_num].a[1]*pow(dt,2);

}

void	calc_half_vlcts(t_trap	**trap, int partcl_num, double dt){

	(*trap)->particles[partcl_num].v[0] += 0.5*(*trap)->particles[partcl_num].a[0]*dt;
	(*trap)->particles[partcl_num].v[1] += 0.5*(*trap)->particles[partcl_num].a[1]*dt;

}

int verlet(t_trap **trap, FILE *fp){

	double dt;
	double t_fin;
	int nh;

	if((*trap)->tstep <= 0 || (*trap)->tfull <= 0){
		dt= 1 / (*trap)->freq / 250;	// шаг времени
		t_fin = 20 / (*trap)->freq;	//[s] время окончания расчетов
	}else{
		dt = (*trap)->tstep;
		t_fin = (*trap)->tfull;
	}
	nh = t_fin / dt; //количество шагов
	printf("freq:	%f\n", (*trap)->freq);
	printf("calculation time[s]:	%.10f, amount of steps:	%d, step: %f \n", t_fin, nh, dt);
	//printf("#############################\n\n");


	int t = 0;
	while( t <= nh){
		(*trap)->tcurr = t*dt;
		int partcl_num=0;
		while(partcl_num <(*trap)->amount_of_particles){
			//printf("[%d] into cycle!\n", partcl_num);
			if((*trap)->particles[partcl_num].out == 1){
				//printf("[%d] is out!\n", partcl_num);
				partcl_num++;
				//save_step(t, &trap, &trap.particles[i], &vis);
				continue;
			}
			// r_i+1
			calc_crdnts(trap, partcl_num, dt);

			// %проверка граничных условий
			if( check_boarder_conditions(trap, partcl_num)){
				partcl_num++;
				//save_step(t, &trap, &trap.particles[i], &vis);
				continue;
			}

			//V_i+1 = V_i + 0.5(a_i + a_i+1)*dt = V_i + 0.5*a_i*dt + 0.5*a_i+1*dt
			// вычисление скорости со старым ускорением
			calc_half_vlcts(trap, partcl_num, dt);

			clean_acc(*trap, partcl_num);
			// вычисление сил & нового ускорения
			calc_forces(*trap, partcl_num, t*dt);

			printf("sum_acc[%d]: %Lf %Lf %Lf\n\n", partcl_num,(*trap)->particles[partcl_num].a[0], (*trap)->particles[partcl_num].a[1], (*trap)->particles[partcl_num].a[2]);

			// вычисление скорости с новым ускорением
			calc_half_vlcts(trap, partcl_num, dt);

			partcl_num++;
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
