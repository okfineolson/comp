from mrjob.job import MRJob
import sys
import json
import re
import math
from mrjob.compat import jobconf_from_env
dic = {}
class weightndex(MRJob):
	def mapper(self, _, line):
		line = line.lower()
		words = line.split(' ')
		key = words[0]
		values = words[1:]
		for i in values:
			if(i in dic ):
				if (key in dic[i]):
					dic[i][key] += 1
				else:
					dic[i][key] = 1
			else:
				dic[i] = {}
				dic[i][key] = 1
		yield jobconf_from_env('myjob.settings.docnumber'), 1
	def reducer_init(self):
		self.count = 0;
	def reducer(self, path, ones):
		self.count = sum(ones)
		for key in sorted(dic.keys()):
				for key1,value1 in dic[key].items():
					N = self.count
					ni = len(dic[key])
					tfij = value1
					wij = tfij*math.log10(N/ni)
					yield key,key1+","+str(wij)
		
if __name__ == '__main__':
    weightndex.run()