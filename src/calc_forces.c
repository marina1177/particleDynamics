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

