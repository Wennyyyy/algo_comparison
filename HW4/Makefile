# ###################################################################
# # For Windows environment, please use the following makefile code #
# ###################################################################
# CXX=g++
# # CXXFLAGS=-Wall -O3
# EXEC=hw4_bnb.exe
# #SRC=$(wildcard hw4_bnb*.c*)
# # The input data is in "data" folder by default.
# INPUT_DIR=data
# INPUT_FILES=$(sort $(wildcard $(INPUT_DIR)/*.txt))

# all: $(EXEC)

# %.exe: %.cpp
# 	@$(CXX) $(CXXFLAGS) -o $@ $^

# test:
# 	@for %%f in ($(INPUT_FILES)) do .\$(EXEC) < "%%f"

# clean:
# 	@del *.exe *.log


###############################################################
#For Linux environment, please use the following makefile code#
###############################################################
CXX=g++
# CXXFLAGS=-Wall -O3
EXEC=hw4_bnb.exe
SRC=$(wildcard hw4_bnb*.c*)
# The input data is in "data" folder by default.
INPUT_DIR=data
INPUT_FILES=$(sort $(wildcard $(INPUT_DIR)/*.txt))

all: $(EXEC)

$(EXEC): $(SRC)
	@$(CXX) $(CXXFLAGS) -o $(EXEC) $(SRC)

test:
	@for f in $(INPUT_FILES); do \
		./$(EXEC) < "$$f"; \
	done

clean:
	@rm -f $(EXEC)