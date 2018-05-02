# project2_ecen5013-002
Project2 for ecen5013-002 Advanced practical embedded software.
Note, must use `-X` ssh option when connecting to BeagleBone to enable X11 forwarding for GUI applications (kst).

## Build commands:

### HOST
```
make clean build
```

### BBB (also upload)
```
make clean run PLATFORM=BBB
```

## Launch options:
Consume packets from BBB serial port, write to dirfile.
```
./project2 -f /dev/ttyO4
```
Consume packets from HOST serial port, write to dirfile.
```
./project2 -f /dev/ttyUSB0
```

Write continuous packet stream of dummy data to `stream` file and also read and convert this stream to dirfile.
```
./project2 -f stream -g
```

Only generate stream file, do NOT parse to dirfile.
```
./project2 -f stream -gb
```

Parse stream file to dirfile.
```
./project2 -f stream
```

## PLotting
Launch kst to plot dirfile data in realtime (all fields).
```
kst2 all_config.kst &
```

Launch kst to plot dirfile data in realtime (only setpoint, speed, error).
```
kst2 config.kst &
```
