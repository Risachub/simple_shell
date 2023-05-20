#include "shell.h"

char *_strncat(char *dest, const char *src, size_t n);
char *_strcpy(char *dest, const char *src);
char *_strcat(char *dest, const char *src);
int _strlen(const char *s);

/**
 * _strncat - this concantenates two strings where n number
 *            of bytes are copied from source.
 * @dest: This is a pointer to destination string.
 * @src: This is a pointer to source string.
 * @n: n bytes to copy from src.
 *
 * Return: This is the Pointer to destination string.
 */

char *_strncat(char *dest, const char *src, size_t n)
{
	size_t dest_len = _strlen(dest);
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[dest_len + i] = src[i];
	dest[dest_len + i] = '\0';

	return (dest);
}

/**
 * _strcpy - This copies the string pointed to by src, includes the
 *           terminating null byte, to the buffer pointed by des
 * @dest: A pointer to the destination of copied string.
 * @src: A pointer to the src of the source string.
 *
 * Return: Pointer to dest.
 */

char *_strcpy(char *dest, const char *src)
{
	size_t i;

	for (i = 0; src[i] != '\0'; i++)
		dest[i] = src[i];
	dest[i] = '\0';
	return (dest);
}

/**
 * _strlen - This returns the length of a string.
 * @s: Pointer to characters string.
 *
 * Return: length of the character string.
 */

int _strlen(const char *s)
{
	int len = 0;

	if (!s)
		return (len);
	for (len = 0; s[len]; len++)
		;
	return (len);
}


/**
 * _strcat - This concantenates two strings.
 * @dest: A pointer to destination string.
 * @src: A pointer to source string.
 *
 * Return: A pointer to destination string.
 */

char *_strcat(char *dest, const char *src)
{
	const char *srcTemp;
	char *destTemp;

	srcTemp =  src;
	destTemp = dest;

	while (*destTemp != '\0')
		destTemp++;

	while (*srcTemp != '\0')
		*destTemp++ = *srcTemp++;
	*destTemp = '\0';
	return (dest);
}
