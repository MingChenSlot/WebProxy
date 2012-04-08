/*
 * http.c
 *
 *  Created on: Apr 7, 2012
 *      Author: chenming
 */

#include "server.h"

const char *get_status_text(int status)
{
  if (status >= 100 && status <= 199)
    return "Information";
  if (status >= 200 && status <= 299)
    return "OK";
  if (status >= 300 && status <= 399)
    return "Redirect";
  if (status >= 400 && status <= 499)
    return "Client error";
  if (status >= 500 && status <= 599)
    return "Server error";
  return "Internal error";
}

const char* get_mime_type(char *filename)
{
  char* dot;

  dot = strrchr(filename, '.' );
  if ( dot == (char*) 0 )
    return "text/plain; charset=iso-8859-1";

  if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
    return "text/html; charset=iso-8859-1";

  if (strcmp( dot, ".jpg") == 0 || strcmp( dot, ".jpeg") == 0)
    return "image/jpeg";

  if (strcmp(dot, ".gif") == 0)
    return "image/gif";

  return "text/plain; charset=iso-8859-1";
}
