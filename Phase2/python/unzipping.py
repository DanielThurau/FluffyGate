from zipfile import ZipFile

with ZipFile('test.zip') as zf:
    zf.extractall(pwd='fluffy')