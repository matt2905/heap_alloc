CC			?= gcc
CFLAGS		+= -Wall -Wextra --pedantic -Werror -g

SRC			= heap_alloc.c \
			  heap_free.c \
			  heap_global.c \
			  heap_list.c \
			  heap_logger.c \
			  heap_malloc.c \
			  heap_sem.c \
			  heap_strategy.c
MSRCS		= main.c
TESTSRCS	= test/main_test.c \
 
DOBJ		= obj/
NAME		= heap_alloc
INC			= -I includes
OBJ			= $(patsubst %.c, $(DOBJ)%.o, $(SRC))
MOBJ		= $(patsubst %.c, $(DOBJ)%.o, $(MSRCS))
TESTOBJ		= $(patsubst %.c, $(DOBJ)%.o, $(TESTSRCS))
DEPENDS		= -MT $@ -MMD -MP -MF $(subst .o,.d,$@)

DOXYGEN := $(shell command -v doxygen 2> /dev/null)

all: $(NAME)

$(NAME):		$(OBJ) $(MOBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(MOBJ)
	@echo "\033[33m"Compilation of $(NAME) : "\033[32m"Success"\033[0m"

-include		$(OBJ:.o=.d)

$(DOBJ)%.o:		%.c
	@mkdir -p $(DOBJ)
	@mkdir -p $(DOBJ)test
	@$(CC) $(CFLAGS) $(DEPENDS) -c $< -o $@ $(INC)
	@echo "\033[32m$(CC) \033[33m$(CFLAGS) \033[35m$(DEPENDS) \033[36m-c $< -o $@ $(INC)\033[0m"

test: $(OBJ) $(TESTOBJ)
	@$(CC) $(CFLAGS) -o test_func $(OBJ) $(TESTOBJ) $(LIB) -lcunit
	@echo "\033[33m"Compilation of test_func : "\033[32m"Success"\033[0m"
	./test_func

clean:
	@/bin/rm -rf $(DOBJ)
	@echo "\033[31m"Objects of $(NAME) : deleted"\033[0m"

docs:
ifndef DOXYGEN
	@echo "\033[31mPlease install doxygen with apt-get install doxygen on a terminal\033[0m"
else
	@doxygen Doxyfile_config
	@echo "\033[32mDocumentation successfully generated \033[0m"
endif

fclean:			clean
	@/bin/rm -f $(NAME) test_*
	@/bin/rm -rf $(NAME).dSYM
	@/bin/rm -rf html/
	@echo "\033[31m"$(NAME) : deleted"\033[0m"

re:				fclean all

.PHONY:			all clean docs re
