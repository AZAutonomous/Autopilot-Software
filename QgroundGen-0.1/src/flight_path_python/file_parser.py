'''
parser.py
'''
import coordinate
import mission
import path
import math

def read_file(filename):
    f = open(filename)
    op_area = []
    search_area = []
    waypoints_list = []
    for line in f:
        p = parse_line(line)
        if(line[0] == 'A'):
            op_area.append(p)
        elif(line[0] == 'S'):
            search_area.append(p)
        elif(line[0] == 'W'):
            waypoints_list.append(p)
    f.close()
    return (op_area, search_area, waypoints_list)


def parse_line(line):
    '''
    :return a GeoCoord object parsed from the line
    :rtype GeoCoord
    '''
    li = line.split(',')
    if(len(li) == 4):
        return coordinate.GeoCoord(li[1], li[2], li[3])
    elif(len(li) == 3):
        return coordinate.GeoCoord(li[1], li[2])
    else:
        return None