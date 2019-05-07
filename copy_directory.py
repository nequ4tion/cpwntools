#!/usr/bin/env python2.7

import argparse
import sys
import os
import shutil

def main(args):
    parser = argparse.ArgumentParser(description='Copies files from one place to another while preserving the directory structure. DO NOT use outside of this project.')
    parser.add_argument('--infile', required=True)
    parser.add_argument('--outdir', required=True)
    parser.add_argument('--extern', required=True)
    args = parser.parse_args(args[1:])
    infile = os.path.realpath(args.infile)
    infile_dir = os.path.dirname(infile)
    if args.extern == "no":
        infile = infile[infile.rfind('src')+4:]
        infile_dir = infile_dir[infile_dir.rfind('src')+4:]
    elif args.extern == "yes":
        infile = infile[infile.rfind('cpwn_extern'):]
        infile_dir = infile_dir[infile_dir.rfind('cpwn_extern'):]
    else:
        print("--extern yes/no")
        return 1
    if args.outdir.endswith('/'):
        directory_to_copy_to = args.outdir + infile_dir
    else:
        directory_to_copy_to = args.outdir + "/" + infile_dir
    if os.path.isfile(directory_to_copy_to):
        print("Will not copy the directory into a file.")
        return 1
    if not os.path.exists(directory_to_copy_to):
        os.makedirs(directory_to_copy_to)
    shutil.copy(args.infile, directory_to_copy_to)
    return 0

if __name__ == "__main__":
    exit(main(sys.argv))
