# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amahla <amahla@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/17 21:07:29 by amahla            #+#    #+#              #
#    Updated: 2022/10/22 17:42:11 by amahla           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL				=	/bin/sh

PROGNAME			:=	webserv

SRCDIR				:=	srcs

INCLUDEDIR			:= headers

OBJDIR				:=	./obj
DEBUGDIR			:=	./debugobj

SRCSMAC				:=	$(addprefix	mac/,	serv_process_mac.cpp		\
											socket_settings_mac.cpp		)

SRCSLINUXSELECT		:=	$(addprefix	linux/,	serv_process_select_linux.cpp		\
											socket_settings_linux.cpp	)

SRCSLINUX			:=	$(addprefix	linux/,	serv_process_select_linux.cpp		\
											socket_settings_linux.cpp	)

SRCSMULTIOS			:=	$(addprefix process/,	main.cpp					\
												webserv.cpp				)	\
						$(addprefix class/,		WebServException.cpp		\
												Server.cpp					\
												Client.cpp					\
												Request.cpp					\
												Response.cpp			)

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

ifdef SELECT
	SRCS				:=	$(SRCSMULTIOS) 							\
							$(addprefix process/,	$(SRCSLINUXSELECT))
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

select				:
ifndef SELECT
	$(MAKE) SELECT=1
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

.PHONY				:	all clean fclean re debug mac select

-include	$(addprefix $(OUTDIR)/,$(SRCS:.cpp=.d))
