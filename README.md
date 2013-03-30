OVERVIEW
========
Liberty-Daemon is the daemon process that communicates over USB with
the Polhemus High-Speed Liberty motion tracker and reads data from
the eight sensors and puts them onto the "liberty" ach channel.
The daemon reads position (x, y, z) and rotation (quaternion w, i, j, k). 

COMMUNICATION
=============
* Set up for USB (but RS-232 is possible)

PREQUISITES
===========
The following are necessary prerequites to use Liberty:

1. install libusb-1.0.0-dev

    `sudo apt-get install libusb-1.0.0-dev`

2. install ach (up-to-date instruction are at http://github.com/golems/ach)

  * Add the following to /etc/apt/sources:

      deb http://code.golems.org/debian squeeze golems.org

  (substitute the codename for your distribution in place of squeeze,
  e.g. wheezy, precise)

    `sudo apt-get update && sudo apt-get install libach-dev ach-utils`
  
    `sudo dpkg-reconfigure ach-utils`

INSTALLATION
=========
1. go to the Liberty-Daemon directory

      `cd Liberty-Daemon`

2. compile libertyd.cpp

      `make`

USAGE
=====
1. create an ach channel called 'liberty' ("-o 666" gives permissions)

    `ach -C liberty -o 666`

2. run the libertyd executable

    `./libertyd`

QUIRKS
======

- Device takes about ten seconds to initialize after switched on.  You
  must wait this time before starting the daemon.

- The sensor can only measure a positive displacement in X.  DO NOT
  BREAK THE YZ PLANE!

- The daemon does not use Euler angles, and you shouldn't either.
