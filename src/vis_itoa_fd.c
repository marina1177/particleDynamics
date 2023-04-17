#include "../includes/verle.h"

static int			count(int n)
{
	int	num;

	num = 0;
	if (n < 0)
	{
		num++;
		n = -n;
	}
	if (n == 0)
		num = 1;
	while (n != 0)
	{
		num++;
		n /= 10;
	}
	return (num);
}

static void			to_fd(int n, int num, short ui, int fd)
{
	int		i;
	char	*s;

	i = 0;
	s = ft_strnew((num) * sizeof(char));
	if (s != NULL)
	{
		while (i < num)
		{
			s[num - i - 1] = n % 10 + 48;
			n = n / 10;
			i++;
		}
		if (s[0] == '0' && s[1])
			s[0] = '-';
		if (ui)
			s[1] = '2';
		vs_putstr_fd(fd, s);
		free(s);
	}
}

char		*ft_strnew(size_t size)
{
	char	*s;
	size_t	i;

	if (size + 1 < size)
		return (NULL);
	s = (char *)malloc(sizeof(char) * (size + 1));
	if (!s)
		return (NULL);
	i = 0;
	while (i < size + 1)
	{
		s[i] = '\0';
		i++;
	}
	return (s);
}


int					vs_itoa_fd(int fd, int n)
{
	int				num;
	short			ui;

	ui = 0;
	num = count(n);
	if (n < 0)
	{
		n = -n;
	}
	if (n == -2147483648)
	{
		n = 147483648;
		ui = 1;
	}
	to_fd(n, num, ui, fd);
	return (num);
}
