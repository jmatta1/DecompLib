#third level makefile for doxygen which calls doxygen and builds the latex it generates
FILE_LIST=DataVector.h Decomposition.h DecompVector.h Helper.h RespMatrix.h main_page.dox
DOX_LIST=$(FILE_LIST:%=../../DecompLib/%)
.PHONY:doxygen
doxygen:index.html

index.html:html/index.html
	ln -s -f html/index.html ./index.html
	cp latex/refman.pdf ./refman.pdf

# if index.html had to be updated, then so does the pdf manual
# the first sed expression changes the latex build file doxygen makes to pipe pdflatex output to /dev/null
# the second sed expression changes the latex build file doxygen makes to pipe makeindex output to /dev/null
# the dumping to /dev/null is because there is no need for the gigantic output that pdflatex makes to be seen
html/index.html:$(DOX_LIST)
	@echo -e "\e[1m\e[32mRunning Doxygen\e[0m"
	@sleep 1
	doxygen > /dev/null
	@echo -e "\e[1m\e[32mRunning LaTeX for Doxygen\e[0m"
	@sleep 1
	@sed -i 's,pdflatex refman,pdflatex refman > /dev/null,' latex/Makefile
	@sed -i 's,makeindex refman.idx,makeindex refman.idx 2> /dev/null,' latex/Makefile
	make -C latex > /dev/null

.PHONY:clean
clean:
	-rm -f latex/*.ps latex/*.dvi latex/*.aux latex/*.toc latex/*.idx latex/*.ind
	-rm -f latex/*.ilg latex/*.log latex/*.out latex/*.brf latex/*.blg latex/*.bbl

.PHONY:cleanall
cleanall:clean
	-rm -rf latex
	-rm -rf html
	-rm -f ./index.html
	-rm -f ./refman.pdf
