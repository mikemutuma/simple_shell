#include "shell.h"

/**
 * bfree - this function makes space for a pointer and NULLs the address
 * @ptr: ads of the pointer
 *
 * Return: 1 if true, orwise 0.
 */
int bfree(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
