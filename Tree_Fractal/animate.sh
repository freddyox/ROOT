#!/bin/bash

cd Screenshots1

convert -delay 5 -loop 0 FractalTree_{0..360}.png fractal2.gif

#delay is 10/100 seconds inbetween images. No loops, and no problems
#iterating through my directory with the inclusion of {0..149} (150 files)
