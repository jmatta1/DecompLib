#!/usr/bin/python
import sys
import subprocess
import os

def main(root_fn, name_base, out_base):
    for suffix in SUFFIX_LIST:
        cmd_list = ["./convertCsv", "{0:s}{1:s}.csv".format(name_base, suffix),
                    root_fn, "{0:s}{1:s}".format(out_base, suffix)]
        if os.path.exists("./{0:s}".format(cmd_list[1])):
            subprocess.call(cmd_list)


SUFFIX_LIST = ["_init", "_final", "_sum", "_resid"]

HELP_STR = """write_spectrum_set.py
A simple script to automatically write DecomposeSpectra outputs to a root file.
This script does not pass a minimum cutoff value to convertCsv, causing it to
default to 1.0e-10

Usage:
    {0:s} <RootFile> <BaseName> <OutBase>

  RootFile  - the name of the ROOT file that the spectra should be written to
  BaseName  - the name given to DecomposeSpectra as the base for its 4 outputs
   OutBase  - the base name of each of the spectra added to the ROOT file
"""


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print HELP_STR.format(sys.argv[0])
        sys.exit()
    root_fn = sys.argv[1]
    name_base = sys.argv[2]
    out_base = sys.argv[3]
    main(root_fn, name_base, out_base)
