# imports
import sys;

# definitions
FREQUENCY_FILE = "freq.csv"

#
# Stateless function that will calculate time in ms for given
# note length.
#
def getDuration(length, bpm):
	
	# making variables floats
	bps = float(bpm) / 60.0;
	length = float(length);
	
	# One fourth should last one beat #
	num_beats = 4.0 / length;

	return 1000 * (num_beats / bps);

#
# Saving frequencies into datastructure
#
freq = {};
for line in open(FREQUENCY_FILE):
	split = line.strip().split(",");
	key = split[0].strip().split("/")[0];
	key = key.strip(" ");
	freq[key] = float(split[1]);




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

	if note == "0":
		f = 0.0;
	else:
		f = freq[note];

	
	print("/* note %s, length %i */" % (note, length));
	print("%s[%i][%i].hz = %f;" % (variable_name, track, i, f))
	print("%s[%i][%i].ms = %f;" % (variable_name, track, i, 0.9*ms))
	print("");
	i += 1;
	print("/* pause */")
	print("%s[%i][%i].hz = %f;" % (variable_name, track, i, 0))
	print("%s[%i][%i].ms = %f;" % (variable_name, track, i, 0.1*ms))
	print("");
	i += 1;

print("%s[%i] = %i;" % (length_variable_name, track, i+1));
