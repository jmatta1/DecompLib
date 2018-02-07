#second level make file that builds all the documentation
.PHONY:all
all:doxygen technical_report

.PHONY:doxygen
doxygen:
	@make -C doxygen

.PHONY:technical_report
technical_report:
	@echo -e "\e[1m\e[32mMaking Technical Report\e[0m"
	@sleep 1
	@make -C technical_report

.PHONY:clean
clean:
	-@make -C doxygen clean
	-@make -C technical_report clean

.PHONY:cleanall
cleanall:
	-@make -C doxygen cleanall
	-@make -C technical_report cleanall

.PHONY:cleanall_doxygen
cleanall_doxygen:
	-@make -C doxygen cleanall
