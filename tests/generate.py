#!/bin/python3

from random import uniform

def generate_file(lines_amount : int, path_to_file : str):
    print(lines_amount, path_to_file)
    pass


def __main__():
    path_to_file = input('Enter path to file: ')
    lines_amount = int(input('Enter lines amount: '))

    file = open(path_to_file, 'w')

    for _ in range(lines_amount):
        file.write(str(round((uniform(0, 100)), 6)) + ' ')
        file.write(str(round((uniform(0, 100)), 6)) + ' ')
        file.write(str(round((uniform(0, 100)), 6)) + '\n')

    file.flush()
    file.close()


__main__()

