NAME = PmergeMe

INCLUDES = inc
SRCS = srcs/
SRC =   $(SRCS)main.cpp \
		$(SRCS)PmergeMe.cpp

OBJ_DIR = obj
OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRC)))
DEP = $(OBJ:.o=.d)

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g3 -I$(INCLUDES) -MMD -MP

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR)/%.o: $(SRCS)%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re