import math as m
import sys
import copy
import coordinate

def generate_waypt_path(path_info, takeoff_pt, waypts_list, op_area):
    pts_list = []
    pts_list = copy.deepcopy(waypts_list)

    '''
    waypts_list_copy = list(waypts_list)
    pt = closest_point(takeoff_pt, waypts_list_copy)
    pt_c = copy.deepcopy(pt)
    pts_list.append(pt_c)
    waypts_list_copy.remove(pt)

    # Select the closest waypt
    while(True):
        if(not waypts_list_copy):
            break
        pt = closest_point(pts_list[-1], waypts_list_copy)
        print(pt.lat, pt.lon)
        pt_c = copy.deepcopy(pt)
        pts_list.append(pt_c)
        waypts_list_copy.remove(pt)
    '''

    return pts_list


def generate_search_path(path_info, start_pt, search_area, boundary_pts):
    '''
    :param path_info: the parameter needed to generate the path
    :type path_info: tuple of int
    :param start_pt: the starting point of the path, may or may not be
        inside the search area boundary
    :type start_pt: GeoCoord
    :param boundary_points: the boundary pts of the search area
    :type boundary_pts: list GeoCoord
    '''
    pts_list = []
    pts_list.append(start_pt)

    (closest_pt, index) = closest_point(start_pt, boundary_pts, True)

    # First Pt
    i = 1
    next_index = index + i if index + i < len(boundary_pts) else 0
    prev_index = index - i if index > i else len(boundary_pts) - 1
    angle = angle_of_pt(boundary_pts[index], boundary_pts[next_index])
    temp = coord_with_dist_from_pt(boundary_pts[index], path_info['boundary_cushine'], angle)
    angle = angle_of_pt(boundary_pts[index], boundary_pts[prev_index])
    waypt = coord_with_dist_from_pt(temp, path_info['boundary_cushine'], angle)
    pts_list.append(waypt)

    # Select second pt
    dist_prev_index = dist_between_coord(boundary_pts[index], boundary_pts[prev_index])
    dist_next_index = dist_between_coord(boundary_pts[index], boundary_pts[next_index])
    i = 1 if dist_next_index >= dist_prev_index else -1

    # Recalculate the path cushine
    (farthest_pt, index) = farthest_point(boundary_pts[index], boundary_pts, True)


    index = index + i if index + i < len(boundary_pts) else 0

    return pts_list


def closest_point(pt1, pt_list, with_index = False):
    '''
    Find a point in the pt_list that is closest to the pt1
    '''
    closest_pt = pt_list[0]
    min_dist = 0.0
    index = 0
    for index in range(len(pt_list)):
        pt = pt_list[index]
        dist = dist_between_coord(pt1, pt)
        if(min_dist > dist):
            min_dist = dist
            closest_pt = pt
    if(with_index == False):
        return closest_pt
    else:
        return (closest_pt, index)


def farthest_point(pt1, pt_list, with_index = False):
    '''
    Find a point in the pt_list that is farthest to the pt1
    '''
    farthest_pt = pt_list[0]
    max_dist = 0.0
    index = 0
    for index in range(len(pt_list)):
        pt = pt_list[index]
        dist = dist_between_coord(pt1, pt)
        if(max_dist > dist):
            max_dist = dist
            farthest_pt = pt
    if(with_index == False):
        return farthest_pt
    else:
        return (farthest_pt, index)


def waypt_selection(path, waypts_list):
    '''
    Select one waypoints and add to the path

    :param path:
    :type path: list of GeoCoord object
    :param waypts_list:
    :type waypts_list: list of GeoCoord object
    '''
    assert(len(path) > 1)
    center_lat = 0.0
    center_lon = 0.0
    for pt in waypts_list:
        center_lat += pt.lat
        center_lon += pt.lon
    center_lat /= len(waypts_list)
    center_lon /= len(waypts_list)
    sorted_list = list(waypts_list)
    for i in range(len(sorted_list)):
        for i in range(len(sorted_list)-1):
            pt1 = sorted_list[i]
            pt2 = sorted_list[i+1]
            dist_1 = m.sqrt((pt1.lat - center_lat) ** 2 + (pt1.lon - center_lon) ** 2)
            dist_2 = m.sqrt((pt2.lat - center_lat) ** 2 + (pt2.lon - center_lon) ** 2)
            if(dist_1 < dist_2):
                temp = copy.deepcopy(pt1)
                sorted_list[i] = pt2
                sorted_list[i+1] = temp


def angle_between_three_pts(pt1, pt2, pt3):
    '''
    :param pt1:
        :param pt2: the vertex of the resulting angle
    :param pt3:
        '''
    d_lat1 = pt1.lat - pt2.lat
    d_lon1 = pt1.lon - pt2.lon
    d_lat3 = pt3.lat - pt2.lat
    d_lon3 = pt3.lon - pt2.lon
    angle1 = m.atan2(d_lat1, d_lon1)
    angle3 = m.atan2(d_lat3, d_lon3)
    angle = abs(angle1 - angle3)
    angle = angle if angle < m.pi else angle - m.pi
    return angle

def angle_of_pt(center_pt, pt1):
    d_lat = center_pt.lat - pt1.lat
    d_lon = center_pt.lon - pt1.lon
    angle = m.atan2(d_lat, d_lon)
    angle = angle if angle < -2*m.pi else angle + 2*m.pi
    angle = angle if angle > 2*m.pi else angle - 2*m.pi
    return angle

def dist_between_coord(coord1, coord2):
    R = 6371*(10**3) # 6371 km, radius of Earth
    lat_1 = coord1.lat / 180.0 * m.pi
    lat_2 = coord2.lat / 180.0 * m.pi
    lon_1 = coord1.lon / 180.0 * m.pi
    lon_2 = coord2.lon / 180.0 * m.pi
    d_lat = lat_1 - lat_2
    d_lon = lon_1 - lon_2
    a = ( m.sin(d_lat / 2.0) ) ** 2 + m.cos(lat_1) * m.cos(lat_2) * ( m.sin(d_lon / 2.0) ) ** 2
    c = 2 * m.atan2(m.sqrt(a), m.sqrt(1.0 - a))
    return R * c


def coord_with_dist_from_pt(pt, dist, angle):
    '''
    Approximate the coordinate of the point that is some distance
        from the original point,
    :param pt:
        :param dist:
            :param angle: use the pt as the center of the coordinate system,
        the angle between the line connect the resulting point and
        the origin and the postive X axis
    '''
    def approximate(pt, pt_guess, dist_target):
        new_lat = pt_guess.lat
        new_lon = pt_guess.lon
        while(True):
            dist = dist_between_coord(pt, pt_guess)
            error = (dist_target - dist) / dist
            if(abs(error) < 0.00001):
                return coordinate.GeoCoord(new_lat, new_lon, pt_guess.alt)
            else:
                d_lat = pt_guess.lat - pt.lat
                d_lon = pt_guess.lon - pt.lon
                new_lat = (1 + error) * d_lat + pt.lat
                new_lon = (1 + error) * d_lon + pt.lon
                pt_guess = coordinate.GeoCoord(new_lat, new_lon, pt_guess.alt)
        return pt_guess


    # Assume in the same hemisphere
    assert(angle <= 2.0 * m.pi and angle >= -2.0 * m.pi)
    one_m = 0.000009
    one_m = 0.000011444
    one_m = 0.000010222
    if(angle <= m.pi / 2.0 and angle >= m.pi / -2.0):
        initial_lat = pt.lat + m.sin(angle) * dist * one_m
        initial_lon = pt.lon + m.cos(angle) * dist * one_m
    elif((angle >= m.pi / 2.0 and angle <= 2.0 * m.pi) \
            or (angle <= m.pi / -2.0 and angle >= -2.0 * m.pi)):
        initial_lat = pt.lat + m.sin(angle) * dist * one_m
        initial_lon = pt.lon - m.cos(angle) * dist * one_m
    else:
        raise ValueError
    inital_guess_pt = coordinate.GeoCoord(initial_lat, initial_lon, pt.alt)
    return approximate(pt, inital_guess_pt, dist)




