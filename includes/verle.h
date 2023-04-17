#ifndef VERLE_H
# define VERLE_H

# include <fcntl.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <limits.h>
# include <float.h>
# include <string.h>
# include <stdint.h>
# include <math.h>

# include <stdio.h>

# define G_CONST 			9.8

typedef struct s_particle	t_prtcl;
typedef struct s_trap		t_trap;
typedef struct s_vis		t_vis;

struct		s_particle
{
	double	r[2];	// [m] координаты
	double	v[2];	// [m/s]скорость
	double	a[2];	// [m/s^2] ускорение

	double	rho;	// [kg/m^3] плотность
	double	d;		// [m] диаметр, размер частицы
	double	m;		// [kg] масса частицы
	double	q;		// [e] заряд в единицах элементарного заряда e

	double	**x_arr;// массивы координат для построения траектории
	double	**y_arr;

	t_prtcl	*next;
};


struct		s_trap
{
	double		a;		// [m] диаметр электрода
	double		ra;		// [m] расстояние между центрами электродов
	double		Ua;		// [V] амплитуда переменного напряжения на электродах
	double		freq;	// [Hz] частота переменного напряжения на электродах

};

struct		s_vis
{
	int			end_game;
	int			fd;
};


/*
** vis_save_step.c
*/
void	put_file(int t, t_vis *vis, t_trap *trap, t_prtcl *particle);
int		save_step(int t, t_trap *trap, t_prtcl *particle, t_vis *vis);

/*
** vis_utilites.c
*/
int				count_dig(size_t val);
int				vs_putstr_fd(int fd, const char *s);
int				vs_strcpy(char *dest, const char *src);
int				vs_itoa(int n, char *s);

/*
** vis_itoa_fd.c
*/
int				vs_itoa_fd(int fd, int n);
char		*ft_strnew(size_t size);

/*
** float_itoa_fd.c
*/
int				float_itoa_fd(int fd, double d);
char		*ft_strnew(size_t size);

#endif
