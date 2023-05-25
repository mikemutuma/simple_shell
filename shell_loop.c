#include "shell.h"

/**
 * hsh - the function is for the main shell loop
 * @info: arguements and  return infomation structure
 * @av: parameter vector from main()
 *
 * Return: 0 on true, 1 on false
 */
int hsh(info_t *info, char **av)
{
	ssize_t l = 0;
	int builtin_ret = 0;

	while (l != -1 && builtin_ret != -2)
	{
		clear_info(info);
		if (interactive(info))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		l = get_input(info);
		if (l != -1)
		{
			set_info(info, av);
			builtin_ret = find_builtin(info);
			if (builtin_ret == -1)
				find_cmd(info);
		}
		else if (interactive(info))
			_putchar('\n');
		free_info(info, 0);
	}
	write_history(info);
	free_info(info, 1);
	if (!interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - this function checks builtin cmd
 * @info: aruement and return infomation structure
 *
 * Return: -1 if builtin is empty,
 * 0 if builtin is true,
 * 1 if builtin is false,
 * 2 if builtin signals exit()
 */
int find_builtin(info_t *info)
{
	int j, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (j = 0; builtintbl[j].type; j++)
		if (_strcmp(info->argv[0], builtintbl[j].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[j].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - this function checks cmd
 * @info: argurment and return information structure
 *
 * Return: void
 */
void find_cmd(info_t *info)
{
	char *path = NULL;
	int j, x;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (j = 0, x = 0; info->arg[j]; j++)
		if (!is_delim(info->arg[j], " \t\n"))
			x++;
	if (!x)
		return;

	path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		fork_cmd(info);
	}
	else
	{
		if ((interactive(info) || _getenv(info, "PATH=")
					|| info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
			fork_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			print_error(info, "not found\n");
		}
	}
}

/**
 * fork_cmd - this function forks a an executable thread
 * @info: arguement and return information structure
 *
 * Return: void
 */
void fork_cmd(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_environ(info)) == -1)
		{
			free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				print_error(info, "Permission denied\n");
		}
	}
}
