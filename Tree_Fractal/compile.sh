rootcint -f TreeAppaDict.cxx -c Tree.hh TreeAppaLinkDef.h

g++ `root-config --cflags` -o tree Tree.cpp TreeAppaDict.cxx `root-config --glibs`

./tree
