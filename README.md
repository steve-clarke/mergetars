# Mergetars

Mergetars is a command-line utility program that merges the contents of multiple archive files into a single tar archive, purging duplicate files as it goes.

If Mergetars finds the same file in multiple archives, it will only save the one that was modified last.

If the duplicate files have the same modification time, it will select the larger file. If *utime* and size are the same, it will choose the file in order of command line precedence.

## Usage

```
mergetars [-v] input_archive1.tar [input_archive2.tar ... ] output_file.tar
```

You can supply as many archive files as necessary, as long as you specify a novel output tarfile name as the final argument.

### Verbosity

Users can turn on verbose mode with the -v flag. 
This will print information to stdout about the program's operation.

Error messages are printed to stdout by default.

## Authors

[Steve Clarke](https://github.com/steve-clarke)

[Jamie McCullough](https://github.com/jmemcc)
