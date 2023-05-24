#include "shell.h"

void free_list(list_t *head);
void free_alias_list(alias_t *head);
list_t *add_node_end(list_t **head, char *dir);
alias_t *add_alias_end(alias_t **head, char *name, char *value);


/**
 * free_list - This will free a list_t linked list.
 * @head: head of the list_t list.
 */

void free_list(list_t *head)
{
	list_t *nex;

	while (head)
	{
		nex = head->nex;
		free(head->dir);
		free(head);
		head = nex;
	}
}

/**
 * free_alias_list - This will free an alias_t linked list.
 * @head: head of the alias_t list.
 */

void free_alias_list(alias_t *head)
{
	alias_t *nex;

	while (head)
	{
		nex = head->nex;
		free(head->name);
		free(head->value);
		free(head);
		head = nex;
	}
}

/**
 * add_node_end - attaches a node to the end of a list_t linked list.
 * @head: pointer to the head of the list_t list.
 * @dir: directory path for the new node to contain.
 *
 * Return: If an error occurs - NULL.
 *         Else - a pointer to the new node.
 */

list_t *add_node_end(list_t **head, char *dir)
{
	list_t *new_node = malloc(sizeof(list_t));
	list_t *last;

	if (!new_node)
		return (NULL);

	new_node->dir = dir;
	new_node->next = NULL;

	if (*head)
	{
		last = *head;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
	else
		*head = new_node;

	return (new_node);
}

/**
 * add_alias_end - Attaches a node to the end of a alias_t linked list.
 * @head: pointer to the head of the list_t list.
 * @name: name of the new alias to be added.
 * @value: value of the new alias to be added.
 *
 * Return: If an error occurs - NULL.
 *        Else - a pointer to the new node.
 */

alias_t *add_alias_end(alias_t **head, char *name, char *value)
{
	alias_t *new_node = malloc(sizeof(alias_t));
	alias_t *last;

	if (!new_node)
		return (NULL);

	new_node->next = NULL;
	new_node->name = malloc(sizeof(char) * (_strlen(name) + 1));
	if (!new_node->name)
	{
		free(new_node);
		return (NULL);
	}
	new_node->value = value;
	_strcpy(new_node->name, name);

	if (*head)
	{
		last = *head;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
	else
		*head = new_node;

	return (new_node);
}
