#include "shell.h"

/**
 * _strlen - this function returns len of str
 * @s: str
 *
 * Return: int len of str
 */
int _strlen(char *s)
{
	int j = 0;

	if (!s)
		return (0);

	while (*s++)
		j++;
	return (j);
}

/**
 * _strcmp - this function does lex on strs.
 * @s1: beginning str
 * @s2: ending str
 *
 * Return: -ve when s1 < s2 & +ve when s1 > s2 & 0 when s1 == s2
 */
int _strcmp(char *s1, char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	if (*s1 == *s2)
		return (0);
	else
		return (*s1 < *s2 ? -1 : 1);
}

/**
 * starts_with - this function looks for needle beginning with hay stack
 * @haystack: str to look
 * @needle: substr to look
 *
 * Return: ads of following char of hay stack, empty
 */
char *starts_with(const char *haystack, const char *needle)
{
	while (*needle)
		if (*needle++ != *haystack++)
			return (NULL);
	return ((char *)haystack);
}

/**
 * _strcat - this function concatenates strs
 * @dest: location buffer
 * @src: start buffer
 *
 * Return: it points to loaction buffer
 */
char *_strcat(char *dest, char *src)
{
	char *ret = dest;

	while (*dest)
		dest++;
	while (*src)
		*dest++ = *src++;
	*dest = *src;
	return (ret);
}
