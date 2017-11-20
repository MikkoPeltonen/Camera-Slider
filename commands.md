General format

0		start
1		command
2-n 	data
n + 1	stop

If a command send from the client requires a response, the response is
sent with the same command byte.



Handshake (greeting, welcome)
-----------------------------------------------------------
0		  start
1		  greeting
2-17	UUIDv4					Secret that must match
18		stop


Handshake (welcome)
-----------------------------------------------------------
0		start
1		welcome					  tells client I'm ready
2		stop


Status
-----------------------------------------------------------
0		  start
1		  status_message
2		  status 					running/stopped
3		  mode 					  timelapse/video
4-8		elapsed					32 bit unsigned long
9-13	remaining				32 bit unsigned long
14		stop


Position
-----------------------------------------------------------
0		  start
1		  position
2-6		slide					32 bit unsigned long
7-11	pan						32 bit unsigned long
12-16	tilt 					32 bit unsigned long
17-21	focus 				32 bit unsigned long
22-26	zoom					32 bit unsigned long
27		stop


Move
-----------------------------------------------------------
0   start
1   move
2   stepper         8 bit unsigned int
3   angle           32 bit signed long
4   stop


Move (continuous, button down)
-----------------------------------------------------------
0   start
1   move_hold
2   bit   data value
    0     slide direction
    1     slide on/off
    2     pan direction
    3     pan on/off
    4     tilt direction
    5     tilt on/off
3   0     focus direction
    1     focus on/off
    2     zoom direction
    3     zoom on/off
4    stop


BEGIN_DATA_DOWNLOAD
-----------------------------------------------------------
0   start
1   BEGIN_DATA_DOWNLOAD
2-5 count                      number of instructions to receive
