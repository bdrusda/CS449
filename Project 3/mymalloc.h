void *my_firstfit_malloc(int size);
void my_free(void *ptr);

struct node
{
    int size;
    int free;
    struct node *next;
    struct node *prev;
};
