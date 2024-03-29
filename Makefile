NAME = verle

WFLAGS = #-O3 -mfpmath=sse -fstack-protector-all -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -std=gnu99 -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wold-style-definition -Wdeclaration-after-statement -Wbad-function-cast -Wnested-externs
FLAGS = -g $(WFLAGS)
CC = gcc
INC = -I ./includes

### PATH ###
HEADPATH = includes/
HEADLIST = verle.h
SRCS_PATH = src/
OBJ_PATH = obj/

SRCS_NAME = main.c 					\
			init.c 					\
			malloc_tools.c			\
			free_tools.c			\
			verlet.c				\
			calc_forces.c			\
			ft_strsplit.c			\
			fp_save_step.c			\
			exclude_out_particles.c	

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
