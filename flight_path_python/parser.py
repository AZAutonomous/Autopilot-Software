'''
A - operation area
S - search area
W - waypoint


"MAV_AUTOPILOT"     3
"groundStation"     "QGroundControl"
"item"
        "autoContinue":     true
        "command":          22 - takeoff
                            16 - waypoint
                            30 - change altitude
                            178 - change speed
                            21 - land
        "frame":            3 - when takeoff, land and waypoint
                            2 - when change altitude and speed
        "id":               index of the mission item
        "param1":           15 when takeoff, 25 if land; 1 if other param are not 0, 0 otherwise
        "param2":           change in speed
        "param3":           change in throttle
        "type":             seem to always be "missionItem"
"plannedHomePosition"   homeposition, same format as "item"
                        frame, id, and all param are all 0

{
    "firmwareType": 3,
    "groundStation": "QGroundControl",
    "items": [ { } ],
    "plannedHomePosition": [
        37.803784,
        -122.462276,
        0
    ],
    "version": 2
}
{
    "MAV_AUTOPILOT": 12,
    "complexItems": [ { complex mission item }, { complex mission item } ],
    "groundStation": "QGroundControl",
    "items": [ { mission item }, { mission item } ],
    "plannedHomePosition": { mission item }, 
    "version": "1.0"
}
{
    "autoContinue": true,
    "command": 16,
    "coordinate": [ 47.633425920000001, -122.08828776999999, 25 ],
    "frame": 3,
    "id": 2,
    "param1": 0,
    "param2": 0,
    "param3": 0,
    "param4": 0,
    "type": "missionItem"
},

'''
import coordinate
import mission
import search_path
import math

path_info = {'turn_radius' : 1, \
        'path_cushine' : 5, \
        'boundary_cushine' : 5, \
        'pt_distance' : 10}

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
    li = line.split(',')
    coord = coordinate.GeoCoord(li[1], li[2])
    if(len(li) == 4):
        p = Point(li[0][0], li[0][1:], coord, li[3])
    else:
        p = Point(li[0][0], li[0][1:], coord)
    return p

def output_file(filename, item_list):
    f.open(filename)
    f.write('{ \
        \"MAV_AUTOPILOT\": 12, \
        \"complexItems\": [], \
        \"groundStation\": \"QGroundControl\", \
        ')
    fwrite('\"items: [\"')
    # mission item
    for item in item_list:
        f.write(item)
    fwrite('],')
    f.write('\"plannedHomePosition\": { mission item }, \
        \"version\": \"1.0\"}')
    f.close()

def main():
    # Assumed order: waypts, search
    filename = 'data.csv'
    #(op_area, search_area, waypoints_list) = read_file(filename)

    # given a takeoff point
    # generate path to go over waypts
    # generate search path within search_area
    pt1 = coordinate.GeoCoord(38.145278, -76.428889)
    pi = math.pi
    pt2 = search_path.coord_with_dist_from_pt(pt1, 10.0, pi / 4.0)
    print('----------')
    pt1 = coordinate.GeoCoord(38.145278, -76.428889, 20)
    pt2 = coordinate.GeoCoord(38.145278, -76.428850, 20)
    pt3 = coordinate.GeoCoord(38.145278, -76.428820, 20)
    pt4 = coordinate.GeoCoord(38.145278, -76.428800, 20)
    pt5 = coordinate.GeoCoord(38.145278, -76.428780, 20)
    test = mission.Mission()
    item1 = mission.MissionItem(pt1, 22)
    test.add_mission_item(item1)
    item2 = mission.MissionItem(pt2, 16)
    test.add_mission_item(item2)
    item3 = mission.MissionItem(pt3, 16)
    test.add_mission_item(item3)
    item4 = mission.MissionItem(pt4, 16)
    test.add_mission_item(item4)
    item5 = mission.MissionItem(pt5, 21)
    test.add_mission_item(item5)

    f = open('output.txt', 'w')
    f.write(str(test))
    f.close()






main()
