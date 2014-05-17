/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbenjami <rbenjami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/16 20:02:10 by rbenjami          #+#    #+#             */
/*   Updated: 2014/05/17 14:29:00 by rbenjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/serveur.h"

char		*get_pwd(void)
{
	char		buf[1024];

	return (ft_strdup(getcwd(buf, 1024)));
}

void		ft_ls(int cs)
{
	DIR				*rep;
	struct dirent	*dir;
	char			*tmp;
	char			*res;

	res = NULL;
	rep = opendir(get_pwd());
	while ((dir = readdir(rep)))
	{
		if (!res && dir->d_name[0] != '.')
			res = ft_strdup(dir->d_name);
		else if (dir->d_name[0] != '.')
		{
			tmp = res;
			res = ft_strjoin(res, "\n");
			free(tmp);
			tmp = res;
			res = ft_strjoin(res, dir->d_name);
			free(tmp);
		}
	}
	closedir(rep);
	send_msg(cs, res, 0);
	if (res)
		free(res);
}

int			ft_cd(int cs, char *dir, char *home)
{
	struct stat		st;
	char			**tmp;
	char			*save;

	tmp = ft_strsplit(dir, ' ');
	if (ft_tabsize((void **)tmp) == 2)
	{
		lstat(tmp[1], &st);
		if (S_ISDIR(st.st_mode))
		{
			save = get_pwd();
			chdir(tmp[1]);
			if (ft_strncmp(home, get_pwd(), ft_strlen(home)))
			{
				chdir(save);
				return (send_error(cs, "error"));
			}
		}
		else
			return (send_error(cs, "not a directory"));
	}
	else if (ft_tabsize((void **)tmp) == 1)
		chdir(home);
	return (send_msg(cs, get_pwd(), 0));
}
