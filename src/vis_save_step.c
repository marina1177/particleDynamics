#include "../includes/verle.h"

// void	put_const(t_vis *vs, t_prtcl *particle)
// {
// 	const char *s[2];

// 	s[0] = "\"Oo\"";
// 	s[1] = "\"Xx\"";
// 	vs_putstr_fd(vs->fd, "\n\t{\"Consts\": {\n\t\t\"map_rows\": ");
// 	vs_itoa_fd(vs->fd, map->rows);
// 	//vs_putstr_fd(vs->fd, A_MCOLS);
// 	vs_itoa_fd(vs->fd, map->cols);
// 	//vs_putstr_fd(vs->fd, A_MY_CH);
// 	vs_putstr_fd(vs->fd, s[map->num_player - 1]);
// 	//vs_putstr_fd(vs->fd, A_OPP_CH);
// 	vs_putstr_fd(vs->fd, s[map->num_opp - 1]);
// 	vs_putstr_fd(vs->fd, "\n\t},");
// }


void	put_file(int t, t_vis *vis, t_trap *trap, t_prtcl *particle)
{
	if (t == 0)
	{
		vs_putstr_fd(vis->fd, "[{");
		//put_const(vis, particle);
	}
	else
	{
		vs_putstr_fd(vis->fd, ",{");
	}
	vs_putstr_fd(vis->fd, "\n\t\"time_step\": ");
	vs_itoa_fd(vis->fd, t);

	vs_putstr_fd(vis->fd, ",\n\t\"x\": ");
	float_itoa_fd(vis->fd, particle->r[0]);
	vs_putstr_fd(vis->fd, ",\n\t\"y\": ");
	float_itoa_fd(vis->fd, particle->r[1]);

	vs_putstr_fd(vis->fd, ",\n\t\"Vx\": ");
	float_itoa_fd(vis->fd, particle->v[0]);
	vs_putstr_fd(vis->fd, ",\n\t\"Vy\": ");
	float_itoa_fd(vis->fd, particle->v[1]);

	vs_putstr_fd(vis->fd, ",\n\t\"ax\": ");
	float_itoa_fd(vis->fd, particle->a[0]);
	vs_putstr_fd(vis->fd, ",\n\t\"ay\": ");
	float_itoa_fd(vis->fd, particle->a[1]);

	vs_putstr_fd(vis->fd, "\n}");
}


int save_step(int t, t_trap *trap, t_prtcl *particle, t_vis *vis){

	if(t==0 &&
	(vis->fd = open("vis.json", O_CREAT | O_TRUNC | O_WRONLY, 0644)) == -1)
	{
		printf("%s\n", "vis.json file error\n");
		return (0);
	}
	if (vis->fd > 2)
	{
		put_file(t, vis, trap, particle);
		if (vis->end_game == 1)
		{
			printf("end game\n");
			write(vis->fd, "]", 1);
			close(vis->fd);
			return (1);
		}
	}

}
