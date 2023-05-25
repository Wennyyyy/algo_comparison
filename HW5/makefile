.PHONY: test

###################################################################
# For Windows environment, please use the following makefile code #
###################################################################
CXX=g++
# CXXFLAGS=-Wall -O3
EXEC=hw5_q.exe
SRC=$(wildcard hw5_q*.c*)
# The input data is in "data" folder by default.
INPUT_DIR=data
INPUT_FILES=$(sort $(wildcard $(INPUT_DIR)/*.txt))

all: $(EXEC)

$(EXEC): $(SRC)
	@$(CXX) $(CXXFLAGS) -o $(EXEC) $(SRC)


test:
	@for %%f in ($(INPUT_FILES)) do .\$(EXEC) < "%%f"

clean:
	@del $(EXEC)


# ###############################################################
# #For Linux environment, please use the following makefile code#
# ###############################################################
# CXX=g++
# # CXXFLAGS=-Wall -O3
# EXEC=hw5_pns.exe
# SRC=$(wildcard hw5_pns*.c*)
# # The input data is in "data" folder by default.
# INPUT_DIR=data
# INPUT_FILES=$(sort $(wildcard $(INPUT_DIR)/*.txt))

# all: $(EXEC)

# $(EXEC): $(SRC)
# 	@$(CXX) $(CXXFLAGS) -o $(EXEC) $(SRC)

# test:
# 	@for f in $(INPUT_FILES); do \
# 		./$(EXEC) < "$$f"; \
# 	done

# clean:
# 	@rm -f $(EXEC)