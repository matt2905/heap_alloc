#ifndef TAS_ALLOC_H
#define TAS_ALLOC_H

#define max(a, b) \
	({	__typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _a : _b; })
#define SIZE 128
#define FREE_BLOCK -1

typedef enum e_strategy_type
{
	FIRST,
	BEST,
	WORST
} t_strategy_type;

typedef int (*t_strategy)(unsigned int size);
t_strategy *get_strategy();
int *get_first_libre();

char *get_tas();
char *tas_malloc(unsigned int size);
void tas_free(char *ptr);
void show_tas();

int first_fit(unsigned int size);
int best_fit(unsigned int size);
int worst_fit(unsigned int size);

void set_first_libre(int size, int index, int rest);

#endif