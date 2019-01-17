import json
import os
import pkgutil

from gzip import GzipFile
from StringIO import StringIO   

model_weights = json.load(open("data/aptagger-0.1.0.json"))

st_fh = open("data/specified_tags.txt", "wb")
for k, v in model_weights["specified_tags"].iteritems():
    st_fh.write("%s %s " % (k, v))

bw_fh = open("data/bias_weights.txt", "wb")
for e in model_weights["bias_weights"]:
    bw_fh.write("%s %s " % (e[0], e[1]))

w_fh = open("data/weights.txt", "wb")
for e in model_weights["weights"]:
    for k, v in e.iteritems():
        w_fh.write("%s " % k)
        for x in v:
            w_fh.write("%s %s " % (x[0], x[1]))
        w_fh.write("\n")
    w_fh.write("####\n")