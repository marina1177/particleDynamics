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
# define EPS_CONST			8.854e-12 //электрическая постоянная
# define K_CONST			1/(4*3.14*EPS_CONST)

# define AMOUNT_OF_PARAMS  10

# define ERR_ALLOC "Can\'t allocate memory"


typedef struct s_params			t_params;
typedef struct s_array_of_v4	t_arrv4;
typedef struct s_vector4		t_v4;

typedef struct s_particle	t_prtcl;
typedef struct s_trap		t_trap;
typedef struct s_forces		t_frcs;
typedef struct s_vis		t_vis;


struct		s_params{

// Trap's parameters
	double		a;		// [m] диаметр электрода
	double		ra;		// [m] расстояние между центрами электродов
	double		Ua;		// [V] амплитуда переменного напряжения на электродах
	double		V;		// [V] амплитуда постянного напряжения на электродах
	double		freq;	// [Hz] частота переменного напряжения на электродах
	double		nu;		// вязкость среды (воздуха)

// Particle's parameters
	double	rho;	// [kg/m^3] плотность
	double	d;		// [m] диаметр, размер частицы
	double	m;		// [kg] масса частицы
	double	q;		// [e] заряд в единицах элементарного заряда e

	double	a_param;
	double	q_param;
	double	tau;
};

struct		s_array_of_v4{
int		max;
int		len;

t_v4	*p_arr;
};


struct		s_vector4{

double	x;
double	y;
double	z;
double	w;
};



struct		s_particle
{
	int		p_indx;
	double	r[3];	// [m] координаты

// TODO v & a сразу записывать и не хранить
	 double	v[3];	// [m/s]скорость
	 double	a[3];	// [m/s^2] ускорение

	double	rho;	// [kg/m^3] плотность
	double	d;		// [m] диаметр, размер частицы
	double	m;		// [kg] масса частицы
	double	q;		// [e] заряд в единицах элементарного заряда e

	t_frcs	*p_forces;
};

struct		s_forces


{
	double	F_tr[3];	// удерживающая сила ловушки
	double	F_st[3];	// сила Стокса
	double	F_mg[3];	// сила гравитации
	double	F_q[3];		// сила Кулона
};

struct		s_trap
{
	double		a;		// [m] диаметр электрода
	double		ra;		// [m] расстояние между центрами электродов
	double		Ua;		// [V] амплитуда переменного напряжения на электродах
	double		V;		// [V] амплитуда постянного напряжения на электродах
	double		freq;	// [Hz] частота переменного напряжения на электродах
	double		nu;		// вязкость среды (воздуха)

	double		amount_of_particles;
	double		tfull;
	double		tstep;
	double		tcurr;
	//t_params	params;
	t_prtcl		*particles;

	// параметры уравнения Матье
	double	a_param;
	double	q_param;
	double	tau;

	//t_vis		vis;
};

struct		s_vis
{
	int		end_game;
	int		fd;
};


/*
** vis_save_step.c
*/
void		put_file(int t, t_vis *vis, t_trap *trap, t_prtcl *particle);
int			save_step(int t, t_trap *trap, t_prtcl *particle, t_vis *vis);

/*
** vis_utilites.c
*/
int			count_dig(size_t val);
int			vs_putstr_fd(int fd, const char *s);
int			vs_strcpy(char *dest, const char *src);
int			vs_itoa(int n, char *s);

/*
** vis_itoa_fd.c
*/
int			vs_itoa_fd(int fd, int n);
char		*ft_strnew(size_t size);

/*
** ft_strsplit.c
*/
char		**ft_strsplit(char const *s, char c);

/*
** float_itoa_fd.c
*/
int			float_itoa_fd(int fd, double d);
	//t_frcs	forces;
/*
** acceleration.c
*/

/*
** malloc_tools.c
*/
int			malloc_2d_double_array(double **arr, int x, int y);

/*
** free_tools.c
*/
int			free_2d_double_array(double **arr);
void		ft_free2d(void **array, int len);
void		free_trap(t_trap **trap);

/*
** fp_save_step.c
*/
void		write_particle_state(FILE *fp, int i, t_prtcl	*particle);
void		write_particle_parameters(FILE *fp, int i, t_prtcl	*particle);
int			fp_save_step(t_trap **trap, FILE *fp);

/*
** verlet.c
*/
void		mg_calc(t_trap	*trap,  int p_indx);
void		f_Stokes_calc(t_trap	*trap,  int p_indx);
void		interparticle_Columb_calc(t_trap	*trap,  int p_indx, double t);
void		particle_acceleration_calc(t_trap	*trap,  int p_indx, double t);
void		f_trap_calc(t_trap	*trap,  int p_indx);
void		calc_forces(t_trap	*trap, int p_indx, double t);
int			verlet(t_trap **trap, FILE *fp);

/*
** init.c
*/
double 		randfrom(double min, double max);
void		init_particle_velocity(FILE *fp, t_prtcl	*particle);
void		init_particle_position(FILE *fp, t_prtcl	*particle);

void		init_particle(FILE *fp, t_prtcl	*particle);
int			init_trap(FILE *fp, t_trap	**trap);
int			check_flags(FILE *fp, int ac, char **av, t_trap	**trap);
int			start_parameters_generator(FILE *fp, int ac, char **av, t_trap **trap);


/*
** main.c
*/
int			handle_error(char *s);


#endif
