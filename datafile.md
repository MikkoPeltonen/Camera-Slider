This document specifies the format of the data file used to store the commands
for moving the slider.

Line structure
-----------------------------------------------------------

All integer values are 32 bit unsigned longs.

1-4    start_time       Time when the action starts. Relative to initial start.
5-8    duration         Total time required for the action.
9-12   slide            Slider move length (mm)
13-16  pan              Pan angle (degrees)
17-20  tilt             Tilt angle (degrees)
21-24  focus            Focus angle (degrees)
25-28  zoom             Zoom angle (degrees)
