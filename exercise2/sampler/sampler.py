# imports
import sys

# definitions
FREQUENCY_FILE = "freq.csv"

#
# Stateless function that will calculate time in ms for given
# note length.
#
def getDuration(length, bpm):
	
	# making variables floats
	bps = float(bpm) / 60.0
	length = float(length)
	
	# one fourth should last one beat
	num_beats = 4.0 / length
	
	# scale to ms, return
	return 1000 * (num_beats / bps)

#
# Will generate and return a frequency dictionary
#
def getFrequencies(frequency_file):
	freq = {}
	for line in open(frequency_file):
		split = line.strip().split(",")
		key = split[0].strip().split("/")[0]
		key = key.strip(" ")
		freq[key] = float(split[1])
	return freq

def main():	
	# getting frequencies
	freq = getFrequencies(FREQUENCY_FILE)
	
	# header information	
	track_name = raw_input().split("\t")[1]
	variable_name = raw_input().split("\t")[1]
	length_variable_name = raw_input().split("\t")[1]
	track = int(raw_input().split("\t")[1])
	bpm = float(raw_input().split("\t")[1])
	
	# track name
	print("/*")
	print(" * %s" % (track_name))
	print(" */")
	print("")
	
	# index counter
	i = 0

	# iterating notes
	for line in sys.stdin:

		# line split
		sp = line.strip().split("\t")

		# note key
		note = sp[0]
		
		# note length
		length = int(sp[1])
	
		# calculate duration in milliseconds
		ms = getDuration(length, bpm)
	
		# get frequency
		if note == "0":
			f = 0.0
		else:
			f = freq[note]
	
		# add 0.9 of the duration with given frequency
		print("/* note %s, length %i */" % (note, length))
		print("%s[%i][%i].hz = %f;" % (variable_name, track, i, f))
		print("%s[%i][%i].ms = %f;" % (variable_name, track, i, 0.9*ms))
		print("")
		i += 1

		# add 0.1 of the duration with 0 frequency (pause)
		print("/* pause */")
		print("%s[%i][%i].hz = %f;" % (variable_name, track, i, 0))
		print("%s[%i][%i].ms = %f;" % (variable_name, track, i, 0.1*ms))
		print("")
		i += 1

	# add length	
	print("%s[%i] = %i;" % (length_variable_name, track, i+1))

if __name__ == "__main__":
    main()
