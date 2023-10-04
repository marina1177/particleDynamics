#include "../includes/verle.h"

 void write_particle_state(FILE *fp, int i, t_prtcl	*particle){

	fwrite(&(particle->rho), sizeof(double), 1, fp);
	fwrite(&(particle->d), sizeof(double), 1, fp);
	fwrite(&(particle->q), sizeof(double), 1, fp);
}

 void write_particle_parameters(FILE *fp, int i, t_prtcl	*particle){

	fwrite(&(particle->r), sizeof(particle->r), 1, fp);
	fwrite(&(particle->v), sizeof(particle->v), 1, fp);
	fwrite(&(particle->a), sizeof(particle->a), 1, fp);

}

	int fp_save_step(t_trap **trap, FILE *fp){

		//printf("fp_save_step\n");
		double startbit;

		int i = 0;
		while (i < (*trap)->amount_of_particles)
		{
			if(i == 0){
				//write startbit=1
				startbit = 1;
				//printf("SAVE: tcurr = %lf\n", (*trap)->tcurr);
				fwrite(&startbit, sizeof(double), 1, fp);
				//fwrite(&(*trap)->tcurr,sizeof(double), 1, fp);

			}else{
				startbit = 0;
				fwrite(&startbit,sizeof(double), 1, fp);
			}
			//printf("write_particle_state, [%d]\n", i);
			write_particle_state(fp, i, &(*trap)->particles[i]);
			//printf("write_particle_parameters, [%d]\n", i);
			write_particle_parameters(fp, i, &(*trap)->particles[i]);
			i++;
		}

		//put_file(t, vis, trap, particle);
		// if (vis->end_game == 1)
		// {
		// 	printf("end game\n");
		// 	write(vis->fd, "]", 1);
		// 	close(vis->fd);
		// 	return (1);
		// }

	return (0);
 }
