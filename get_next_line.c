/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybenzidi <ybenzidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:20:41 by ybenzidi          #+#    #+#             */
/*   Updated: 2024/12/30 22:23:57 by ybenzidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	fonctionlessline(char **storage, int i)
{
	char	*temp;

	if ((*storage)[i])
	{
		temp = ft_strdup(*storage + i);
		free(*storage);
		*storage = temp;
	}
	else
	{
		free(*storage);
		*storage = NULL;
	}
}

char	*extract_line(char **storage)
{
	char	*line;
	int		i;

	if (!*storage || !**storage)
	{
		if (*storage)
		{
			free(*storage);
			*storage = NULL;
		}
		return (NULL);
	}
	i = 0;
	while ((*storage)[i] && (*storage)[i] != '\n')
		i++;
	if ((*storage)[i] == '\n')
		i++;
	line = (char *)malloc(i + 1);
	if (!line)
		return (NULL);
	ft_strncpy(line, *storage, i);
	fonctionlessline(storage, i);
	return (line);
}

char	*read_and_update_storage(int fd, char *buffer, char **storage)
{
	ssize_t	bytes_read;

	bytes_read = 1;
	while (!has_newline(*storage) && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			free(*storage);
			*storage = NULL;
			return (NULL);
		}
		if (bytes_read > 0)
		{
			buffer[bytes_read] = '\0';
			*storage = ft_strjoin(*storage, buffer);
			if (!(*storage))
				return (free(buffer), NULL);
		}
	}
	return (*storage);
}

char	*get_next_line(int fd)
{
	static char	*storage;
	char		*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > 1024 || BUFFER_SIZE > 2147483647)
		return (NULL);
	buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (!read_and_update_storage(fd, buffer, &storage))
		return (NULL);
	free(buffer);
	return (extract_line(&storage));
}