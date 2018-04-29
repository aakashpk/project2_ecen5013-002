# Create files:

# Launch kst
# May need to invoke kst script in background that first sleeps for a second
# to ensure dirfile gets setup correctly by main application
kst2 <kst-config-file>

# hopefully this works if logfile gets re-written
# may need same sleep strategy
x-terminal-emulator -e tail -f <logfile.txt>

# launch application last
# pass any required options
# could hardcode logfile for now
