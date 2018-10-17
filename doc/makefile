#second level make file that builds all the documentation
.PHONY:all
all:doxygen technical_report use_cases

.PHONY:use_cases
use_cases:
	@echo "Building Document Describing A Few Algorithm Use Cases"
	@make -C algorithm_usecases

.PHONY:doxygen
doxygen:
	@make -C doxygen

.PHONY:technical_report
technical_report:
	@echo "Building Technical Report"
	@make -C technical_report

.PHONY:clean
clean:
	-@make -C doxygen clean
	-@make -C technical_report clean
	-@make -C use_cases clean

.PHONY:cleanall
cleanall:
	-@make -C doxygen cleanall
	-@make -C technical_report cleanall
	-@make -C use_cases cleanall

.PHONY:cleanall_doxygen
cleanall_doxygen:
	-@make -C doxygen cleanall
