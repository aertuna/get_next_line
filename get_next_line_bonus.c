/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aertuna <aertuna@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 17:08:37 by aertuna           #+#    #+#             */
/*   Updated: 2023/11/15 18:11:11 by aertuna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <stdio.h>
#include <fcntl.h>

char	*ft_save(char *save)
{
	int		i;
	int		c;
	char	*s;

	i = 0;
	while (save[i] && save[i] != '\n')
		i++;
	if (!save[i])
		return (free(save), NULL);
	s = (char *)malloc(sizeof(char) * (ft_strlen(save) - i + 1));
	if (!s)
		return (free (save), NULL);
	i++;
	c = 0;
	while (save[i])
		s[c++] = save[i++];
	s[c] = '\0';
	return (free(save), s);
}

char	*ft_read_and_save(int fd, char *save)
{
	char	*buff;
	int		read_bytes;

	buff = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buff)
		return (free(save), NULL);
	read_bytes = 1;
	while (!ft_strchr(save, '\n') && read_bytes != 0)
	{
		read_bytes = read(fd, buff, BUFFER_SIZE);
		if (read_bytes == -1)
			return (free(buff), free(save), NULL);
		buff[read_bytes] = '\0';
		save = ft_strjoin(save, buff);
		if (!save)
			return (free(buff), free(save), NULL);
	}
	return (free(buff), save);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*save[257];

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > 256)
		return (NULL);
	save[fd] = ft_read_and_save(fd, save[fd]);
	if (!save[fd])
		return (NULL);
	line = ft_get_line(save[fd]);
	save[fd] = ft_save(save[fd]);
	return (line);
}

//Düz Yazdırma
// int	main(void)
// {
// 	char	*line;
// 	int		fd1;
// 	int		fd2;
// 	int		i;

// 	fd1 = open("test.txt", O_RDONLY);
// 	fd2 = open("test2.txt", O_RDONLY);
// 	i = 0;
// 	while (i < 19)
// 	{
// 		line = get_next_line(fd1);
// 		printf("%s", line);
// 		free(line);
// 		line = get_next_line(fd2);
// 		printf("%s", line);
// 		free(line);
// 		i++;
// 	}
// 	close(fd1);
// 	close(fd2);
// 	return (0);
// }
