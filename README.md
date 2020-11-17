# Mergetars

## Usage
Mergetars is a command-line utility program that merges the contents of multiple archive files into a single tar archive, purging duplicate files as it goes.

If Mergetars finds the same file in multiple archives, it will only save the one that was modified last.

If the duplicate files have the same modification time, it will select the larger file. If modtime and size are the same, it will choose the file from the last argument in the command line.

## About
Mergetars was originally authored under CITS2002, the University of Western Australia's second-year systems programming unit run by Dr. Chris McDonald.

## Authors

[Steve Clarke](https://github.com/steve-clarke)

[Jamie McCullough](https://github.com/jmemcc)
