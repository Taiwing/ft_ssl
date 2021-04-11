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

CYPHERSDIR		=	cyphers
HASHDIR			=	hash
CMDFDIR			=	cmdf
RSADIR			=	rsa

SRCC			=	readfile.c\
					main.c\
					input_cmd.c\
					exec_cmd.c\

CYPHERSC		=	parse_des_options.c\
					des_io.c\
					cmd_base64.c\
					base64_readfile.c\
					des.c\
					get_salty.c\
					cmd_des.c\
					des_const.c\
					base64.c\
					des_cem.c\

HASHC			=	sha256.c\
					md.c\
					cmd_md.c\
					md5.c\

CMDFC			=	debug.c\
					options.c\
					commands.c\
					internal_commands.c\
					help.c\
					output_option.c\

RSAC			=	parse_rsa_key_headers.c\
					rsa_des.c\
					get_rand.c\
					modular_ops.c\
					cmd_genrsa.c\
					parse_rsa_key.c\
					cmd_rsautl.c\
					prime.c\
					rsa_hexdump.c\

ODIR			=	obj
OBJ				=	$(patsubst %.c,%.o,$(CYPHERSC))\
					$(patsubst %.c,%.o,$(HASHC))\
					$(patsubst %.c,%.o,$(CMDFC))\
					$(patsubst %.c,%.o,$(RSAC))\
					$(patsubst %.c,%.o,$(SRCC))\

vpath			%.o	$(ODIR)
vpath			%.h	$(HDIR)
vpath			%.h	$(SUB1D)/$(HDIR)
vpath			%.c	$(SRCDIR)/$(CYPHERSDIR)
vpath			%.c	$(SRCDIR)/$(HASHDIR)
vpath			%.c	$(SRCDIR)/$(CMDFDIR)
vpath			%.c	$(SRCDIR)/$(RSADIR)
vpath			%.c	$(SRCDIR)

############################## BUILD ###########################################

all: $(NAME)

$(NAME): $(SUB1D)/libft.a $(ODIR) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(patsubst %.o,$(ODIR)/%.o,$(OBJ)) $(LIBS)

$(SUB1D)/libft.a:
	make -C $(SUB1D)

parse_des_options.o: libft.h cmd_des_utils.h commands.h options.h des.h md.h
des_io.o: commands.h options.h libft.h readfile.h base64.h des.h
cmd_base64.o: commands.h options.h libft.h readfile.h base64.h
base64_readfile.o: readfile.h base64.h libft.h
des.o: des.h
get_salty.o: readfile.h cmd_des_utils.h commands.h options.h libft.h des.h
cmd_des.o: cmd_des_utils.h commands.h options.h libft.h des.h base64.h debug.h
des_const.o: des.h
base64.o: base64.h libft.h
des_cem.o: des.h
sha256.o: md.h libft.h
md.o: md.h libft.h
cmd_md.o: commands.h options.h libft.h readfile.h md.h
md5.o: md.h libft.h
readfile.o: readfile.h libft.h
main.o: commands.h options.h libft.h
input_cmd.o: commands.h options.h libft.h
exec_cmd.o: commands.h options.h libft.h readfile.h
debug.o: libft.h des.h
options.o: options.h libft.h
commands.o: commands.h options.h libft.h help.h
internal_commands.o: commands.h options.h libft.h
output_option.o: libft.h
parse_rsa_key_headers.o: libft.h rsa.h des.h cmd_des_utils.h commands.h\
	options.h
rsa_des.o: rsa.h des.h cmd_des_utils.h commands.h options.h libft.h
modular_ops.o: rsa_math.h
cmd_genrsa.o: commands.h options.h libft.h rsa_math.h rsa.h des.h base64.h
parse_rsa_key.o: base64.h libft.h rsa.h des.h
cmd_rsautl.o: commands.h options.h libft.h rsa.h des.h readfile.h rsa_math.h
prime.o: libft.h rsa_math.h
rsa_hexdump.o: libft.h
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
