#include "shell.h"

/**
 * is_cmd - this function checks if a file is an executable command
 * @info: information structure
 * @path: path
 *
 * Return: 1 if successful, 0 owise
 */
int is_cmd(info_t *info, char *path)
{
	struct stat st;

	(void)info;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * dup_chars - copies chars
 * @pathstr: path str
 * @start: beginning position
 * @stop: ending position
 *
 * Return: it points to new buffer
 */
char *dup_chars(char *pathstr, int start, int stop)
{
	static char buf[1024];
	int j = 0, x = 0;

	for (x = 0, j = start; j < stop; j++)
		if (pathstr[j] != ':')
			buf[x++] = pathstr[j];
	buf[x] = 0;
	return (buf);
}

/**
 * find_path - this function checks path str
 * @info: information structure
 * @pathstr: path str
 * @cmd: command
 *
 * Return: path of command
 */
char *find_path(info_t *info, char *pathstr, char *cmd)
{
	int j = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
	{
		if (is_cmd(info, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstr[j] || pathstr[j] == ':')
		{
			path = dup_chars(pathstr, curr_pos, j);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (is_cmd(info, path))
				return (path);
			if (!pathstr[j])
				break;
			curr_pos = j;
		}
		j++;
	}
	return (NULL);
}
