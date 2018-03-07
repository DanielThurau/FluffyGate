from zipfile import ZipFile
import sys

password = sys.argv[1]
zipfile = sys.argv[2]

with ZipFile(zipfile) as zf:
	zf.extractall(pwd=password)