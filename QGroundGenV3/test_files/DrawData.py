import pygame

f = open('D:/Coding/AZAutonomous/Autopilot-Software/QGroundGenV3/test_files/points.txt', 'r')

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
BLUE = (0, 0, 255)

size = (800, 800)
screen = pygame.display.set_mode(size)
pygame.display.set_caption("Test")

done = False

clock = pygame.time.Clock()

#Clear the screen
screen.fill(WHITE)

#Read file
data_list = [] #Holds list of longitude, latitude, and point type for every point
point_data = [] #Holds the data of each point
for line in f:
	new_line = line.rstrip('\n')
	point_data = new_line.split(" ")
	data_list.append(point_data)
f.close()

#Manipulate data to work with screen drawing
#Convert string values to float
for i in data_list:
	i[1] = float(i[1])
	i[2] = float(i[2])
	if i[0] == "O":
		i[4] = float(i[4])
#Find minimum x value and y value, then shift all points
min_x = data_list[0][1]
min_y = data_list[0][2]
for i in data_list:
	if i[1] < min_x:
		min_x = i[1]
	if i[2] < min_y:
		min_y = i[2]

for i in data_list:
	i[1] = i[1] - min_x
	i[2] = i[2] - min_y

#Scale the data up
for i in data_list:
	i[1] = round(50000 * i[1])
	i[2] = round(50000 * i[2])
	
#Split lists into separate waypoint, search, op area, and obstacle lists
#Removes unnecessary data such as altitude and the designator after they are separated
waypoints = []
search_corners = []
op_corners = []
obstacles = []
for a in data_list:
	if a[0] == "W":
		del a[0]
		del a[-1]
		waypoints.append(a)
	elif a[0] == "S":
		del a[0]
		del a[-1]
		search_corners.append(a)
	elif a[0] == "B":
		del a[0]
		del a[-1]
		op_corners.append(a)
	elif a[0] == "O":
		del a[0]
		del a[-2]
		obstacles.append(a)

for a in waypoints:
	a = tuple(a)
for a in search_corners:
	a = tuple(a)
for a in op_corners:
	a = tuple(a)

#Draw on screen
pygame.draw.lines(screen, GREEN, False, waypoints, 1)
pygame.draw.lines(screen, BLUE, True, search_corners, 1)
pygame.draw.lines(screen, BLACK, True, op_corners, 1)
for o in obstacles:
	position = (o[0], o[1])
	radius = round(50000 * o[2])
	pygame.draw.circle(screen, RED, position, radius, 1)

#Update the screen
temp_screen = pygame.transform.flip(screen, False, True)
screen.blit(temp_screen, (0, 0))
pygame.display.flip()

#Run until user exits
while not done:
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			done = True
	#60 FPS Limit
	clock.tick(60)

pygame.quit()