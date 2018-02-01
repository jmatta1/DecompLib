#third level makefile for doxygen which calls doxygen and builds the latex it generates
.PHONY:doxygen
doxygen:
	@echo -e "\e[1m\e[32mRunning Doxygen\e[0m"
	@sleep 1
	doxygen
	@echo -e "\e[1m\e[32mRunning LaTeX for Doxygen\e[0m"
	@sleep 1
	@make -C latex

.PHONY:clean
clean:
	-rm -f latex/*.ps latex/*.dvi latex/*.aux latex/*.toc latex/*.idx latex/*.ind latex/*.ilg latex/*.log latex/*.out latex/*.brf latex/*.blg latex/*.bbl

.PHONY:cleanall
cleanall:clean
	-@make -C latex clean
