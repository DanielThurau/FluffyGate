from __future__ import print_function

true_sort = {}
with open("all_files","r") as f:
    for line in f:
        k = int(filter(str.isdigit, line))
        true_sort[k] = line


for k in sorted(true_sort):
    print(true_sort[k], end="")