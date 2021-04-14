################################################################
# AUTHOR INFORMATION                                           #
################################################################

AUTHOR      = "UrielMayo,AlejoBados,LadislaoRodriguez"
CLASS       = "R1042"
TP          = "Sprint1"
YEAR        = "2019"

################################################################
# COMPILER AND ARGUMENTS                                       #
################################################################

CC          = gcc
GG 			= g++
CVFLAGS 	= `pkg-config --cflags --libs opencv`
CFLAGS      = -c -Wall
LDFLAGS     = -lm -Wall

################################################################
# SOURCES, HEADERS, OBJECTS AND EXECUTABLE                     #
################################################################

C_DIR     	= src/Cliente/
S_DIR		= src/Servidor/
INC_DIR		= headers/
BUILD_DIR	= build

BUILD_DIR_S = build/Servidor/
BUILD_DIR_C = build/Cliente/

BIN_DIR     = bin
SRC_C       = $(shell find $(C_DIR) -name '*.c')
OBJS_C      = $(patsubst $(C_DIR)%.c, $(BUILD_DIR_C)%.o, $(SRC_C))

SRC_S		= $(shell find $(S_DIR) -name '*.c')
OBJS_S      = $(patsubst $(S_DIR)%.c, $(BUILD_DIR_S)%.o, $(SRC_S))

HEADERS     = $(shell find $(INC_DIR) -name '*.h')

EXEC_S       = servidor
EXEC_C		 = cliente

################################################################
# TAR FILE INFORMATION                                         #
################################################################

FILE_NAME   = $(AUTHOR)-$(CLASS)-$(TP)-$(YEAR).tar.gz

################################################################
# TEXT EDITOR                                                  #
################################################################

TEXT_EDITOR = code

################################################################
# DOCUMENTATION                                                #
################################################################

DOC_GEN     = doxygen
DOC_FILE    = Doxyfile
DOC_DIR     = doxy
HTML_DIR	= html
LAT_DIR		= latex

################################################################
# MAKE TARGETS                                                 #
################################################################

.PHONY: servidor cliente clean compress edit doc help

# LINK CLIENT
$(EXEC_C): $(OBJS_C)
	@echo ''
	@echo '*****************************************************'
	@echo '---> Linking...'
	mkdir -p $(BIN_DIR)
	$(GG) $(OBJS_C) $(LDFLAGS) $(CVFLAGS) -o $(BIN_DIR)/$@
	@echo '---> Linking Complete!'
	@echo '*****************************************************'
	@echo ''

#COMPILE CLIENT
$(BUILD_DIR_C)%.o: $(C_DIR)%.c Makefile
	@echo ''
	@echo '*****************************************************'
	@echo '---> Compiling...'
	mkdir -p $(dir $@)
	$(GG) $(CFLAGS) -I$(INC_DIR) $< -o $@
	@echo '---> Compiling Complete!'
	@echo '*****************************************************'
	@echo ''

# LINK SERVER
$(EXEC_S): $(OBJS_S)
	@echo ''
	@echo '*****************************************************'
	@echo '---> Linking...'
	mkdir -p $(BIN_DIR)
	$(CC) $(OBJS_S) $(LDFLAGS) $(LIBS) -o $(BIN_DIR)/$@ -ldirs -L./
	@echo '---> Linking Complete!'
	@echo '*****************************************************'
	@echo ''

# COMPILE SERVER
$(BUILD_DIR_S)%.o: $(S_DIR)%.c Makefile
	@echo ''
	@echo '*****************************************************'
	@echo '---> Compiling...'
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) $< -o $@
	@echo '---> Compiling Complete!'
	@echo '*****************************************************'
	@echo ''

clean:
	@echo ''
	@echo '*****************************************************'
	@echo '---> Cleaning...'
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(DOC_DIR) $(HTML_DIR) $(LAT_DIR)
	@echo '---> Cleaning Complete!'
	@echo '*****************************************************'
	@echo ''

compress: $(SRC_DIR) Makefile $(DOC_FILE)
	@echo ''
	@echo '*****************************************************'
	@echo '---> Packing...'
	tar -zcvf $(FILE_NAME) $(SRC_DIR) Makefile $(DOC_FILE)
	@echo '---> Packing Complete!'
	@echo '*****************************************************'
	@echo ''

edit:
	@echo ''
	@echo '*****************************************************'
	@echo '---> Editing...'
	$(TEXT_EDITOR) .

doc:
	@echo ''
	@echo '*****************************************************'
	@echo '---> Generating Code Documentation...'
	$(DOC_GEN) $(DOC_FILE)
	@echo '---> Generating Code Documentation Complete!'
	@echo '*****************************************************'
	@echo ''

help:
	@echo ''
	@echo '*****************************************************'
	@echo '  Uso:'
	@echo '    make all:       Compiles and Links'
	@echo '    make clean:     Removes objects and executable'
	@echo '    make compress:  Generates .tar.gz file'
	@echo '    make edit:      Opens code files with text editor'
	@echo '    make doc:       Generates code documentation'
	@echo '    make run:       Runs executable'
	@echo '    make help:      Shows help'
	@echo '*****************************************************'
	@echo ''
