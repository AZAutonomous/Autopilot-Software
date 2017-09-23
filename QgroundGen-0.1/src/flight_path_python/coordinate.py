import sys

class GeoCoord:
    '''
    :attr lat_dir: North or South
    :attr lat_DD: Degree
    :attr lat_MM:
    :attr lat_SS:
    :attr lon_dir: West or East
    :attr lon_DD: Degree
    :attr lon_MM:
    :attr lon_SS:
    '''
    def __init__(self, lat_in, lon_in, alt = -1):
        if(type(lat_in) is str):
            try:
                li = lat_in.split('-')
                lat_dir = li[0][0]
                lat_DD = float(li[0][1:])
                lat_MM = float(li[1])
                lat_SS = float(li[2])
                self.lat = lat_DD + lat_MM/60.0 + lat_SS/3600.0
                if(lat_dir == 'N'):
                    self.lat
                elif(lat_dir == 'S'):
                    self.lat = -1.0 * self.lat
                else:
                    raise ValueError

                li = lon_in.split('-')
                lon_dir = li[0][0]
                lon_DD = float(li[0][1:])
                lon_MM = float(li[1])
                lon_SS = float(li[2])
                self.lon = lon_DD + lon_MM/60.0 + lon_SS/3600.0
                if(lon_dir == 'E'):
                    self.lon
                elif(lon_dir == 'W'):
                    self.lon = -1.0 * self.lon 
                else:
                    raise ValueError

                self.alt = float(alt) * 0.3048
            except ValueError:
                print('Incorrect Format')
                sys.exit()
        elif(type(lat_in) is float):
            try:
                self.lat = lat_in
                self.lon = lon_in
                self.alt = float(alt)
            except ValueError:
                print('Incorrect Format')
                sys.exit()

