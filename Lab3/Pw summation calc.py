import math

A = [2, 3, 4, 5, 6, 7, 8, 9, 9.8, 10.5]
output = 0
for y in A:
    for x in range(10):
        output = output + pow(y,x)/math.factorial(x)

    print(output)
