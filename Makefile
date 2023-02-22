CXX ?= g++
CXXFLAGS ?= -Wall -Werror -pedantic -g --std=c++11

#run: calc costs.in.txt
#	@./main

# Compile the main executable
blissdle: main.cpp Blissdle.h
	$(CXX) $(CXXFLAGS) main.cpp -o blissdle

# Remove automatically generated files
clean :
	rm -rvf *.exe *~ *.out *.dSYM *.stackdump

.PHONY: poetl weddle wardle

poetl:
	open https://poeltl.dunk.town
	open https://www.gannett-cdn.com/usatoday/editorial/sports/nba/nba-projections-2016.jpg

weddle:
	open https://www.weddlegame.com
	open https://images.squarespace-cdn.com/content/v1/5ce3ea91ae2b190001d02fba/1601444797075-M9N6ZBUUCKQZV1FD1CHC/Teams+by+division.jpg?format=1000w

wardle:
	open https://wardle.app
	open https://i.imgur.com/MtlMYqk.jpg
