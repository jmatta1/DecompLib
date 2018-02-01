# Here are typical global and local install locations for headers
Global_Header_Loc=/usr/include/DecompLib
Local_Header_Loc=~/include/DecompLib

# select the location you want to install to with this variable, or set something custom
Install_Location=${Global_Header_Loc}

.PHONY:all
all:example doc

.PHONY:example
example:
	@echo -e "\e[1m\e[32mBuilding example\e[0m"
	@make -C Example all

.PHONY:doc
doc:
	@echo -e "\e[1m\e[32mBuilding documentation\e[0m"
	@make -C doc all

.PHONY:clean
clean:
	-@make -i -C doc clean
	-@make -i -C Example clean

.PHONY:cleanall
cleanall:clean
	-@make -i -C doc cleanall
	-@make -i -C Example cleanall

.PHONY:install
install:
	@echo -e "\e[1m\e[91mCreating Destination\e[0m"
	install -b -d ${Install_Location}
	@echo -e "\e[1m\e[91mCopying Headers\e[0m"
	install -b -m 644 -D ./DecompLib/*.h ${Install_Location}

.PHONY:uninstall
uninstall:
	@echo -e "\e[1m\e[91mDeleting Headers\e[0m"
	rm ${Install_Location}/*.h
	@echo -e "\e[1m\e[91mDeleting DecompLib Header Folder\e[0m"
	rm -r ${Install_Location}

