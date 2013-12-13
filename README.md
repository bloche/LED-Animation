#LED-Animation

This program was created specifically for, but is not restricted to, streaming a YouTube video on an LED panel through a Raspberry Pi. 

This is a University of Idaho, Fall 2013 semester, senior design project created by team Automaten. For more information please visit the project's wiki page, which can be found at http://mindworks.shoutwiki.com/wiki/LED_Animation. 

###Secifications and Dependencies

####Raspberry PI
* Debian Operating System
* RM1176JZF-S 700 MHz processor
* 512MB RAM

####OpenCV
* CMake 2.6 or higher.
* GTK+2.x or higher, including headers (libgtk2.0-dev).
* pkgconfig;
* Python 2.6 or later and Numpy 1.5 or later with developer packages (python-dev, python-numpy).
* ffmpeg or libav development packages: libavcodec-dev, libavformat-dev, libswscale-dev.

####libcurl
* libcurl4-gnutls-dev binary package
* v. 7.9.6 or later
* curl 7.14.0 or later

####Qt
* v. 4.5.2 or higher
* qmake 4.5 or higher

####LED Display
* 32x32 LED panel, this project's was acquired from Adafruit (adafruit.com).
* Custom 16-to-26 pin adapter.
* Image data in 3072x1 8-bit array, per frame, per LED display