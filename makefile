# Here are typical global and local install locations for headers
Global_Header_Loc=/usr/include
Local_Header_Loc=~/include

Ginstall_Location=${Global_Header_Loc}/decomplib
Linstall_Location=${Local_Header_Loc}/decomplib

.PHONY:all
all:example doc

.PHONY:example
example:
	@echo -e "\e[1m\e[32mBuilding Example and Utilities\e[0m"
	@make -C Example all

.PHONY:doc
doc:
	@echo -e "\e[1m\e[32mBuilding Documentation\e[0m"
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
	install -b -d ${Ginstall_Location}
	@echo -e "\e[1m\e[91mCopying Headers\e[0m"
	install -b -m 644 -D ./DecompLib/*.h ${Ginstall_Location}

.PHONY:uninstall
uninstall:
	@echo -e "\e[1m\e[91mDeleting Headers\e[0m"
	rm ${Ginstall_Location}/*.h
	@echo -e "\e[1m\e[91mDeleting DecompLib Header Folder\e[0m"
	rm -r ${Ginstall_Location}

.PHONY:local_install
local_install:
	@echo -e "\e[1m\e[91mCreating Destination\e[0m"
	install -b -d ${Linstall_Location}
	@echo -e "\e[1m\e[91mCopying Headers\e[0m"
	install -b -m 644 -D ./DecompLib/*.h ${Linstall_Location}

.PHONY:local_uninstall
local_uninstall:
	@echo -e "\e[1m\e[91mDeleting Headers\e[0m"
	rm ${Linstall_Location}/*.h
	@echo -e "\e[1m\e[91mDeleting DecompLib Header Folder\e[0m"
	rm -r ${Linstall_Location}

