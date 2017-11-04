# coding=UTF-8
from __future__ import print_function
import curses
import time

maze_str = \
"""#####################
#     #             #
# # ### ##### #######
# # #   #     #     #
### # ### ##### ### #
#   # # #     # #   #
# ### # ##### # # ###
# #   #   # #   #   #
# # ### # # ####### #
#       #     #     #
# ############# ### #
#   #     #     # # #
### # ### # ##### # #
# # #   # #   #     #
# # ### # # # ##### #
# # #   # # #     # #
# # ##### # ##### # #
# # # #   # #     # #
# # # # ##### ##### #
#     #       #     #
#####################"""

# Turn maze_str into 2d list
maze = [[c for c in row] for row in maze_str.split("\n")]

def maze_simulation(screen, start_x=1, start_y=1):
  # Set up curses stuff 
  screen.clear()
  height, width = screen.getmaxyx()

  # Draw maze initially
  for y in xrange(len(maze)):
    for x in xrange(len(maze[0])):
      screen.addstr(y, x, maze[y][x])
  screen.refresh()

  visited, stack = set(), [(start_y, start_x)]
  
  while stack:
    pos = stack.pop()
    screen.addstr(pos[0], pos[1], ".")
    if pos not in visited:
      visited.add(pos)
      if((pos[0]-1, pos[1]) not in visited and maze[pos[0]-1][pos[1]] == " "):
        stack.append((pos[0]-1, pos[1]))
      if((pos[0], pos[1]-1) not in visited and maze[pos[0]][pos[1]-1] == " "):
        stack.append((pos[0], pos[1]-1))
      if((pos[0]+1, pos[1]) not in visited and maze[pos[0]+1][pos[1]] == " "):
        stack.append((pos[0]+1, pos[1]))
      if((pos[0], pos[1]+1) not in visited and maze[pos[0]][pos[1]+1] == " "):
        stack.append((pos[0], pos[1]+1))
    screen.refresh()
    time.sleep(.5)
  
  while True:
    continue

curses.wrapper(maze_simulation)
