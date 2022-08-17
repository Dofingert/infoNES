import pygame
import serial
tty = serial.Serial(port='/dev/ttyUSB0', baudrate='230400')
tty.reset_input_buffer()
inp = tty
outp = tty

pygame.init()

key_state = int(0)

def get_key_id(event):
  if event.key == pygame.K_UP:
    return 4
  if event.key == pygame.K_DOWN:
    return 5
  if event.key == pygame.K_LEFT:
    return 6
  if event.key == pygame.K_RIGHT:
    return 7
  if event.key == pygame.K_z:
    return 1
  if event.key == pygame.K_x:
    return 0
  if event.key == pygame.K_a:
    return 2
  if event.key == pygame.K_s:
    return 3
  return -1

import struct
screen =  pygame.display.set_mode((640,480))
clock = pygame.time.Clock()
while 1:
  clock.tick(30)
  cnt = 0
  for event in pygame.event.get():
    cnt = 1
    if event.type == pygame.KEYDOWN:
      k_id = get_key_id(event)
      if(k_id != -1):
        print(chr(ord('a') + k_id - 1),end='')
        key_state |= (1 << k_id)
    elif event.type == pygame.KEYUP:
      k_id = get_key_id(event)
      if(k_id != -1):
        print(chr(ord('A') + k_id - 1),end='')
        key_state &= ~(1 << k_id)
  if(cnt == 1):
    l = ((64 + (key_state & 0x0f))) | ((64 + ((key_state & 0xf0) >> 4)) << 8)
    outp.write(struct.pack('<I', l))
    outp.write(b'\n')