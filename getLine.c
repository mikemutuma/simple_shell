#include "shell.h"

/**
 * input_buf - this function buffers cmds
 * @info: arguement structure
 * @buf: buffer
 * @len: length of variable
 *
 * Return: bytes read
 */
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t l = 0;
	size_t len_p = 0;

	if (!*len)
	{
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		l = getline(buf, &len_p, stdin);
#else
		l = _getline(info, buf, &len_p);
#endif
		if (l > 0)
		{
			if ((*buf)[l - 1] == '\n')
			{
				(*buf)[l - 1] = '\0';
				l--;
			}
			info->linecount_flag = 1;
			remove_comments(*buf);
			build_history_list(info, *buf, info->histcount++);
			{
				*len = l;
				info->cmd_buf = buf;
			}
		}
	}
	return (l);
}

/**
 * get_input - this function gets the existing line
 * @info: arguement structure
 *
 * Return: bytes read
 */
ssize_t get_input(info_t *info)
{
	static char *buf;
	static size_t a, b, len;
	ssize_t l = 0;
	char **buf_p = &(info->arg), *p;

	_putchar(BUF_FLUSH);
	l = input_buf(info, &buf, &len);
	if (l == -1)
		return (-1);
	if (len)
	{
		b = a;
		p = buf + a;

		check_chain(info, buf, &b, a, len);
		while (b < len)
		{
			if (is_chain(info, buf, &b))
				break;
			b++;
		}

		a = b + 1;
		if (a >= len)
		{
			a = len = 0;
			info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p;
		return (_strlen(p));
	}

	*buf_p = buf;
	return (l);
}

/**
 * read_buf - this function reads a buffer
 * @info: arguement structure
 * @buf: buffer
 * @i: size
 *
 * Return: l
 */
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t l = 0;

	if (*i)
		return (0);
	l = read(info->readfd, buf, READ_BUF_SIZE);
	if (l >= 0)
		*i = l;
	return (l);
}

/**
 * _getline - this function gets the following line
 * @info: arguement structure
 * @ptr: pointer
 * @length: length
 *
 * Return: s
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t j, len;
	size_t x;
	ssize_t l = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (j == len)
		j = len = 0;

	l = read_buf(info, buf, &len);
	if (l == -1 || (l == 0 && len == 0))
		return (-1);

	c = _strchr(buf + j, '\n');
	x = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, s, s ? s + x : x + 1);
	if (!new_p)
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buf + j, x - j);
	else
		_strncpy(new_p, buf + j, x - j + 1);

	s += x - j;
	j = x;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - this is the function of the block
 * @sig_num: signal no
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
