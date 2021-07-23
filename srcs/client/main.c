#include "../../libft/libft.h"
#include <signal.h>
#include <unistd.h>

static char	*ft_chartobin(int x)
{
	int	bit_shift;
	int	bytes;
	char	*binary;

	bit_shift = 7;
	bytes = 0;
	binary = malloc(sizeof(*binary) * 9);
	if (!binary)
		return (NULL);
	while (bit_shift >= 0)
	{
		if (x & (1 << bit_shift))
			binary[bytes] = '1';
		else
			binary[bytes] = '0';
		bytes++;
		bit_shift--;
	}
	binary[bytes] = '\0';
	return (binary);
}

static void	send_signals(int pid, char *binary)
{
	int	ret;
	int	i;

	i = 0;
	while (binary[i])
	{
		if (binary[i] == '1')
			ret = kill(pid, SIGUSR1);
		else
			ret = kill(pid, SIGUSR2);
		if (ret == -1)
			ft_failed_exit("invalid PID", binary);
		usleep(20);
		i++;
	}
	ft_free(binary);
	binary = NULL;
}

static void	message_received(int sig)
{
	(void)sig;
	write(1, "\033[32m", 5);
	write(1, "Message successfully sent\n", 26);
	write(1, "\033[0m", 4);
	exit (EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	char	*binary;
	char	*message;
	size_t	character;
	int	pid;

	if (argc != 3)
		ft_failed_exit("usage: ./client <pid> <string>", NULL);
	signal(SIGUSR1, message_received);
	binary = NULL;
	message = argv[2];
	character = 0;
	pid = ft_atoi(argv[1]);
	if (pid < 0)
		ft_failed_exit("invalid PID", NULL);
	while (character <= ft_strlen(message))
	{
		binary = ft_chartobin(message[character]);
		if (!binary)
			ft_failed_exit("memory error", NULL);
		send_signals(pid, binary);
		binary = NULL;
		character++;
	}
}
