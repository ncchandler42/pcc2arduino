import pygame

pygame.init()
pygame.joystick.init()

js = pygame.joystick.Joystick(0)
js.init()

while True:
    js.get_axis(0)
    print(js)
        
pygame.joystick.quit()
pygame.quit()
