# Here are typical global and local install locations for headers
Global_Header_Loc=/usr/include
Local_Header_Loc=~/include

Ginstall_Location=${Global_Header_Loc}/decomplib
Linstall_Location=${Local_Header_Loc}/decomplib

.PHONY:all
all:example docs

.PHONY:example
example:
	@echo "Building Example and Utilities"
	@make -C Example all

.PHONY:docs
docs:
	@echo "Building Documentation"
	@make -C doc all

.PHONY:clean
clean:
	@echo "Cleaning Intermediates in Doc Directory"
	-@make -i -C doc clean
	@echo "Cleaning Intermediates in Example Directory"
	-@make -i -C Example clean

.PHONY:cleanall
cleanall:
	@echo "Cleaning Everything in doc Directory"
	-@make -i -C doc cleanall
	@echo "Cleaning Everything in Example Directory"
	-@make -i -C Example cleanall

.PHONY:install
install:
	@echo "Creating Destination"
	install -b -d ${Ginstall_Location}
	@echo "Copying Headers"
	install -b -m 644 -D ./DecompLib/*.h ${Ginstall_Location}

.PHONY:uninstall
uninstall:
	@echo "Deleting Headers"
	rm ${Ginstall_Location}/*.h
	@echo "Deleting DecompLib Header Folder"
	rm -r ${Ginstall_Location}

.PHONY:local_install
local_install:
	@echo "Creating Destination"
	install -b -d ${Linstall_Location}
	@echo "Copying Headers"
	install -b -m 644 -D ./DecompLib/*.h ${Linstall_Location}

.PHONY:local_uninstall
local_uninstall:
	@echo -e "Deleting Headers"
	rm ${Linstall_Location}/*.h
	@echo -e "Deleting DecompLib Header Folder"
	rm -r ${Linstall_Location}

.PHONY:gh-pages
gh-pages:
	@echo -e "Building Github Pages documentation"
	-@rm -rf docs
	@make -C doc doxygen
	@mv doc/doxygen/html docs
	grep -lr "http://cdn.mathjax.org" docs | xargs sed -i 's,http://cdn.mathjax.org,https://cdn.mathjax.org,'
	-@make -i -C doc cleanall_doxygen
