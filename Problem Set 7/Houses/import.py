from cs50 import SQL
import csv
from sys import argv, exit

db = SQL("sqlite:///students.db")

if len(argv) != 2:
    print("Usage: python import.py characters.csv")
    exit(1)

with open(argv[1]) as characters:
    characters = csv.reader(characters, delimiter=',', quotechar=',')
    for row in characters:
        a = row[0].split()
        if a[0] != 'name':
            if len(a) == 2:
                db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                           a[0], None, a[1], row[1], row[2])
            if len(a) == 3:
                db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                           a[0], a[1], a[2], row[1], row[2])