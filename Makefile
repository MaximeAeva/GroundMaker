EXE  =		grd
DEBUG =  	yes
CXX =		g++

# dir
SRC_DIR =       src
INC_DIR =       inc
OBJ_DIR =       obj

# sources
PROJ_SRC_LIST = main.cpp glad.cpp shader.cpp dsq.cpp

# objects
PROJ_OBJ_LIST = $(PROJ_SRC_LIST:.cpp=.o)

#paths
PROJ_SRC =		$(addprefix $(SRC_DIR)/, $(PROJ_SRC_LIST))
PROJ_OBJ =		$(addprefix $(OBJ_DIR)/, $(PROJ_OBJ_LIST))

ifeq ($(DEBUG),yes)
CXXFLAGS = -std=c++17 -Wall -Wextra #-ansi -pedantic -g -Weffc++ 
else
CXXFLAGS = -std=c++17 -w -Wall -Werror -s -O2
endif

#libs
IFLAGS =	-I./inc
LFLAGS =	-L./lib
LDLIBS = 	-lglew32 -lopengl32 -lWs2_32 -lole32 -lcomctl32 -lgdi32 -lcomdlg32 -luuid -lglut32 -lglfw3dll


all :		 	$(EXE)


#Creating .exe with .o
$(EXE) :    $(PROJ_OBJ)
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LFLAGS) $(LDLIBS) 
	@echo "Compiling "$(EXE)


# Creating .o with .cpp and libs
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp  
	@$(shell mkdir obj)
	@$(CXX) $(CXXFLAGS) -c -o $@ $< $(IFLAGS) 
	@echo "Compiling" $<

clean :
	@echo "Cleaning Object"
	@$(shell rmdir /S /Q obj)

fclean :	    clean
	@echo "Removing Binary"
	@$(shell rm $(EXE))

re :		    fclean all

.PHONY :	    all, clean, fclean, re

.SILENT :
