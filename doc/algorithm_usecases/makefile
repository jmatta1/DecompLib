algorithm_usescases.pdf: algorithm_usecases.tex
	pdflatex algorithm_usecases > /dev/null
	pdflatex algorithm_usecases > /dev/null
	pdflatex algorithm_usecases > /dev/null

.PHONY:clean
clean:
	-rm -f *.ps *.dvi *.aux *.toc *.idx *.ind *.ilg *.log *.out *.brf *.blg *.bbl *.spl

.PHONY:cleanall
cleanall: clean
	-rm algorithm_usescases.pdf
