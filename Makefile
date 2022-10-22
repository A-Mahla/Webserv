# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amahla <amahla@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/17 21:07:29 by amahla            #+#    #+#              #
#    Updated: 2022/10/22 14:34:05 by amahla           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL				=	/bin/sh

PROGNAME			:=	webserv

SRCDIR				:=	srcs

INCLUDEDIR			:= headers

OBJDIR				:=	./obj
DEBUGDIR			:=	./debugobj

SRCSMAC				:=	$(addprefix	mac/,	serv_process_mac.cpp		\
											socket_setting_mac.cpp		)

SRCSLINUX			:=	$(addprefix	linux/,	serv_process_linux.cpp		\
											socket_setting_linux.cpp	)

SRCSMULTIOS			:=	$(addprefix process/,	main.cpp					\
												webserv.cpp				)	\
						$(addprefix class/,		WebServException.cpp	)

SRCS				:=	$(SRCSMULTIOS) 							\
						$(addprefix process/,	$(SRCSLINUX))

CC					:=	c++
RM					:=	rm

CCFLAGS				:=  -std=c++98 -Wall -Wextra -Werror
OPTFLAG				:=

NAME				:=	$(PROGNAME)

OUTDIR				:=	$(OBJDIR)

DEBUGNAME			:=	$(addsuffix .debug,$(PROGNAME))

ifdef DEBUG
	OPTFLAG 		:=	-g
	NAME			:=	$(DEBUGNAME)
	OUTDIR			:=	$(DEBUGDIR)
endif

ifdef MAC
	NAME			:=	$(addsuffix .mac,$(PROGNAME))
	SRCS				:=	$(SRCSMULTIOS) 							\
							$(addprefix process/,	$(SRCSMAC))
endif

all					:	$(NAME)

bonus				:	$(BONUS)

debug				:
ifndef DEBUG
	$(MAKE) DEBUG=1
endif

mac					:
ifndef MAC
	$(MAKE) MAC=1
endif

$(OUTDIR)/%.o		:	$(SRCDIR)/%.cpp | $(OUTDIR)
	@mkdir -p $(dir $@)
	$(CC) -c -MMD -MP $(CCFLAGS) $(OPTFLAG) $(addprefix -I ,$(INCLUDEDIR)) $< -o $@

$(NAME)				:	$(addprefix $(OUTDIR)/,$(SRCS:.cpp=.o))
	$(CC) $(CCFLAGS) $(OPTFLAG) -o $(NAME) $^


$(OUTDIR)			:
	mkdir $(OUTDIR)

clean				:
	$(RM) -rf $(OBJDIR) $(DEBUGDIR)

fclean				:	clean
	$(RM) -f $(PROGNAME) $(addsuffix .mac,$(PROGNAME)) $(DEBUGNAME)

re					:	fclean
	$(MAKE) $(NAME)

.PHONY				:	all clean fclean re debug

-include	$(addprefix $(OUTDIR)/,$(SRCS:.cpp=.d))
