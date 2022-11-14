#!/bin/python3


def __main__():
    path_to_file = input('Enter path to file: ')
    file = open(path_to_file, 'r')

    total_sum = 0

    while True:
        line = file.readline()
        total_sum += sum(map(float, line.split()))
        if not line:
            break

    file.close()
    print(total_sum)
        

__main__()

