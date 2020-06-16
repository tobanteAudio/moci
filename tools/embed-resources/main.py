"""Embed the given resources in a C file.

Example:

    embed_file foo foo.rsrc

    Creates foo.c with:

      const char foo[] = { /* bytes of resource foo */ };
      const size_t foo_len = sizeof(foo);

    Which you can use with from C++:

      extern "C" const char foo[];
      extern "C" const size_t foo_len;
"""


import subprocess
import distutils.ccompiler
import tempfile
import shutil
import os
import argparse
import sys


def open_file(path, mode='r'):
    """
    Open file with an option to route from stdin or stdout as needed.
    """
    if path == '-':
        if mode.count('r'):
            f = sys.stdin
        else:
            f = sys.stdout
    else:
        f = open(path, mode)

    return f


def main(argv=None):
    if argv is None:
        argv = sys.argv

    # Parse main arguments
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawTextHelpFormatter)

    parser.add_argument('symbol', type=str, nargs=1,
                        help='Name of resources symbol, SYMBOL.cpp default output')
    parser.add_argument('input', type=str, nargs=1, help='Input file ')

    args = parser.parse_args(argv[1:])

    # Read our resource
    f = open_file(args.input[0])
    contents = f.read()

    # Open our output file
    symbol = args.symbol[0]
    sourceFile = open_file(f"{symbol}.cpp", 'w')
    bytesWritten = 0
    try:
        # Header for file
        sourceFile.write("#include <cstdlib>\n")
        sourceFile.write("#include <cstdint>\n\n")
        sourceFile.write(f"#include \"{symbol}.hpp\"\n\n")

        sourceFile.write("namespace moci::resources {\n")

        sourceFile.write(f"const char {symbol}[] = {{ \n")

        # Write the contents, 11 bytes per row (fits within 80 chars)
        linecount = 0

        for ch in contents:
            # Nicely format things in rows
            sourceFile.write('%-06s ' % ('0x%0X,' % ord(ch)))
            bytesWritten = bytesWritten + 1

            linecount += 1

            if linecount == 11:
                sourceFile.write("\n  ")

                linecount = 0

        # Write footer
        sourceFile.write('\n};\n\n')
        # Write size
        # Close namespace
        sourceFile.write('\n}\n\n')
    finally:
        sourceFile.close()

    headerFile = open_file(f"{symbol}.hpp", 'w')
    try:
        # Header for file
        headerFile.write("#include <cstdlib>\n")
        headerFile.write("#include <cstdint>\n")
        headerFile.write("\n")

        # Open namespace
        headerFile.write("namespace moci::resources {\n")

        # Write declarations
        headerFile.write(f"extern \"C\" const char {symbol}[];\n")
        headerFile.write(
            f'constexpr std::size_t {symbol}_len = {len(contents)};\n\n')

        # Close namespace
        headerFile.write('\n}\n\n')
    finally:
        headerFile.close()


if __name__ == '__main__':
    sys.exit(main())
