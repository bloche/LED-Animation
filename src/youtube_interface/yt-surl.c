/* Everett Bloch    9/25/13
   yt-surl.c

   This is a library for retreiving a youtube video stream url using curl.
   The main function getStreamURL() takes a youtube video url, strips the 
   video id, gets the youtube video info, extracts stream map, decodes
   url encoded information, and peices together a video stream url with 
   the desired format and quality. It returns a string containing the 
   stream url.
*/

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include "yt-surl.h"

/* memory struct to be passed to curl write callback function */
struct MemoryStruct {
  char *memory;
  size_t size;
};

/* local "private" procedure declarations */

// curl perform callback function: local function, only to be used in this file
static size_t curl_capture_cb (void *ptr, size_t size, size_t nmemb, void *stream);

// strip videoo id from youtube video url
static char* getVideoId(char* video_url);

// decode url encoded string
static char* decodeURL(CURL *curl, char* url);

// parse stream url from stream map
static char* parseURL(char* stream_map, VIDEO_FORMAT format, VIDEO_QUALITY quality);

// get last occurence of substr in str
static char* laststr(char* str, char* substr);

/* 
   getStreamURL() retrieves a youtube video stream url based on the 
   video_url video id and the video format / quality that is supplied.
   Be sure to free the string returned when finished with it.

   video_url : the youtube video url obtained from youtube website, or
               optionally, the video id specified at the end of the url.

   format : The desired format for the video stream, youtube offers four
            different streaming formats: webm, mp4, x-flv, and 3gpp.
	    VIDEO_FORMAT is an enum type, valid arguments are:
	    
	    VF_WEBM
	    VF_MP4
	    VF_XFLV
	    VF_3GPP

   quality : The desired video quality of the video stream, youtube offers
             five different video qualities: hd1080p hd720p, large, medium, 
	     and small.
	     VIDEO_QUALITY is an enum type, valid arguments are:

	     VQ_LOW
	     VQ_HIGH
	     
	     These qualities will take either the highest available quality,
	     VQ_HIGH, or the lowest availabe quality, VQ_LOW, for the 
	     specified video format.

   On success getStreamURL() returns a string containig the youtube video's 
   stream url.

   On failure getStreamURL() returns NULL.
*/
char* getStreamURL (char* video_url, 
		    VIDEO_FORMAT format, 
		    VIDEO_QUALITY quality) 
{
  CURL *curl;   // curl instance
  CURLcode rv;  // curl function return value

  char* video_id; // id of youtube video
  char* gvi_url = malloc (59);  // youtube get_video_info url 
  char* gvi_url_template = "http://www.youtube.com/get_video_info?video_id=";
  struct MemoryStruct gvi_info; // info returned from youtube's get_video_info
  char* stream_map;     // contains all video stream urls
  char* stream_url;     // contains desired video steam quality and format url
  char* temp;           // temporarally holds the url encoded stream map

  char* mark; // marks beginning of substring
  char* end;  // locates the end of substring
  
  // strip video id from youtube url
  video_id = getVideoId(video_url);
  if (video_id == NULL) {
    free(gvi_url);
    return "Error: Invalid URL / ID\n";
  }
  gvi_info.memory = malloc(1); // will get bigger with realloc
  gvi_info.size = 0;   // nothing in memory

  // set up youtube get_video_info url for curl
  strcpy(gvi_url, gvi_url_template);
  strcat(gvi_url, video_id);
  free(video_id);
  
  // curl request
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, gvi_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_capture_cb); 
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &gvi_info);
    rv = curl_easy_perform(curl);
    if (rv != CURLE_OK) {
      fprintf(stderr, "curl request failed: %s\n",
	      curl_easy_strerror(rv));
      free(gvi_url);
      return NULL;
    }
  }
  free(gvi_url);  // release precious memory

  // mark start and end of fmt stream map
  mark = strstr( gvi_info.memory, "url_encoded_fmt_stream_map=" );
  if (mark == NULL)
    return "Error: This video is protected by the YouTube copyright agreement and cannot be displayed\n";
  end = strchr( mark, '&');

  // move pointer to beginning of fmt stream map value
  mark = mark + 27;
  // null terminate stream map
  end[0] = '\0';
  // copy stream map
  temp = strdup(mark);
  if (temp == NULL) {
    fprintf(stderr, "strdup failed\n");
    free(gvi_info.memory);
    return NULL;
  }
  free(gvi_info.memory); // release precious memory

  stream_map = decodeURL(curl, temp);
  if (stream_map == NULL) {
    fprintf(stderr, "Failed decode URL\n");
    free(temp);
    return NULL;
  }
  free(temp);  // release precious memory
 
  stream_url = parseURL(stream_map, format, quality);
  
  free(stream_map);

  // end curl session
  curl_easy_cleanup(curl);

  return stream_url;
}

/* callback function that curl writes to when curl_easy_perform is called */
static size_t curl_capture_cb (void *contents, size_t size, 
			       size_t nmemb, void *cb_data) 
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *) cb_data;
  
  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory == NULL) {
    perror("curl_capture_cb : realloc");
    exit(1);
  }
  
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

/*
  Extracts youtube video id from url. Returns video id on success,
  NULL on failur.

  If only id is supplied, this will copy input string and return.
*/
static char* getVideoId(char* video_url) 
{
  char* video_id = NULL;
  int url_len = strlen(video_url);
  int id_len = 11;     
  int i, j;   // loop variables

  // assume only video id was put in with no url
  if (url_len == id_len) {
    video_id = strdup(video_url);
  }
  else { // serach for youtube video id in url
    // find id parameter "v="
    for (i = 0; i < (url_len - 1); i++) {
      if ( video_url[i] == 'v' ) 
	if ( video_url[++i] == '=' )
	  break;
    }
    
    // extract video id, if pressent
    if (strlen(video_url + i) >= id_len) {
      video_id = malloc(id_len +1);
      for (j = 0; j < id_len; j++) {
	video_id[j] = video_url[++i];
      }
      video_id[j] = '\0';
    }
  }

  return video_id;
}

/* decodes url encoded string */
static char* decodeURL(CURL *curl, char* encoded_url) 
{
  char* url = NULL;
  int urllen;
  int outlen;

  // loop until url is entirely decoded
  do {
    urllen = strlen(encoded_url);
    // decode url encoded string
    url = curl_easy_unescape(curl, encoded_url, urllen, &outlen); 
    if (url == NULL) {
      break;
    }
    
    encoded_url = url;
  } while (urllen != strlen(url));

  return url;
}

/* parse stream url from stream_map, with given format and quality */
static char* parseURL(char* stream_map, 
		      VIDEO_FORMAT format, 
		      VIDEO_QUALITY quality)
{
  enum { // stream map param types
    URL, CP, IP, UPN, SPARAMS, SVER, SOURCE, EXPIRE, ITAG, 
    IPBITS, ID, KEY, RATEBYPASS, FEXP, BURST, PCM2FR, HIGHTC, 
    ALGORITHM, CLEN, DUR, FACTOR, GIR, MT, MV, MS, 
    FALLBACK_HOST, TYPE, QUALITY, SIG, N_PARAMS
  };
  char* url = NULL;  // stream url
  char first_param[11]; // first param in stream map
  char end_param[13];// contains end of param is token, "," + first_param
  char *url_attached_param; // parameter that is after ? in url
  char* formatstr = strFormat(format); // string representaiton of format
  
  char* mark; // mark beginning of correct url param list
  char* end;  // end of correct url param list

  char* map[N_PARAMS]; // all required param fields for url

  size_t url_len = 0; // lenght of returned url
  int i;  // loop iterator

  // get first parameter in stream map
  for (i = 0; stream_map[i] != '\0' && i < 9; i++) {
    first_param[i] = stream_map[i];
    if (stream_map[i] == '=')
      break;
  }
  first_param[i+1] = '\0';

  // get string that designates end of inidividual url param list
  snprintf(end_param, 13, ",%s", first_param); // "," + first_param

  // find desired video stream url params
  if (quality == VQ_HIGH) {
    // find highest quality video format, first in stream map
    mark = strstr(stream_map, formatstr);
    if (mark == NULL) {
      // cannot find format string
      fprintf(stderr, "Failed to find format string (1)\n");
      return NULL;
    }

    // get end of needed url params, ie start of next set of params
    end = strstr(mark, end_param);
    if (end == NULL) 
      // desired param list is at end of list
      end = stream_map + strlen(stream_map);
    end[0] = '\0';

    // mark points to start of desired url param list
    mark = laststr(stream_map, first_param);
    if (mark == NULL) {
      // cannot find format string
      fprintf(stderr, "Failed to find format string (2)\n");
      return NULL;
    }
  } // end if (quality == VQ_HIGH)
  else { // VQ_LOW
    mark = laststr(stream_map, formatstr);
    if (mark == NULL) {
      // cannot find format string
      fprintf(stderr, "Failed to find format string (3)\n");
      return NULL;
    }

    // get end of needed url params, ie start of next set of params
    end = strstr(mark, end_param);
    if (end == NULL)  
      // desired param list is at end of list
      end = stream_map + strlen(stream_map);
    end[0] = '\0';

    // mark points to start of desired url param list
    mark = laststr(stream_map, first_param);
    if (mark == NULL) {
      // cannot find format string
      fprintf(stderr, "Failed to find format string (4)\n");
      return NULL;
    }
  }

  // parse url params
  map[URL] = strstr(mark, "url=");
  map[CP] = strstr(mark, "cp=");
  map[IP] = strstr(mark, "ip=");
  map[UPN] = strstr(mark, "upn=");
  map[SPARAMS] = strstr(mark, "sparams=");
  map[SVER] = strstr(mark, "sver");
  map[SOURCE] = strstr(mark, "source=");
  map[EXPIRE] = strstr(mark, "expire=");
  map[ITAG] = strstr(mark, "itag=");
  map[IPBITS] = strstr(mark, "ipbits=");
  map[ID] = strstr(mark, "id=");
  map[KEY] = strstr(mark, "key=");
  map[RATEBYPASS] = strstr(mark, "ratebypass=");
  map[FEXP] = strstr(mark, "fexp=");
  map[BURST] = strstr(mark, "burst=");
  map[PCM2FR] = strstr(mark, "pcm2fr=");
  map[HIGHTC] = strstr(mark, "hightc=");
  map[ALGORITHM] = strstr(mark, "algorithm=");
  map[CLEN] =  strstr(mark, "clen=");
  map[DUR] = strstr(mark, "dur=");
  map[FACTOR] = strstr(mark, "factor=");
  map[GIR] = strstr(mark, "gir=");
  map[MT] = strstr(mark, "mt=");
  map[MV] = strstr(mark, "mv=");
  map[MS] = strstr(mark, "ms=");
  if (map[MS] > map[SPARAMS]) // will get ms part of sparams if >
    map[MS] = strstr(map[MS] + 1, "ms=");
  map[FALLBACK_HOST] = strstr(mark, "fallback_host=");
  map[TYPE] = strstr(mark, "type=");
  map[QUALITY] = strstr(mark, "quality=");
  map[SIG] = strstr(mark, "sig="); // will replace "sig=" with "signature="

  // have to have a value for url field
  if (map[URL] == NULL) {
    fprintf(stderr, "Failed to find URL param\n\n%s\n", mark);
    return NULL;
  }
  map[URL] += 4;

  // null terminate all perams
  for (i=0; i<N_PARAMS; i++) {
    if(map[i] != NULL) {
      // replace delimenator & with '\0'
      end = strchr(map[i], '&');
      if (end != NULL) {
	end[0] = '\0';
      }
    }
  }

  // find parameter attached to ? part of url
  url_attached_param = strchr(map[URL], '?');
  if (url_attached_param == NULL) {
    fprintf(stderr, "Failed to find param after ?\n");
    return NULL;
  }
  url_attached_param += 1;

  url_len = 1024; // make sure buffer for url will be big enough

  url = (char*) malloc(url_len); 
  if (url == NULL) {
    perror("parseURL : malloc");
    return NULL;
  }
    
  // copy parsed url params into string url
  strcpy(url, map[URL]);
  strcat(url, "&");
  for (i=1; i<SIG; i++)
    if (map[i] != NULL) {
      // dont copy param that is included in url, or second itag
      if (strcmp(map[i], url_attached_param) != 0) {
	strcat(url, map[i]);              
	strcat(url, "&");
      }
    }
  if (map[SIG] != NULL) {
    map[SIG] += 4;
    strcat(url, "signature=");
    strcat(url, map[SIG]);
  } 
  else {
    fprintf(stderr, "Failed to find signature param\n");
    free(url);
    return NULL;
  }
  return url;
}

char* strFormat(VIDEO_FORMAT format)
{
  switch(format) {
  case VF_WEBM:
    return "video/webm";
    break;
  case VF_MP4:
    return "video/mp4";
    break;
  case VF_XFLV:
    return "video/x-flv";
    break;
  case VF_3GPP:
    return "video/3gpp";
    break;
  default:
    return NULL;
  }
}

/* find last occurrence of substr in str */
static char* laststr(char* str, char* substr)
{
  char* temp;
  char* str2;

  // check if substr is in str
  temp = strstr(str, substr);
  if (temp == NULL)
    return NULL;

  // find last occurrence of substr
  while (temp != NULL) {
    str2 = str;
    str = temp;
    temp = strstr(str + 1, substr);
  }

  return strcmp(substr, "itag=") ? str : str2;
}
