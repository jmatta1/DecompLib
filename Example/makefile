#second level make file that builds the example and associated utilities
.PHONY:all
all:csvtorootth1 csvtorootth2 decomposespectrum rootth1tocsv rootth2tocsv

.PHONY:csvtorootth1
csvtorootth1:
	@echo "Building CsvToRootTh1 Utility"
	@make -C CsvToRootTh1

.PHONY:csvtorootth2
csvtorootth2:
	@echo "Building CsvToRootTh2 Utility"
	@make -C CsvToRootTh2

.PHONY:decomposespectrum
decomposespectrum:
	@echo "Building DecomposeSpectrum Example"
	@make -C DecomposeSpectrum

.PHONY:rootth1tocsv
rootth1tocsv:
	@echo "Building RootTh1ToCsv Utility"
	@make -C RootTh1ToCsv

.PHONY:rootth2tocsv
rootth2tocsv:
	@echo "Building RootTh2ToCsv Utility"
	@make -C RootTh2ToCsv

.PHONY:clean
clean:
	-@make -C DecomposeSpectrum clean

.PHONY:cleanall
cleanall:
	-@make -C CsvToRootTh1 cleanall
	-@make -C CsvToRootTh2 cleanall
	-@make -C DecomposeSpectrum cleanall
	-@make -C RootTh1ToCsv cleanall
	-@make -C RootTh2ToCsv cleanall
