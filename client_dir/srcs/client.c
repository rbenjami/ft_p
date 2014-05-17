/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbenjami <rbenjami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/12 18:51:57 by rbenjami          #+#    #+#             */
/*   Updated: 2014/05/17 15:42:05 by rbenjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/client.h"

void	usage(char *str)
{
	ft_printf("Usage: %s <address> <port>\n", str);
	exit(-1);
}

int		create_client(char *addr, int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (-1);
	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		ft_printf("Connect error\n");
		exit(-42);
	}
	return (sock);
}

void	prompt(char *addr, int port, char ok)
{
	ft_printf("%C%C%s:%d", YELLOW, B_BLACK, addr, port);
	if (ok == -1)
		ft_printf(" %C>%C%C>%C ", RED, RESET, BLACK, RESET);
	else
		ft_printf(" %C>%C%C>%C ", GREEN, RESET, BLACK, RESET);
}

void	rep(char buf[1024], int r)
{
	buf[r] = '\n';
	if (buf[0] != -1)
		ft_printf(buf);
	else
		ft_printf("%C%s%C\n", RED, &buf[1], RESET);
}

int		main(int argc, char *argv[])
{
	int					port;
	int					sock;
	int					r;
	char				*buf;

	if (argc != 3)
		usage(argv[0]);
	port = ft_atoi(argv[2]);
	sock = create_client(argv[1], port);
	prompt(argv[1], port, '0');
	buf = (char *)ft_memalloc(1024);
	while ((r = read(1, buf, 1023)) > 0)
	{
		buf[r - 1] = '\0';
		write(sock, buf, r);
		if (!ft_strcmp(buf, "quit"))
			break ;
		if ((r = recv(sock, buf, 1024, 0)) > 0)
			rep(buf, r);
		prompt(argv[1], port, buf[0]);
		ft_bzero(buf, 1024);
	}
	close(sock);
	return (0);
}
