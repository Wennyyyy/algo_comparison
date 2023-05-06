TARGET = algo4_bruteforce.exe testcaseproduction.exe

all: $(TARGET)

%.exe: %.cpp
	g++ $^ -o $@

clean:
ifeq ($(OS),Windows_NT)
	del *.exe *.log
else
	rm -rf *.exe *.log
endif