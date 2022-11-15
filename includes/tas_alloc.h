#ifndef TAS_ALLOC_H
#define TAS_ALLOC_H

#define max(a, b) \
	({	__typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _a : _b; })
#define SIZE 128

typedef enum e_strategy_type
{
	FIRST,
	BEST,
	WORST
} t_strategy_type;

typedef int (*t_strategy)(unsigned int taille);
t_strategy *get_strategy();
int *get_first_libre();

char *get_tas();
char *tas_malloc(unsigned int taille);
int tas_free(char *ptr);
void show_tas();

int first_fit(unsigned int taille);
int best_fit(unsigned int taille);
int worst_fit(unsigned int taille);

#endif