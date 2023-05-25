#include "shell.h"

/**
 * is_cmd - this function checks if a file can be executed
 * @info: infomation structure
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
 * dup_chars - this function copies chars
 * @pathstr: path of the str
 * @start: beginning position
 * @stop: ending position
 *
 * Return: pointer
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
 * find_path - this function looks for the command in a str
 * @info: infomation structure
 * @pathstr: path of the string
 * @cmd: command
 *
 * Return: path
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
