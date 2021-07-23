#include <unistd.h>

void	ft_putcharl(char c)
{
	write(STDOUT_FILENO, &c, 1);
	write(STDOUT_FILENO, "\n", 1);
}
