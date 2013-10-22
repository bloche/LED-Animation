/* Everett Bloch   9/25/13
   yt-surl.h

   Header file for sturl.c.
   
   This file defines enums used for retrieving youtube stream url with 
   desired video format and quality. These enums list the possible types 
   of each catigory provided by youtube.
   
   This file also declares all procedure prototypes needed for urlstream.c.
*/

#ifndef URLSTREAM_H
#define URLSTREAM_H

// possible youtube video stream formats
typedef enum {
  VF_WEBM,
  VF_MP4,
  VF_XFLV,
  VF_3GPP
} VIDEO_FORMAT;

// possible video quality formats
typedef enum {
  VQ_LOW,
  VQ_HIGH
} VIDEO_QUALITY;

/* 
   Retrieve youtube stream url based on video_url video id, and video 
   format / quality.

   On failure returns NULL.
*/
char* getStreamURL (char* video_url, 
		    VIDEO_FORMAT format, 
		    VIDEO_QUALITY quality);

/* return string for video format, or NULL if invalid format supplied */
char* strFormat (VIDEO_FORMAT format);
#endif
