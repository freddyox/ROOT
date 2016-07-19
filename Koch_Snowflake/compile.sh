rootcint -f SnowflakeAppaDict.cxx -c Snowflake.hh SnowflakeAppaLinkDef.h

g++ `root-config --cflags` -o flake Snowflake.cpp SnowflakeAppaDict.cxx `root-config --glibs`

#./pmt_analysis
