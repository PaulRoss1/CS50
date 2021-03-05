from cs50 import SQL
import csv
from sys import argv, exit

db = SQL("sqlite:///students.db")

if len(argv) != 2:
    print("Usage: python roster.py House")
    exit(1)

a = db.execute("SELECT * FROM students WHERE house=? ORDER BY last, first", argv[1])

for x in range(len(a)):
    if a[x]['middle'] == None:
        print(f"{a[x]['first']} {a[x]['last']}, born {a[x]['birth']}")
    elif a[x]['middle'] != None:
        print(f"{a[x]['first']} {a[x]['middle']} {a[x]['last']}, born {a[x]['birth']}")