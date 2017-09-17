import sys
acc = 0
for l in sys.stdin:
    acc += l.count('F')
print(acc)


