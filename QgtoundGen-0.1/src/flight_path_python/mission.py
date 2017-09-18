'''
mission.py
'''

class Mission:
    def __init__(self):
        self.mission_item_list = []
        self.MAV_AUTOPILOT = 12
        self.firmwareType = 3
        self.groundStation = 'QGroundControl'
        self.plannedHomePosition = None

        # Initialize the takeoff
        self.takeoff_coord = 1
        # Initialize the landing
        self.land_coord = 1

    def gen_search_path(self, search_area_boundry):
        print()

    def add_mission_item(self, item):
        self.mission_item_list.append(item)

    def __str__(self):
        #string = '{\"MAV_AUTOPILOT\": 12,\"complexItems\": [ { complex mission item }, { complex mission item } ],"groundStation": "QGroundControl","items": [ { mission item }, { mission item } ],"plannedHomePosition": { mission item }, "version": "1.0"}'
        string = '{ \"firmwareType\": 3, \"groundStation\": \"QGroundControl\", '
        string += '\"items\": [ '
        for i in range(len(self.mission_item_list)):
            self.mission_item_list[i].set_id(i+1) # set id
            string += str(self.mission_item_list[i])
            if(i < len(self.mission_item_list) - 1):
                string += ', '
        string += ' ], '
        # Use the coordinate of the First Mission Item as the Home Position
        takeoff_coord = self.mission_item_list[0].coord
        string += '\"plannedHomePosition\": [{}, {}, {} ], '.format( \
            takeoff_coord.lat, takeoff_coord.lon, takeoff_coord.alt)
        string += '"version": 2}'
        return string


class MissionItem:
    def __init__(self, coord, command, param = 0):
        try:
            self.auto_conti = True
            self.command = int(command)
            self.coord = coord
            self.frame = 3
            self.id = 0
            self.param = [0, 0, 0, 0]
            self.param1 = 0
            self.param2 = 0
            self.param3 = 0
            self.param4 = 0
            self.type = 'SimpleItem'
        except ValueError:
            print('Incorrect parameter format to construct a mission item')
            sys.exit()

    def set_id(self, Id):
        self.id = Id

    def set_command(self, command):
        self.command = command
        if(command == 22):
            self.param = [15, 0, 0, 0]
        elif(command == 16):
            self.param = [0, 0, 0, 0]
        elif(command == 30):
            self.param = [1, 0, 0, 0]
        elif(command == 21):
            self.param = [10, 0, 0, 0]

    def set_takeoff(self):
        self.command = 22
        self.param = [15, 0, 0, 0]

    def set_land(self):
        self.command = 21
        self.param = [10, 0, 0, 0]

    def __str__(self):
        string = '{ '
        string += '\"autoContinue\": {}, '.format( \
            'true' if self.auto_conti else 'false')
        string += '\"command\": {}, '.format(self.command)
        string += '\"coordinate\": [ {0:.8f}, {1:.8f}, {2:d} ], '.format( \
            self.coord.lat, self.coord.lon, int(self.coord.alt))
        string += '\"doJumpId\": {}, '.format(self.id)
        string += '\"frame\": {}, '.format(self.frame)
        string += '\"params\": [ {}, {}, {}, {} ], '.format(self.param[0], self.param[1], self.param[2], self.param[3])
        string += '\"type\": \"{}\" '.format(self.type)
        string += '}'
        return string


class Fence:
	def __init__(self):
		self.pt_list = []
		self.ACTION = 0
		self.MINALT = 0
		self.MAXALT = 0
		self.RETALT = 0
		self.AUTOENABLE = 0
		self.RET_RALLY = 0

	def add_boundary_point(self, pt):
		self.pt_list.append(pt)

	def __str__(self):
		string = '{'
		string += '\"breachReturn\": [ 38.143678703642927, -76.429535377473599 ],'
		string += '\"fileType\": \"GeoFence\", '
		string += '\"groundStation\": \"QGroundControl\",'
		string += '\"parameters\": ['
		string += '{ ' + '\"compId\": 1, \"name\": \"FENCE_ACTION\", \"value\": {} '.format(self.ACTION) + '},'
		string += '{ ' + '\"compId\": 1, \"name\": \"FENCE_MINALT\", \"value\": {} '.format(self.MINALT) + '},'
		string += '{ ' + '\"compId\": 1, \"name\": \"FENCE_MAXALT\", \"value\": {} '.format(self.MAXALT) + '},'
		string += '{ ' + '\"compId\": 1, \"name\": \"FENCE_RETALT\", \"value\": {} '.format(self.RETALT) + '},'
		string += '{ ' + '\"compId\": 1, \"name\": \"FENCE_AUTOENABLE\", \"value\": {} '.format(self.AUTOENABLE) + '},'
		string += '{ ' + '\"compId\": 1, \"name\": \"FENCE_RET_RALLY\", \"value\": {} '.format(self.RET_RALLY) + '}'
		string += '],'
		string += '\"polygon\": ['
		for i in range(len(self.pt_list)):
			string += '[ {}, {} ]'.format(self.pt_list[i].lat, self.pt_list[i].lon)
			if(i < len(self.pt_list) - 1):
				string += ','
		string += '],'
		string += '\"version\": 1'
		string += '}'
		return string

'''
{
    "breachReturn": [
        38.143678703642927,
        -76.429535377473599
    ],
    "fileType": "GeoFence",
    "groundStation": "QGroundControl",
    "parameters": [
        {
            "compId": 1,
            "name": "FENCE_ACTION",
            "value": 0
        },
        {
            "compId": 1,
            "name": "FENCE_MINALT",
            "value": 0
        },
        {
            "compId": 1,
            "name": "FENCE_MAXALT",
            "value": 0
        },
        {
            "compId": 1,
            "name": "FENCE_RETALT",
            "value": 0
        },
        {
            "compId": 1,
            "name": "FENCE_AUTOENABLE",
            "value": 0
        },
        {
            "compId": 1,
            "name": "FENCE_RET_RALLY",
            "value": 0
        }
    ],
    "polygon": [
        [
            38.144210286496069,
            -76.429481732815574
        ],
        [
            38.143147117691356,
            -76.43034003970034
        ],
        [
            38.143163993507656,
            -76.428730714291405
        ]
    ],
    "version": 1
}

'''


