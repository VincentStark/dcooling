#!/usr/bin/python -tt

from copy import deepcopy
import numpy as np

class CoolingPipe:
  
  def  __init__(self):
    self.path = []
    self.numpaths = 0
    self.start_vertex = -1
    self.end_vertex = -1

    # Temporary
    self.paths = []

    # Get self.width and self.height
    wh = raw_input('Width and height? ')
    self.width, self.height = map(int, wh.split())
    if self.width not in range(2, 11) or self.height not in range(2, 11):
      raise ValueError('Width/height are out of range')

    # Get path scheme
    field_txt = []
    for line in range(1, self.height + 1):

      curline = raw_input('Line {0:2d}? '.format(line))
      values = map(int, curline.split())
      
      if len(values) != self.width:
        raise ValueError('You must enter exactly %s values' % self.width)

      for value in values:
        if value not in range(0, 4):
          raise ValueError('Scheme must consist of 0s to 3s only')

      field_txt.append(curline)

    # Check for correctness
    #print map(lambda x: map(lambda y: y.split().index(2), x.split()), self.prepath) 
    #print map(lambda x: map(lambda y: '0' in y.split(), x.split()), self.prepath) 
    #print any('0' in x.split() for x in self.prepath)
    
    if sum(map(lambda x: x.split().count('2'), field_txt)) != 1:
      raise ValueError('You must have only one intake')

    if sum(map(lambda x: x.split().count('3'), field_txt)) != 1:
      raise ValueError('You must have only one air conditioner')

    # Compose a full path as a multidimensional list
    self.field = map(lambda x: map(int, x.split()), field_txt)
    

  def calculate_path(self):

    # Build Graph
    f = self.field

    nodes = self.height*self.width
    g = np.zeros( (nodes, nodes), dtype=np.int16 )

    for x in range(self.height):
      for y in range(self.width):

        if f[x][y] != 1:

          current_vertex = x*self.width + y

          # Get start vertex
          if f[x][y] == 2:
            self.start_vertex = current_vertex

          # Get final vertex
          if f[x][y] == 3:
            self.end_vertex = current_vertex
          
          if (x+1 in range(self.height) and f[x+1][y] != 1):
            g[current_vertex][(x+1)*self.width+y] = 1
          if (x-1 in range(self.height) and f[x-1][y] != 1):
            g[current_vertex][(x-1)*self.width+y] = 1
          if (y+1 in range(self.width) and f[x][y+1] != 1):
            g[current_vertex][x*self.width + (y+1)] = 1
          if (y-1 in range(self.width) and f[x][y-1] != 1):
            g[current_vertex][x*self.width + (y-1)] = 1

    # Remove disconnected vertices
    # ...remove zero rows
    g = g[~np.all(g == 0, axis = 1)]
    # ...remove zero columns
    gmask = np.all(g == 0, axis = 0)
    self.graph = g[:, ~gmask]

    #print "Start: " + str(self.start_vertex)
    #print "End: " + str(self.end_vertex)
    print self.graph

    # Backtracing
    self.graph_len = len(self.graph)
    self.path.append(self.start_vertex)
    self.__check_path()

    #self.__draw_path()

    return self.path

  #@profile
  def __check_path(self):

    # Temporary
    #if self.numpaths > 11:
    #  return False

    # Check if we came to the desired position
    if self.path[-1] == self.end_vertex:
      # The graph is complete
      if len(self.path) == self.graph_len:
        return True
      # Optimization check for better speed
      # If we're at the final vertex, but unused vertices remain
      else:
        return False

    for v in range(self.graph_len):

      # Check if vertex is already added
      if v in self.path:
        continue

      # Find all neighbors
      if self.graph[ self.path[-1] ][v] == 1:
 
        self.path.append(v)
        if self.__check_path() == True:
          #self.paths.append(deepcopy(self.path))
          self.numpaths += 1
          print "Paths found so far: " + str(self.numpaths)

        # If it leads nowhere - remove it from stack
        self.path.pop()

    return False
      
#  def __draw_path(self):

#    for path in self.paths:
#      c = deepcopy(self.field)

#      for node in path:
#        x, y = node.coords
#        c[x][y] = 8
#        for line in c:
#          print line
#        print "==="

#      print "+++"


cp = CoolingPipe()

print "Paths: ", cp.calculate_path()
#for graph in cp.calculate_path():
#  print "(X, Y): " + str(graph.coords)
#  print "Neighbors: " + str(graph.neighbors)
#  print "==="
