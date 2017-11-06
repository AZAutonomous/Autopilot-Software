import pygame

f = open('points.txt', 'r')

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
BLUE = (0, 0, 255)

size = (700, 500)
screen = pygame.display.set_mode(size)
pygame.display.set_caption("Test")

done = False

clock = pygame.time.Clock()

#Clear the screen
screen.fill(WHITE)

#Read file
for line in f:
		data_list.append(line)

f.close()

#Update the screen
pygame.display.flip()

#Run until user exits
while not done:
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			done = True
	#60 FPS Limit
	clock.tick(60)

pygame.quit()