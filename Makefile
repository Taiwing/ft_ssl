############################## COMPILE VAR #####################################

CC			=	gcc
#CFLAGS		=	-Wall -Wextra -Werror
CFLAGS		=	-g -Wall -Wextra -Werror -fsanitize=address,undefined
HDIR		=	includes
SRCDIR		=	src
SUB1D		=	libft
HFLAGS		=	-I $(HDIR) -I $(SUB1D)/$(HDIR)
LIBS		=	$(SUB1D)/libft.a
NAME		=	ft_ssl

############################## SOURCES #########################################

CMDFDIR			=	cmdf

SRCC			=	readfile.c\
					main.c\
					input_cmd.c\
					exec_cmd.c\

CMDFC			=	options.c\
					commands.c\
					internal_commands.c\
					sha256.c\
					md.c\
					help.c\
					cmd_md.c\
					md5.c\

ODIR			=	obj
OBJ				=	$(patsubst %.c,%.o,$(CMDFC))\
					$(patsubst %.c,%.o,$(SRCC))\

vpath			%.o	$(ODIR)
vpath			%.h	$(HDIR)
vpath			%.h	$(SUB1D)/$(HDIR)
vpath			%.c	$(SRCDIR)/$(CMDFDIR)
vpath			%.c	$(SRCDIR)

############################## BUILD ###########################################

all: $(NAME)

$(NAME): $(SUB1D)/libft.a $(ODIR) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(patsubst %.o,$(ODIR)/%.o,$(OBJ)) $(LIBS)

$(SUB1D)/libft.a:
	make -C $(SUB1D)

readfile.o: readfile.h libft.h
main.o: commands.h options.h libft.h
input_cmd.o: commands.h options.h libft.h
exec_cmd.o: commands.h options.h libft.h
options.o: options.h libft.h
commands.o: commands.h options.h libft.h help.h
internal_commands.o: commands.h options.h libft.h
sha256.o: md.h libft.h
md.o: md.h libft.h
cmd_md.o: commands.h options.h libft.h readfile.h md.h
md5.o: md.h libft.h
%.o: %.c
	@mkdir -p $(ODIR)
	$(CC) -c $(CFLAGS) $< $(HFLAGS) -o $(ODIR)/$@

$(ODIR):
	mkdir -p $@

############################## CLEANUP #########################################

clean:
	rm -rf $(ODIR)
	make -C $(SUB1D) fclean

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
