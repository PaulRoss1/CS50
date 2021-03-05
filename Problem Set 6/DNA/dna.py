import csv
from sys import argv, exit

# stop program if exactly two arguments arn't given
if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)

# open the second argument (sequence) and save in variable seqs
with open(argv[2]) as seq:
    seqs = seq.read()

var = []

# open first argument(csv file) and save all the STRs in the list 'var'
with open(argv[1], newline='') as data:
    datas = csv.reader(data, delimiter=' ', quotechar='|')
    for row in datas:
        for x in range(1, len(row[0].split(','))):
            var.append(row[0].split(',')[x])
        break

base = []
counter = 0

while True:
    # make a list 'o_l' - 'o' will represent the STR i'm looking for, 'l' will represent all other letters
    o_l = []

    for x in seqs.replace(var[counter], 'o'):
        if x == 'o':
            o_l.append(x)
        else:
            o_l.append('l')

# 's' will be the index, growing by one as I loop through the list 'o_l'
# 't' will be a counter, increasing by one every time an STR(o) is found
# t is set to 0 once a non STR value (l) appears, and also is added to the list res
# the list res will have the number number of repeated STR's
# loop is set to one, once I reach the end of the list o_l, loop is set to 0 and the while loops exits

    s = 0
    t = 0
    res = []
    loop = 1

    while loop == 1:
        if o_l[s] == 'o':
            t += 1
            s += 1

        else:
            res.append(t)
            t = 0
            s += 1

        if s == len(o_l):
            res.append(t)
            loop = 0

# base will append the maximum number of recuring STR's in res
# counter will keep increasing and when it is bigger than lengh of var, the loop will exit

    base.append(max(res))
    counter += 1
    if counter > len(var)-1:
        break

# ran is a string of all the maximum occurances of each STR (string version of base)

ran = ''

for x in base:
    ran += str(x)+','

ran = ran.rstrip(',')

# open the first argument(csv file) and check if ran is in one of the rows, if it is, print the name on the row and exit, else print no match

with open(argv[1], newline='') as data:
    datas = csv.reader(data, delimiter=' ', quotechar='|')
    for row in datas:
        if ran in (', '.join(row)):
            print(row[0].split(',')[0])
            exit(0)
    print('No match')

