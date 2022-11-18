#Importing sys, numpy and obspy
import sys
import numpy as np
import obspy.io.css
from obspy.core.utcdatetime import UTCDateTime
from obspy.core import Stream as st
from obspy.core import read, Trace, Stream
from obspy.signal.trigger import plot_trigger
from obspy.signal.trigger import trigger_onset
from obspy.signal.trigger import recursive_sta_lta
from obspy.signal.trigger import classic_sta_lta
from configparser import ConfigParser

#Defineing configur
configur = ConfigParser()
configur.read(sys.argv[1])

flo = configur.getfloat('filters','flo')
fhi = configur.getfloat('filters','fhi')
thon = configur.getfloat('thresh','ton')
thof = configur.getfloat('thresh','toff')
staw = configur.getfloat('window','sta_win')
ltaw = configur.getfloat('window','lta_win')

#Defineing stream
st = read(sys.argv[2])
#print st

#Defineing trace (z-component in this case)
trz = st[2]

#Defineing time format
print(trz.stats.starttime)
time = UTCDateTime(trz.stats.starttime)

#Printing time
print(time.timestamp)

#Filtering the data with selected frequency
trz_filt = trz.copy()
trz_filt.detrend()
trz_filt.filter('bandpass', freqmin=flo, freqmax=fhi, corners=3, zerophase=False)

df = trz_filt.stats.sampling_rate 

#Defineing the correct automatic detector
#cft = recursive_sta_lta(trz_filt.data, int(staw * df), int(ltaw * df))
cft = classic_sta_lta(trz_filt.data, int(staw * df), int(ltaw * df))

#Defineing the trigger and plotting the data
trig = trigger_onset(cft, thon, thof)
plot_trigger(trz_filt, cft, thon, thof)

#Printing the graph
print(trig)

#creating a file
detaika = np.asarray(time.timestamp)+np.asarray(trig)/np.asarray(df)
#f = open("%s.dets" % sys.argv[1], "w")
f = open("obspydets.out", "w")
f.write("%s\n" % (trz_filt.stats))
np.savetxt(f, (detaika), fmt="%lf")
f.close()

