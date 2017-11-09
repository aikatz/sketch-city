
# Milestone 3: Maze Navigation Algorithm
The goal is to write a maze navigation algorithm that works in simulation and in real life. After the algorithm finishes exploring the maze, an indicator must show that the robot has explored everything explorable, in both simulation and real life. 


### Simulation
We have created a simulation with python. 
```Python
# coding=UTF-8
from __future__ import print_function
import curses
import time
import locale

locale.setlocale(locale.LC_ALL,"")

maze_str = \
"""11111
10001
11101
10001
10101
10101
11111"""

# Turn maze_str into 2d list
maze = [[True if c == "1" else False for c in row] for row in maze_str.split("\n")]

def maze_simulation(screen, start_x=1, start_y=1):
  # Set up curses stuff 
  screen.clear()
  curses.curs_set(0)
  height, width = screen.getmaxyx()
  curses.init_pair(1, curses.COLOR_WHITE, curses.COLOR_BLACK)
  curses.init_pair(2, curses.COLOR_GREEN, curses.COLOR_BLACK)
  curses.init_pair(3, curses.COLOR_RED, curses.COLOR_BLACK)
  curses.init_pair(4, curses.COLOR_BLUE, curses.COLOR_BLACK)

  # Simple wrapper for drawing a wall
  def drawWall(wall_pos):
    screen.addstr(wall_pos[0], wall_pos[1], u"\u2588".encode("utf-8"), curses.color_pair(1))

  # Draw maze initially
  for y in xrange(len(maze)):
    for x in xrange(len(maze[0])):
      screen.addstr(y, x, "?", curses.color_pair(1))
  screen.refresh()

  # A set of all the visited positions
  visited = set()

  # List of tuples corresponding to positions to move to
  move_stack = [(start_y, start_x)]

  # A stack that contains the movement history_stack of the robot
  history_stack = []

  # A stack that contains the position of intersections
  intersect_stack = []

  # The position of the robot, initalized to the upper left
  pos = (start_y, start_x)

  # A flag used to determine whether to backtrack to prev intersection
  backtrack = False

  while move_stack:
    if backtrack:
      # We need to backtrack, so select appropriate range of history 
      # and then add it onto the move stack
      history_stack.pop()
      tmp = history_stack.pop()
      back = intersect_stack.pop()
      tmp_list = []
      while(tmp != back):
        tmp_list.append(tmp)
        tmp = history_stack.pop()
      history_stack.append(tmp)
      move_stack += list(reversed(tmp_list))
      backtrack = False

    else:
      pos = move_stack.pop()
      history_stack.append(pos)

    # Draw robot position
    screen.addstr(pos[0], pos[1], u"\u2588".encode("utf-8"), curses.color_pair(3))
    if pos not in visited:
      visited.add(pos)
      stack_size = len(move_stack)

      # Check "North"
      if(not maze[pos[0]-1][pos[1]] and (pos[0]-1,pos[1]) not in visited):
        move_stack.append((pos[0]-1, pos[1]))
      elif((pos[0]-1,pos[1]) not in visited):
        drawWall((pos[0]-1, pos[1]));

      # Check "West"
      if(not maze[pos[0]][pos[1]-1] and (pos[0],pos[1]-1) not in visited):
        move_stack.append((pos[0], pos[1]-1))
      elif((pos[0],pos[1]-1) not in visited):
        drawWall((pos[0], pos[1]-1));

      # Check "South"
      if(not maze[pos[0]+1][pos[1]] and (pos[0]+1,pos[1]) not in visited):
        move_stack.append((pos[0]+1, pos[1]))
      elif((pos[0]+1,pos[1]) not in visited):
        drawWall((pos[0]+1, pos[1]));

      # Check "East"
      if(not maze[pos[0]][pos[1]+1] and (pos[0],pos[1]+1) not in visited):
        move_stack.append((pos[0], pos[1]+1))
      elif((pos[0],pos[1]+1) not in visited):
        drawWall((pos[0], pos[1]+1));

      # figure out if we're at intersection or dead-end
      if(len(move_stack) - stack_size == 0):
        backtrack = True
      if(len(move_stack) - stack_size > 1):
        intersect_stack.append(pos)

    # Update the screen, and wait for user to press a key
    screen.refresh()
    c = screen.getch()

    # Change position to path
    screen.addstr(pos[0], pos[1], u"\u2588".encode("utf-8"), curses.color_pair(2))

  # Set color of robot to blue since we're done
  screen.addstr(pos[0], pos[1], u"\u2588".encode("utf-8"), curses.color_pair(4))
  screen.refresh()

  # Maze is explored
  while True:
    continue

curses.wrapper(maze_simulation)
```
Unexplored territories are represented by question marks. Walls are represented by white squares. Explored areas are represented by green squares. The robot, or the current position of the robot is represented by a red square. After the algorithm is finished, the current position of the robot turns blue to indicate that all explorable areas are explored

<div style="text-align: center">
<iframe width="534" height="300" src="https://youtu.be/DidsbgD-tjI" frameborder="0" allowfullscreen></iframe>
</div>

### Real Life
