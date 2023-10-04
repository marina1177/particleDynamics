#include "../includes/verle.h"


int check_boarder_conditions(t_trap	**trap, int p_indx){

	if ((*trap)->particles[p_indx].out == 0 &&
			((*trap)->particles[p_indx].r[0] < -0.02 || (*trap)->particles[p_indx].r[0] > 0.02
	  			|| (*trap)->particles[p_indx].r[1] < -0.02 || (*trap)->particles[p_indx].r[1] > 0.02)){

				//!TODO надо прсто исключить ее из расчета
				printf("particle #%d is out!\n",(*trap)->particles[p_indx].p_indx);

				(*trap)->particles[p_indx].out = 1;
				(*trap)->length_of_prtcl_arr -= 1;

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
	printf("calculation time[s]:	%.10f, amount of steps:	%d, step: %.10f \n", t_fin, nh, dt);
	//printf("#############################\n\n");


	int t = 0;
	while( t <= nh){
		(*trap)->tcurr = t*dt;
		int partcl_num=0;
		while(partcl_num <(*trap)->amount_of_particles){
			//printf("[%d] into cycle!\n", partcl_num);
			if((*trap)->particles[partcl_num].out == 1){
				printf("[%d] is out!\n", partcl_num);
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

			//printf("sum_acc[%d]: %f %f %f\n\n", partcl_num,(*trap)->particles[partcl_num].a[0], (*trap)->particles[partcl_num].a[1], (*trap)->particles[partcl_num].a[2]);

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
		if((*trap)->length_of_prtcl_arr != (*trap)->amount_of_particles){
			exclude_out_particles(trap);
		}

		t++;


	}

}
