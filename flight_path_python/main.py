import sys
import coordinate
import mission
import path
import file_parser

path_info = {'turn_radius' : 5, \
        'path_cushine' : 20, \
        'boundary_cushine' : 20, \
        'pt_distance' : 10}

def main():
    # Read the File
    filename = 'data.csv'
    (op_area, search_area, waypoints_list) = file_parser.read_file(filename)

    m1 = mission.Mission()
    f1 = mission.Fence()

    # Given a takeoff point
    takeoff_pt = coordinate.GeoCoord(38.145495, -76.428166, 200)
    item1 = mission.MissionItem(takeoff_pt, 22)
    m1.add_mission_item(item1)

    # Generate path to go over waypts
    waypt_path = path.generate_waypt_path(path_info, takeoff_pt, waypoints_list, op_area)
    for pt in waypt_path:
        item1 = mission.MissionItem(pt, 16)
        m1.add_mission_item(item1)

    # Generate search path within search_area
    search_area_path = []
    #search_area_path = path.generate_search_path(path_info, waypt_path[-1], search_area, op_area, 30)
    search_area_path = path.generate_search_path_spiral(path_info, waypt_path[-1], search_area, op_area, 30)
    for pt in search_area_path:
        item1 = mission.MissionItem(pt, 16)
        m1.add_mission_item(item1)

    # Add Landing point, assume to be takeoff point as well
    item1 = mission.MissionItem(takeoff_pt, 21)
    m1.add_mission_item(item1)

    # Generate Fence from the operational area
    for pt in op_area:
        f1.add_boundary_point(pt)

    # Output the mission into file
    f = open('output.mission', 'w')
    f.write(str(m1))
    f.close()

    # Output the fence into file
    f = open('output.fence', 'w')
    f.write(str(f1))
    f.close()




main()
