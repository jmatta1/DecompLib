#third level makefile for the technical report which builds the latex
#dump outputs to /dev/null because watching the LaTeX sausage being made is ugly
TR3_nai_sim_and_decomp.pdf: TR3_nai_sim_and_decomp.tex references.bib
	pdflatex TR3_nai_sim_and_decomp > /dev/null
	bibtex TR3_nai_sim_and_decomp > /dev/null
	pdflatex TR3_nai_sim_and_decomp > /dev/null
	pdflatex TR3_nai_sim_and_decomp > /dev/null

.PHONY:clean
clean:
	-rm -f *.ps *.dvi *.aux *.toc *.idx *.ind *.ilg *.log *.out *.brf *.blg *.bbl *.spl

.PHONY:cleanall
cleanall: clean
	-rm TR3_nai_sim_and_decomp.pdf
