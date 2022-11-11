# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: meudier <meudier@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/17 21:07:29 by amahla            #+#    #+#              #
#    Updated: 2022/11/11 15:00:57 by meudier          ###   ########.fr        #
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

SRCSLINUXSELECT		:=	$(addprefix	linux/,	$(addprefix	select/,	serv_process_select.cpp)	\
																	socket_settings.cpp			)

SRCSLINUXEPOLL		:=	$(addprefix	linux/,	serv_process_epoll.cpp			\
											serv_process_epoll_loop.cpp		\
											serv_process_epoll_utils.cpp	\
											socket_settings.cpp	)

SRCSLINUX			:=	$(addprefix	linux/,	serv_process_epoll_linux.cpp		\
											socket_settings_linux.cpp	)

SRCSMULTIOS			:=	$(addprefix process/,	main.cpp					\
												webserv.cpp				)	\
						$(addprefix class/,		WebServException.cpp		\
												ParseFile.cpp				\
												ParseFileUtils.cpp				\
												Server.cpp					\
												Client.cpp					\
												Request.cpp					\
												Response.cpp			)

SRCS				:=	$(SRCSMULTIOS) 							\
						$(addprefix process/,	$(SRCSLINUXEPOLL))

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
	./cgi/test_cgi.sh


$(OUTDIR)			:
	mkdir $(OUTDIR)

clean				:
	$(RM) -rf $(OBJDIR) $(DEBUGDIR) ./cgi/signIn.cgi ./cgi/autoindex.cgi

fclean				:	clean
	$(RM) -f $(PROGNAME) $(addsuffix .mac,$(PROGNAME)) $(DEBUGNAME)

re					:	fclean
	$(MAKE) $(NAME)

.PHONY				:	all clean fclean re debug mac select

-include	$(addprefix $(OUTDIR)/,$(SRCS:.cpp=.d))
