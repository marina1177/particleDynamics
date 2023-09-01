NAME = verle

WFLAGS = #-Wall -Werror -Wextra
FLAGS = -g $(WFLAGS)
CC = gcc
INC = -I ./includes

### PATH ###
HEADPATH = includes/
HEADLIST = verle.h
SRCS_PATH = src/
OBJ_PATH = obj/

SRCS_NAME = main.c \
			mem_manage.c \
			print_output.c \
			
			init.c \
			acceleration.c	\
			malloc_tools.c \
			free_tools.c	\
			verlet.c		\
			ft_strsplit.c	\
			fp_save_step.c

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))
OBJ = $(addprefix $(OBJ_PATH), $(SRCS_NAME:.c=.o))


### COMPILE ###
all: $(NAME)

$(OBJ_PATH):
		@mkdir -p $(OBJ_PATH)

$(OBJ_PATH)%.o: $(SRCS_PATH)%.c includes/verle.h
		@echo "\033[34m\033[1mCompilation of \033[0m\033[36m$(notdir $<)\033[1m\033[34m done.\033[0m"
		$(CC)  $(WFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJ_PATH) $(OBJ)
		$(CC) $(OBJ) $(INC) -lm  -o $(NAME)
		@echo "\033[32mBinary \033[1;32m$(NAME)\033[1;0m\033[32m created.\033[0m"


clean:
		@/bin/rm -rf $(OBJ_PATH)
		@/bin/rm -rf vis.json
		@echo "\033[1;31m$(NAME) \033[1;0m\033[31mobjects files removed.\033[0m"


fclean: clean
		@/bin/rm -f $(NAME)
		@echo "\033[31mBin \033[1;31m$(NAME)\033[1;0m\033[31m removed.\033[0m"

re: fclean all

.PHONY: all clean fclean re
