#include "../includes/verle.h"

double randfrom(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

void	init_particle_velocity(FILE *fp, t_prtcl	*particle){
// velocity Vx Vy Vz [m]
	particle->v[0] =  randfrom(0,0.05);
	particle->v[1] = randfrom(0,0.05);
	particle->v[2] = 0;
}

void	init_particle_position(FILE *fp, t_prtcl	*particle){
// position x y z [m]
	particle->r[0] = randfrom(0,0.004);
	particle->r[1] = randfrom(0,0.004);;
	particle->r[2] = 0;
}

void	init_particle(FILE *fp, t_prtcl	*particle){

//TODO считывать начальные параметры из файла

// phisics properties
	particle->rho = 3.95e3;
	particle->d = 2*10e-5;
	particle->m = particle->rho*(4/3)*M_PI *pow(particle->d/2, 3);
	particle->q = 5e-16;

// acceleration
	particle->a[0] = 0;
	particle->a[1] = -G_CONST;
	particle->a[2] = 0;

// printf("init acceleration:	(%.2f, %.2f, %.2f)\n", particle->a[0], particle->a[1], particle->a[2]);

	// printf("\nparticle mass[kg]:	%.3e\n", particle->m);
	// printf("particle size[m]:	%.3e\n", particle->d);
	// printf("particle charge[e]:	%.3e\n", particle->q);
	// printf("\nq/m:	%.3e\n", particle->q/particle->m);
}

static void write_trap_parameters_to_file(FILE *fp, t_trap	**trap){

	//TODO считывать начальные параметры из файла
	(*trap)->a = 0.004;	// [m] диаметр электрода
	fwrite(&((*trap)->a), sizeof(double), 1, fp);
	printf("5. diameter of electrode, a[m]: %f\n", (*trap)->a);

	(*trap)->ra = 0.019;	// [m] расстояние между центрами электродов
	fwrite(&((*trap)->ra), sizeof(double), 1, fp);
	printf("6. distance between centers of electrodes, ra[m]: %f\n", (*trap)->ra);

	(*trap)->Ua = 5000;	// [V] амплитуда переменного напряжения на электродах
	fwrite(&((*trap)->Ua), sizeof(double), 1, fp);
	printf("7. AC amplitude, Ua[V]: %f\n", (*trap)->Ua);

	(*trap)->V = 0;		// [V] амплитуда постянного напряжения на электродах
	fwrite(&((*trap)->V), sizeof(double), 1, fp);
	printf("8. DC amplitude, V[V]: %f\n", (*trap)->V);

	(*trap)->freq = 50;
	fwrite(&((*trap)->freq), sizeof(double), 1, fp);
	printf("9. frequency, freq[Hz]: %f\n", (*trap)->freq);

	(*trap)->nu = 18.27*pow(10,-6);	// вязкость среды (воздуха)
	fwrite(&((*trap)->nu), sizeof(double), 1, fp);
	printf("10. Air viscosity, nu[Pa*s]: %f\n", (*trap)->freq);
}


int	init_trap(FILE *fp, t_trap	**trap){

	write_trap_parameters_to_file(fp, trap);

	(*trap)->tau = 0;
	// ? trap->a_param =;

	!((*trap)->particles = (t_prtcl *)malloc(sizeof(t_prtcl)*(*trap)->amount_of_particles)) ? handle_error(ERR_ALLOC) : 0;

	t_prtcl *particles = (*trap)->particles;

	int i = 0;
	double startbit = 1;
	while(i < (*trap)->amount_of_particles){

			(*trap)->particles[i].p_indx =i;

			init_particle(fp, &(*trap)->particles[i]);
			init_particle_position(fp, &(*trap)->particles[i]);
			init_particle_velocity(fp, &(*trap)->particles[i]);

			if(i == 0){
				//write startbit=1
				startbit = 1;
				fwrite(&startbit,sizeof(double), 1, fp);
			}else{
				startbit = 0;
				fwrite(&startbit,sizeof(double), 1, fp);
			}
			write_particle_state(fp, i, &(*trap)->particles[i]);
			write_particle_parameters(fp, i, &(*trap)->particles[i]);

			//printf("PARTICLE NUMBER	#%d is initiated !\n", (*trap)->particles[i].p_indx);
			if ((*trap)->particles[i].p_forces = (t_frcs*)malloc(sizeof(t_frcs))){
				calc_forces((*trap), i, 0);
			}
			else{
				printf("array of force malloc error for particle number #%d\n", (*trap)->particles[i].p_indx);
				handle_error(ERR_ALLOC);
			}

			i++;
		}
		//printf("finish trap\n");
		return(0);
}

int check_flags(FILE *fp, int ac, char **av, t_trap	**trap){

	char	**argv;
	int		argc;
	int		fcnt;

	if (ac < 5){
		printf("\n args format usage: -a amount_of_particle -t: start:step:end\n");
		return (-1);
	}

	argv = av;
	argc = 1;

	while (argv[argc] != NULL && argv[argc][0] == '-'){
		//printf("argv[%d] = %s\n", argc, argv[argc]);

		if (argv[argc][1] == 'a'){
			argc++;
			(*trap)->amount_of_particles = atoi(argv[argc]);
			printf("2. amount of particles: %f\n", (*trap)->amount_of_particles);
			fwrite(&((*trap)->amount_of_particles), sizeof(double), 1, fp);
		}
		else if(strcmp(argv[argc],"-t") == 0){
			argc++;

			char sep [10]=":";
			char **split = ft_strsplit(argv[argc], ':');

			if(sizeof(split)<3){
				printf("usage -t: start:step:end time\n");
			}

			(*trap)->tfull = atof(split[2]);
			//printf("tfull = %f\n", (*trap)->tfull);
			(*trap)->tstep = atof(split[1]);
			//printf("tstep = %f\n", (*trap)->tstep);
			(*trap)->tcurr = atof(split[0]);
			//printf("tcurr = %f\n", (*trap)->tcurr);

			fwrite(&((*trap)->tfull), sizeof(double), 1, fp);
			fwrite(&((*trap)->tstep), sizeof(double), 1, fp);

			printf("3. full time of calc: %f\n",(*trap)->tfull);
			printf("4. timestep: %f\n",(*trap)->tstep);

		}
		else{
			printf("flags: invalid options <<%s>>\n", argv[argc]);
			return(-1);
		}
		argc++;
	}
	return 0;
}

int	start_parameters_generator(FILE *fp, int ac, char **av, t_trap	**trap){

	// check that file is not empty
	double a[80];

	int c=fgetc(fp);
	if (c==EOF) {
		printf("file is empty\n");

		double sizeof_parameters =  sizeof(double)*AMOUNT_OF_PARAMS;
	//сколько байт на параметры (double sizeof = 8)
		printf("1.bytes of parameters: %lf\n", sizeof_parameters);
		fwrite(&sizeof_parameters, sizeof(double), 1, fp);

	// check flags and set parameters
		return check_flags(fp, ac, av, trap);
	}

	return 0;
}
