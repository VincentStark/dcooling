#!/usr/bin/env python

import sys

def subset(S, count):

  temp = []
  for i, e in enumerate(S):
    #print "count: " + str(count) + ", i: " + str(i) + ", offset: " + str(count>>i) + ", e: " + str(e)
    if count>>i & 1 == 0:
      #print "append!"
      temp.append(e)

  print temp

  count += 1

  if count% (1<<len(S)) == 0:
    return

  subset(S, count)

sys.setrecursionlimit(10000000)
test = [x for x in range(0, 5)]
subset(test, 0)
