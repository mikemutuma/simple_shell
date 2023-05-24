#include "shell.h"

/**
 * input_buf - this function buffers chained commands
 * @info: arguement structure
 * @buf: ads buffer
 * @len: ads length variable
 *
 * Return: bytes read
 */
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t l = 0;
	size_t len_p = 0;

	if (!*len)
	{
		/*bfree((void **)info->cmd_buf);*/
		free(*buf);
		*buf = NULL;

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
			/* if (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = l;
				info->cmd_buf = buf;
			}
		}
	}
	return (l);
}

/**
 * get_input - this function gets a line without a new line
 * @info: arguement structure
 *
 * Return: bytes read
 */
ssize_t get_input(info_t *info)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t a, b, len;
	ssize_t l = 0;
	char **buf_p = &(info->arg), *p;

	_putchar(BUF_FLUSH);
	l = input_buf(info, &buf, &len);
	if (l == -1) /* EOF */
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
 * @info: arguementstructure
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
 * _getline - this function gets the following line of input from STDIN
 * @info: arguement structure
 * @ptr: ads points to buffer, pre allocated or NULL
 * @length: the size of pre allocated ptr buffer if not NULL
 *
 * Return: s
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t x;
	ssize_t l = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	l = read_buf(info, buf, &len);
	if (l == -1 || (l == 0 && len == 0))
		return (-1);

	c = _strchr(buf + i, '\n');
	x = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, s, s ? s + x : x + 1);
	if (!new_p)
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buf + i, x - i);
	else
		_strncpy(new_p, buf + i, x - i + 1);

	s += x - i;
	i = x;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - this function blocks ctrl-C
 * @sig_num: the signal no
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
