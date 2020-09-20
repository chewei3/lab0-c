#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    /* TODO: What if malloc returned NULL? */
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (!q)
        return;
    list_ele_t *head = q->head;
    while (head) {
        q->head = head->next;
        free(head->value);
        free(head);
        head = q->head;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    if (!q)
        return false;
    /* TODO: What should you do if the q is NULL? */
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    size_t len = strlen(s) + 1;
    newh->value = (char *) malloc(len * sizeof(char));
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, len);
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    newh->next = q->head;
    q->head = newh;
    if (!q->size)
        q->tail = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    /* TODO: What should you do if the q is NULL? */
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    size_t len = strlen(s) + 1;
    newh->value = (char *) malloc(len * sizeof(char));
    newh->next = NULL;
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, len);
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */

    if (!q->size)
        q->head = newh;
    else
        q->tail->next = newh;
    q->tail = newh;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;
    list_ele_t *head = q->head;
    q->head = q->head->next;
    q->size--;
    if (head->value) {
        if (sp) {
            size_t len = bufsize > strlen(head->value) ? strlen(head->value)
                                                       : bufsize - 1;
            strncpy(sp, head->value, len);
            sp[len] = '\0';
        }
        free(head->value);
    }

    free(head);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q ? q->size : 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head)
        return;

    list_ele_t *cursor = NULL;
    q->tail = q->head;
    while (q->head) {
        list_ele_t *next = q->head->next;
        q->head->next = cursor;
        cursor = q->head;
        q->head = next;
    }
    q->head = cursor;
}

list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    list_ele_t *tmp = NULL;
    if (strcmp(l1->value, l2->value) <= 0) {
        tmp = l1;
        l1 = l1->next;
    } else {
        tmp = l2;
        l2 = l2->next;
    }

    list_ele_t *head = tmp;
    while (l1 && l2) {
        if (strcmp(l1->value, l2->value) <= 0) {
            tmp->next = l1;
            tmp = tmp->next;
            l1 = l1->next;
        } else {
            tmp->next = l2;
            tmp = tmp->next;
            l2 = l2->next;
        }
    }

    if (l1)
        tmp->next = l1;
    if (l2)
        tmp->next = l2;

    return head;
}

void merge_sort(list_ele_t **head)
{
    if (!*head || !(*head)->next)
        return;
    list_ele_t *fast = (*head)->next;
    list_ele_t *slow = *head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    merge_sort(head);
    merge_sort(&fast);

    *head = merge(*head, fast);
}
/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head)
        return;

    merge_sort(&q->head);

    while (q->tail->next)
        q->tail = q->tail->next;
}