#How to run
#python3 memGenerator.py number_of_memory_addresses, consecutive_memory_addresses, seed_value
#
#

import random, sys
def cacheGen(fileSize, consecutive, seedValue):
	outPutText=open('out.txt', 'w');

	bases = fileSize//consecutive; # gets the total number of random addresses, each addresses will have a specifed number of consecutive addresses called.
	leftOver = fileSize % consecutive;
	if(leftOver): bases+=1;
	random.seed(seedValue);
	baseValues = [];
	numValues =0;

	for i in range(bases):
		baseValues.append( '0x' + '%08x' % random.randrange(16**8));

	#Creates all random memory addresses and writes it to the file
	for i in range(bases):
		nextAddress = baseValues[i];
		for j in range(consecutive):
			nextAddress = '0x' + '%08x' %(int(nextAddress, 16) + 4);
			if(int(nextAddress,16)>= int('ffffffff',16)):
				nextAddress = '0x00000000'
			numValues += 1;
			if(numValues > fileSize):
				break;
			outPutText.write(nextAddress+'\n');


def main():
	fileLines = int(sys.argv[1]);
	seed = sys.argv[3];
	consecutive = int(sys.argv[2]);
	cacheGen(fileLines, consecutive, seed);


if __name__ == '__main__':
	main();
