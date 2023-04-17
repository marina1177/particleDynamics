#include "../includes/verle.h"

void	ft_strclr(char *s)
{
	while (s && *s)
		*(s++) = '\0';
}

int					float_itoa_fd(int fd, double d)
{
	 int			num;
	char			dest[50];

	num = sprintf(dest,"%.16f", d);

	vs_putstr_fd(fd, dest);
	return (num);
}
