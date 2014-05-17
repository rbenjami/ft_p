/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveur.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbenjami <rbenjami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/12 16:48:18 by rbenjami          #+#    #+#             */
/*   Updated: 2014/05/17 15:42:15 by rbenjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

void		usage(char *str)
{
	ft_printf("Usage: %s <port>\n", str);
	exit(-1);
}

int			create_server(int port)
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
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		ft_printf("Bind error\n");
		exit(-42);
	}
	listen(sock, 42);
	return (sock);
}

void		soon(int cs)
{
	char				buf[1024];
	int					r;
	char				*home;

	home = get_pwd();
	while ((r = read(cs, buf, 1023)) > 0)
	{
		buf[r - 1] = '\0';
		ft_printf("received %d bytes: [%s]\n", r, buf);
		if (!ft_strcmp(buf, "ls"))
			ft_ls(cs);
		else if (!ft_strcmp(buf, "pwd"))
			send_msg(cs, get_pwd(), 0);
		else if (!ft_strncmp(buf, "cd ", 3) || !ft_strcmp(buf, "cd"))
			ft_cd(cs, buf, home);
		else if (!ft_strcmp(buf, "quit"))
			break ;
		else
			send_error(cs, "invalid_cmd");
		ft_bzero(buf, 1024);
	}
}

int			main(int argc, char *argv[])
{
	int					sock;
	int					cs;
	unsigned int		cslen;
	struct sockaddr_in	csin;
	pid_t				pid;

	signal(SIGCHLD, SIG_IGN);
	if (argc != 2)
		usage(argv[0]);
	sock = create_server(ft_atoi(argv[1]));
	while ((cs = accept(sock, (struct sockaddr *)&csin, &cslen)) > 0)
	{
		if ((pid = fork()) == -1)
			return (-1);
		if (pid == 0)
		{
			soon(cs);
			exit(0);
			return (0);
		}
	}
	close(cs);
	close(sock);
	return (0);
}
