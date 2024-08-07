SHELL    := /bin/sh

OBJDIR   := objdir

CXXFLAGS += -I./include -Wall -Wextra -Werror -std=c++98
ifeq ($(DEBUG),true)
	CXXFLAGS += -g3
endif
LDFLAGS  +=
LDLIBS   +=
CXX      := c++
LINK.cpp := $(filter-out $(CXXFLAGS), $(LINK.cpp))

vpath %.cpp $(shell find srcs -type d)

SRCS     := main.cpp WebServer.cpp ServerConfig.cpp ConfigParser.cpp DirectiveHandler.cpp \
			Socket.cpp Poll.cpp ReqParsing.cpp ServerUtils.cpp ReqParsUtils.cpp Response.cpp \
			ResponseUtils.cpp Cgi.cpp
OBJS     := $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
DEPS     := $(OBJS:.o=.d)
NAME     := webserv

.PHONY: all clean fclean re options

all: $(NAME)

$(NAME): $(OBJS)
	$(LINK.cpp) $(LDLIBS) $(OUTPUT_OPTION) $^

$(OBJDIR)/%.o: %.cpp Makefile | $(OBJDIR)
	$(COMPILE.cpp) $(OUTPUT_OPTION) -MMD -MP $<

-include $(DEPS)

$(OBJDIR):
	mkdir $@

clean:
	$(RM) $(OBJS) $(DEPS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

options:
	@echo "$(NAME) build options:"
	@echo "CXXFLAGS      = $(CXXFLAGS)"
	@echo "LDFLAGS       = $(LDFLAGS)"
	@echo "LDLIBS        = $(LDLIBS)"
	@echo "CXX           = $(CXX)"
