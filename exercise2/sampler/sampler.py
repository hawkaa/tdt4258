import sys;

FREQUENCY_FILE = "freq.csv"

freq = {};
for line in open(FREQUENCY_FILE):
	split = line.strip().split(",");
	freq[split[0]] = float(split[1]);


def getDuration(length, bpm):
	bps = bpm/60.0;
	return 1000 * (bps / float(length))


variable_name = input().split("\t")[1];
track = int(input().split("\t")[1]);
bpm = float(input().split("\t")[1]);


i = 0;
for line in sys.stdin:
	sp = line.strip().split("\t");
	note = sp[0];
	length = int(sp[1]);

	ms = getDuration(length, bpm);
	
	print("/* note %s, length %i */" % (note, length));
	print("%s[%i][%i].hz = %f;" % (variable_name, track, i, freq[note]))
	print("%s[%i][%i].ms = %f;" % (variable_name, track, i, 0.9*ms))
	print();
	i += 1;
	print("/* pause */")
	print("%s[%i][%i].hz = %f;" % (variable_name, track, i, 0))
	print("%s[%i][%i].ms = %f;" % (variable_name, track, i, 0.1*ms))
	print();


