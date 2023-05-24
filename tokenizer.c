#include "shell.h"

/**
 * **strtow - this function breaks a str to words
 * @str: input str
 * @d: delimeter str
 * Return: it points to an array of strs
 */

char **strtow(char *str, char *d)
{
	int a, b, x, m, numwords = 0;
	char **s;

	if (str == NULL || str[0] == 0)
		return (NULL);
	if (!d)
		d = " ";
	for (a = 0; str[a] != '\0'; a++)
		if (!is_delim(str[a], d) && (is_delim(str[a + 1], d) || !str[a + 1]))
			numwords++;

	if (numwords == 0)
		return (NULL);
	s = malloc((1 + numwords) * sizeof(char *));
	if (!s)
		return (NULL);
	for (a = 0, b = 0; b < numwords; b++)
	{
		while (is_delim(str[a], d))
			a++;
		x = 0;
		while (!is_delim(str[a + x], d) && str[a + x])
			x++;
		s[b] = malloc((x + 1) * sizeof(char));
		if (!s[b])
		{
			for (x = 0; x < b; x++)
				free(s[x]);
			free(s);
			return (NULL);
		}
		for (m = 0; m < x; m++)
			s[b][m] = str[a++];
		s[b][m] = 0;
	}
	s[b] = NULL;
	return (s);
}

/**
 * **strtow2 - this function breaks a str into words
 * @str: input str
 * @d: delimeter
 * Return: it points to an array of strs
 */
char **strtow2(char *str, char d)
{
	int a, b, x, m, numwords = 0;
	char **s;

	if (str == NULL || str[0] == 0)
		return (NULL);
	for (a = 0; str[a] != '\0'; a++)
		if ((str[a] != d && str[a + 1] == d) ||
				    (str[a] != d && !str[a + 1]) || str[a + 1] == d)
			numwords++;
	if (numwords == 0)
		return (NULL);
	s = malloc((1 + numwords) * sizeof(char *));
	if (!s)
		return (NULL);
	for (a = 0, b = 0; b < numwords; b++)
	{
		while (str[a] == d && str[a] != d)
			a++;
		x = 0;
		while (str[a + x] != d && str[a + x] && str[a + x] != d)
			x++;
		s[b] = malloc((x + 1) * sizeof(char));
		if (!s[b])
		{
			for (x = 0; x < b; x++)
				free(s[x]);
			free(s);
			return (NULL);
		}
		for (m = 0; m < x; m++)
			s[b][m] = str[a++];
		s[b][m] = 0;
	}
	s[b] = NULL;
	return (s);
}
