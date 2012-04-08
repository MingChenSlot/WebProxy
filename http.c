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


//void http_error(int status, socketstream *ss)
//{
//  // status line
//  ss->snprintf_buf("HTTP/1.0 %d%s\r\n", status, get_status_text(status));
//  generate_http_common_response_headers(ss);
//  ss->snprintf_buf("Content-type: text/html\r\n\r\n");
//  ss->snprintf_buf("<html>Error Status: %d %s</html>", status, get_status_text(status));
//  ss->flush_buf();
//  exit(0);
//
//}
//
//void generate_http_common_response_headers(socketstream *ss)
//{
//    // Data header
//  time_t now;
//  char datebuf[128];
//  const char* rfc1123_fmt = "%a, %d %b %Y %H:%M:%S GMT";
//  time(&now);
//  strftime(datebuf, sizeof(datebuf), rfc1123_fmt, gmtime(&now));
//  ss->snprintf_buf("Date: %s\r\n", datebuf);
//
//  // Server header
//  ss->snprintf_buf("Server: SimpleServer/0.0 (Unix)\r\n");
//
//  // Connectoin header
//  ss->snprintf_buf("Connection: close\r\n");
//}
