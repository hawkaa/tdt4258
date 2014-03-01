import sys;

FREQUENCY_FILE = "freq.csv"

freq = {};
for line in open(FREQUENCY_FILE):
	split = line.strip().split(",");
	key = split[0].strip().split("/")[0];
	key = key.strip(" ");
	freq[key] = float(split[1]);


def getDuration(length, bpm):
	bps = bpm/60.0;
	return 1000 * (bps / float(length))


track_name = raw_input().split("\t")[1];
variable_name = raw_input().split("\t")[1];
length_variable_name = raw_input().split("\t")[1];
track = int(raw_input().split("\t")[1]);
bpm = float(raw_input().split("\t")[1]);


print("/*")
print(" * %s" % (track_name));
print(" */")
print("");

i = 0;
for line in sys.stdin:
	sp = line.strip().split("\t");
	note = sp[0];
	length = int(sp[1]);

	ms = getDuration(length, bpm);
	
	print("/* note %s, length %i */" % (note, length));
	print("%s[%i][%i].hz = %f;" % (variable_name, track, i, freq[note]))
	print("%s[%i][%i].ms = %f;" % (variable_name, track, i, 0.7*ms))
	print("");
	i += 1;
	print("/* pause */")
	print("%s[%i][%i].hz = %f;" % (variable_name, track, i, 0))
	print("%s[%i][%i].ms = %f;" % (variable_name, track, i, 0.3*ms))
	print("");

print("%s[%i] = %i;" % (length_variable_name, track, i+1));
