#include "../../libft/libft.h"
#include <signal.h>  
#include <unistd.h>

typedef struct s_string
{
	char	*message;
	char	string[BUFFER_SIZE];
	int		message_received;
	int		full_buffer;
	int		bit_shift;
	int		bytes;
	int		si_pid;
}				t_string;

static t_string	g_string = {NULL, {0}, 0, 0, 7, 0, 0};

static void	bin_ones(int signo, siginfo_t *info, void *ucontext)
{
	(void)signo;
	(void)info;
	(void)ucontext;
	g_string.string[g_string.bytes] += 1 << g_string.bit_shift;
	g_string.bit_shift--;
	if (g_string.bit_shift == -1)
	{
		g_string.bit_shift = 7;
		g_string.bytes++;
		if (g_string.bytes == BUFFER_SIZE)
		{
			g_string.string[g_string.bytes] = '\0';
			g_string.full_buffer = 1;
			g_string.bytes = 0;
		}	
	}
}

static void	bin_zeros(int signo, siginfo_t *info, void *ucontext)
{
	(void)signo;
	(void)ucontext;
	g_string.bit_shift--;
	if (g_string.bit_shift == -1)
	{
		g_string.bit_shift = 7;
		if (g_string.string[g_string.bytes] == '\0')
		{
			g_string.message_received = 1;
			g_string.bytes = 0;
			g_string.si_pid = info->si_pid;
		}
		else
			g_string.bytes++;
		if (g_string.bytes == BUFFER_SIZE)
		{
			g_string.string[g_string.bytes] = '\0';
			g_string.full_buffer = 1;
			g_string.bytes = 0;
		}
	}	
}

static void	init_sigaction(struct sigaction *ones, struct sigaction *zeros)
{	
	sigset_t	ones_mask;
	sigset_t	zeros_mask;

	sigemptyset(&ones_mask);
	sigemptyset(&zeros_mask);
	sigaddset(&ones_mask, SIGUSR2);
	sigaddset(&zeros_mask, SIGUSR1);
	ones->sa_sigaction = bin_ones;
	zeros->sa_sigaction = bin_zeros;
	ones->sa_flags = SA_SIGINFO | SA_RESTART;
	zeros->sa_flags = SA_SIGINFO | SA_RESTART;
	ones->sa_mask = ones_mask;
	zeros->sa_mask = zeros_mask;
	if (sigaction(SIGUSR1, ones, NULL) < 0)
		ft_failed_exit("Signal handler error", g_string.message);
	if (sigaction(SIGUSR2, zeros, NULL) < 0)
		ft_failed_exit("Signal handler error", g_string.message);
}

static void	handle_message(void)
{
	char	*tmp;

	tmp = NULL;
	if (g_string.full_buffer)
		g_string.full_buffer = 0;
	if (g_string.message)
	{	
		tmp = ft_strdup(g_string.message);
		ft_free(g_string.message);
		g_string.message = ft_strjoin(tmp, g_string.string);
		if (tmp)
			ft_free(tmp);
	}
	else
		g_string.message = ft_strdup(g_string.string);
	if (!g_string.message)
		ft_failed_exit("memory error", NULL);
	if (g_string.message_received)
	{
		write(1, g_string.message, ft_strlen(g_string.message) + 1);
		write(1, "\n", 2);
		ft_free(g_string.message);
		g_string.message = NULL;
		g_string.message_received = 0;
	}
}

int	main(void)
{
	struct sigaction	ones;
	struct sigaction	zeros;
	int	pid;

	init_sigaction(&ones, &zeros);
	pid = getpid();
	write(1, "\033[36m", 5);
	ft_putstr("Server PID: ");
	ft_putnbrl(pid);
	write(1, "\033[0m", 4);
	while (1)
	{
		pause();
		if (g_string.message_received || g_string.full_buffer)
		{
			if (g_string.message_received)
			{
				write(1, "\033[35mMessage Received: \n", 24);
				write(1, "\033[0m", 4);
				kill (g_string.si_pid, SIGUSR1);
			}
			handle_message();
			ft_bzero(g_string.string, ft_strlen(g_string.string));
		}
	}
}
