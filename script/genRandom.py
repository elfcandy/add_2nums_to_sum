#!/usr/bin/python
# coding: utf-8
import os
import sys
import getopt
import random
#import logging
import pdb

opts, args = getopt.getopt(sys.argv[1:], "n:")
#opts, args = getopt.getopt(sys.argv[1:], "n:p:")

dataNum = 0
for op, value in opts:
  if op == "-n":
    dataNum = int(value)
#  elif op == "-p":
#    filePath = value

def process():
	print('[MSG]: This program startup!')

	"""
	try:
		pid = os.fork()
		if pid > 0:
			logging.info('child pid=%d, kill parent progress' % pid)
			sys.exit(0)
	except OSError, e:
		logging.error(sys.stderr, "fork #1 failed:(%s)" % e.message)
		sys.exit(1)
	"""

#	logging.info('Generating random startup')
	print("[MSG]: Generating random startup")

	filename = sys.path[0] + '/random.txt'
	print('filename is "%s"' % filename)
#	fp = open(filename, 'w')
#	fp.write("The random data is:\n")
#	fp.close()
	try:
#		fp = open(filename, 'a')
		fp = open(filename, 'w')
		i = 0
		while i<dataNum :
			randomData = random.randint(0, dataNum*5)
			print(randomData)
			fp.write(str(randomData))
			fp.write("\n")
			i+=1
	finally:
		if fp:
			fp.close()

#	filename = 'random.txt'
#	with open(filename,'a') as f:
#    	f.write("I am Meringue.\n")
#	file_name.close()

#	logging.info('Generating random completed')
	print("[MSG]: Generating random completed")

if __name__ == '__main__':
    process()

