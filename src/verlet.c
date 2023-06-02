#include "../includes/verle.h"

int verlet(t_trap **trap){

	double dt= 1 / (*trap)->freq / 250;	// шаг времени
	double t_fin = 20 / (*trap)->freq;	//[s] время окончания расчетов

	int nh = t_fin / dt; //количество шагов
	printf("calculation time[s]:	%f, amount of steps:	%d\n", t_fin, nh);
	printf("#############################\n\n");

	int t = 0;
	while( t <= nh){

		int i=0;
		while(i <(*trap)->amount_of_particles){

		(*trap)->particles[i].r[0] += (*trap)->particles[i].v[0]*dt + 0.5*(*trap)->particles[i].a[0]*pow(dt,2);
		(*trap)->particles[i].r[1] += (*trap)->particles[i].v[1]*dt + 0.5*(*trap)->particles[i].a[1]*pow(dt,2);

	// %проверка граничных условий - они должны быть периодическими
      if ((*trap)->particles[i].r[0] < -0.02 || (*trap)->particles[i].r[0] > 0.02
	  || (*trap)->particles[i].r[1] < -0.02 || (*trap)->particles[i].r[1] > 0.02){

		//запись завершается по вылету одной частицы из ловушки
		//TODO надо прсто исключить ее из расчета
		//vis.end_game = 1;
		printf("save last step #%d\n",t);
		//save_step(t, &trap, &trap.particles[i], &vis);
		return 0;
		}

	// вычисление скорости со старым ускорением
		(*trap)->particles[i].v[0] += 0.5*(*trap)->particles[i].a[0]*dt;
		(*trap)->particles[i].v[1] += 0.5*(*trap)->particles[i].a[1]*dt;

	// вычисление нового ускорения
		// a = F(r)/m
		//particle.a = calc_acceleration(&forces);
		(*trap)->particles[i].a[0] += 0.01;
		(*trap)->particles[i].a[1] += 0.02;

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
		//save_step(t, &trap, &vis);
		t++;


	}

}
