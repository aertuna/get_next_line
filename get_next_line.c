/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aertuna <aertuna@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 17:39:46 by aertuna           #+#    #+#             */
/*   Updated: 2023/11/16 15:35:10 by aertuna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

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
		return (free(save), NULL);
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
	static char	*save;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	save = ft_read_and_save(fd, save);
	if (!save)
		return (NULL);
	line = ft_get_line(save);
	save = ft_save(save);
	return (line);
}

// Düz Yazdırma
// int	main(void)
// {
// 	char	*line;
// 	char	fd1;
// 	char	i;

// 	fd1 = open("test.txt", O_RDONLY);
// 	i = 0;
// 	while (i < 19)
// 	{
// 		line = get_next_line(fd1);
// 		printf ("%s ", line);
// 		free (line);
// 		i++;
// 	}
// 	close (fd1);
// 	return (0);
// }

// Düz Yazdırma .txt Kontrollü
// int	main(void)
// {
// 	char		*line;
// 	int			fd1;
// 	int			i;
// 	const char	*filename;
// 	const char	*extension;
// 	size_t		len;

// 	filename = "123.png";
// 	len = strlen(filename);
// 	if (len >= 4)
// 		extension = &filename[len - 4];
// 	else
// 		extension = NULL;
// 	if (extension == NULL || strcmp(extension, ".txt") != 0)
// 	{
// 		printf("Dosya \".txt\" uzantılı değil.\n");
// 		return (1);
// 	}
// 	fd1 = open("123.png", O_RDONLY);
// 	i = 0;
// 	while (i < 19)
// 	{
// 		line = get_next_line(fd1);
// 		printf("%s", line);
// 		free(line);
// 		i++;
// 	}
// 	close(fd1);
// 	return (0);
// }

// Düz Yazdırma V2
// int main(void)
// {
// 	char	**line;
// 	int		i;
// 	int 	j;
// 	int		fd;

// 	i = 0;
// 	fd = open ("test.txt", O_RDONLY);
// 	while (line)
// 	{
// 		line[i] = get_next_line(fd);
// 		if (line[i] == NULL)
// 		{
// 			i--;
// 			break ;
// 		}
// 		i++;
// 	}
// 	j = 0;
// 	while (j <= i)
// 		printf("%s", line[j++]);
// 	// system ("leaks x");
// 	return (0);
// }

// Tersten Yazdırma
// int main(void)
// {
// 	char	**line;
// 	int		i;
// 	int		fd;

// 	i = 0;
// 	fd = open ("test.txt", O_RDONLY);
// 	while (line)
// 	{
// 		line[i] = get_next_line(fd);
// 		if (line[i] == NULL)
// 		{
// 			i--;
// 			break ;
// 		}
// 		i++;
// 	}
// 	while (i >= 0)
// 		printf("%s", line[i--]);
// 	system ("leaks x");
// 	return (0);
// }
