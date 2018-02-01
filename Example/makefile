#second level make file that builds the example and associated utilities
.PHONY:all
all:csvtorootth1 decomposespectrum rootth1tocsv rootth2tocsv

.PHONY:csvtorootth1
csvtorootth1:
	@echo -e "\e[1m\e[32mBuilding CsvToRootTh1 Utility\e[0m"
	@make -C CsvToRootTh1

.PHONY:decomposespectrum
decomposespectrum:
	@echo -e "\e[1m\e[32mBuilding DecomposeSpectrum Example\e[0m"
	@make -C DecomposeSpectrum

.PHONY:rootth1tocsv
rootth1tocsv:
	@echo -e "\e[1m\e[32mBuilding RootTh1ToCsv Utility\e[0m"
	@make -C RootTh1ToCsv

.PHONY:rootth2tocsv
rootth2tocsv:
	@echo -e "\e[1m\e[32mBuilding RootTh2ToCsv Utility\e[0m"
	@make -C RootTh2ToCsv

.PHONY:clean
clean:
	-@make -C DecomposeSpectrum clean

.PHONY:cleanall
cleanall:
	-@make -C CsvToRootTh1 cleanall
	-@make -C DecomposeSpectrum cleanall
	-@make -C RootTh1ToCsv cleanall
	-@make -C RootTh2ToCsv cleanall
