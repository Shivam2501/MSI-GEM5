from m5.SimObject import SimObject
from m5.params import *
from m5.proxy import *

class Predictor(SimObject):
    type = 'Predictor'
    cxx_class = 'Predictor'
    cxx_header = "mem/ruby/structures/Predictor.hh"

    num_histories = Param.UInt32(8,
        "Number of entries in the history table")
    num_patterns = Param.UInt32(8,
        "Number of entries the pattern table")
