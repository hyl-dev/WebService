#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <error.h>
#include <sys/time.h>
#include <fcntl.h>
#include <crypt.h>
#include <unistd.h>

#include "fastcgi.h"
#include "fcgios.h"
#include "fcgi_stdio.h"
#include "cJSON.h"

#define STANDALONE_MODE					1

#ifndef FCGI_SOCKET
#define FCGI_SOCKET						"/tmp/WebService.fastcgi.socket"
#endif

#if STANDALONE_MODE
static int stdinFds[ 3 ];
#endif


#define HTTP_STATUS_200_OK \
	"Status: 200 OK\r\n" \
	"Content-Type: application/json\r\n\r\n"    //"{\"Success\":\"OK\"}"

#define HTTP_STATUS_200_SAVE_UIPARM_OK \
	"Status: 200 OK\r\n" \
	"Content-Type: application/json\r\n\r\n" \
	"{\"Success\":\" param save success\"}"

#define HTTP_STATUS_401_NO_COOKIE \
	"Status: 401 Unauthorized\r\n" \
	"Content-Type: application/json\r\n\r\n" \
	"{\"Error\":\"No Cookie\"}"

#define HTTP_STATUS_401_INVALID_SESSION \
	"Status: 401 Unauthorized\r\n" \
	"Content-Type: application/json\r\n\r\n" \
	"{\"Error\":\"Invalid Cookie\"}"

#define HTTP_STATUS_403_FORBIDDEN \
	"Status: 403 Forbidden\r\n" \
	"Content-Type: text/plain\r\n\r\n" \
	"Forbidden"

#define HTTP_STATUS_404_NOT_FOUND \
	"Status: 404 Not Found\r\n" \
	"Content-Type: text/plain\r\n\r\n" \
	"File Not Found"

#define HTTP_STATUS_500_INTERNAL_ERROR \
	"Status: 500 internal server_error\r\n" \
	"Content-Type: text/plain\r\n\r\n" \
	"internal server_error"


typedef void (*HTTP_REQUEST_HANDLER) (
	char const * request_uri,
	char const * query_string,
	char const * http_accept,
	char const * http_user_agent,
	char const * http_accept_encoding,
	char const * http_accept_language,
	char const * http_cookie,
	char const * content_length );

const struct
{
	char request_method[ 16 ];
	HTTP_REQUEST_HANDLER handler;
} HTTP_REQUEST_HANDLER_MAP[] =
{
	{ "GET", on_get_request },
	{ "PUT", on_put_request },
	{ "PATCH", on_patch_request },
	{ "POST" ,on_post_request}
};


static void exit_handler ( void )
{

	// handler your func to free

}

char char_to_int ( char c )
{
	if ( c >= '0' && c <= '9' )
	{
		return c - '0';
	}
	else if ( c >= 'a' && c <= 'f' )
	{
		return c - 'a' + 10;
	}
	else if ( c >= 'A' && c <= 'F' )
	{
		return c - 'A' + 10;
	}
	else
	{
		return 0;
	}
}

static void convert_url (  const char * url_str )
{
	const char * s = url_str;
	char * d = url_str;
	while ( *s != 0 )
	{
		switch ( *s )
		{
		case '%':
			{
				*d = char_to_int( s[ 1 ] );
				*d <<= 4;
				*d += char_to_int( s[ 2 ] );
				d++;
				s += 3;
			}
			break;
		default:
			*d++ = *s++;
			break;
		}
	}
	*d = 0;
}

static void on_get_request (
	char const* request_uri,
	char const * query_string,
	char const * http_accept,
	char const * http_user_agent,
	char const * http_accept_encoding,
	char const * http_accept_language,
	char const * http_cookie,
	char const * content_length )
{
	cJSON * json_input;
	char * json_str = NULL;

	do
	{
		if ( strcmp(request_uri, "/devices/datastream/ch0") == 0) {
			// do your making json data

		} else {
			printf( HTTP_STATUS_404_NOT_FOUND);
		}

	} while ( 0 );
}

static void on_put_request (
	char const * request_uri,
	char const * query_string,
	char const * http_accept,
	char const * http_user_agent,
	char const * http_accept_encoding,
	char const * http_accept_language,
	char const * http_cookie,
	char const * content_length )
{
	char * cookie = NULL;

	// do your writing json data


}

static void on_post_request (
	char const * request_uri,
	char const * query_string,
	char const * http_accept,
	char const * http_user_agent,
	char const * http_accept_encoding,
	char const * http_accept_language,
	char const * http_cookie,
	char const * content_length )
{
	// progrocess post request


}
static void on_patch_request (
	char const * request_uri,
	char const * query_string,
	char const * http_accept,
	char const * http_user_agent,
	char const * http_accept_encoding,
	char const * http_accept_language,
	char const * http_cookie,
	char const * content_length )
{
	// progrocess patch request
	int ret = 0;
	char * cookie = NULL;
	char *input_parm;
	int len;

	if ( http_cookie != NULL )
	{
		// check your cookie
		cookie = strstr( http_cookie, "=" );
		cookie++;

		if ( content_length != NULL )
		{
			len = strtol( content_length, NULL, 10 ) ;
			input_parm = (char *) apal_mem_alloc( len + 1 ) ;
			FCGI_fgets( input_parm , len + 1,FCGI_stdin );
			input_parm[ len ] = '\0';
			printf( "%s : input_parm : %s" ,__func__,input_parm);

			if( strcmp(request_uri, "/devices/data/") == 0 )
			{
				//process your url
			}

		}
		else
		{
			printf( HTTP_STATUS_404_NOT_FOUND );
		}
	}
	else
	{
		printf( HTTP_STATUS_401_NO_COOKIE );
	}
}


static void handle_request (
	char * request_uri,
	char * query_string,
	char * request_method,
	char * http_accept,
	char * http_user_agent,
	char * http_accept_encoding,
	char * http_accept_language )
{
	char * content_length;
	char * http_cookie;
	uint32_t len;
	int i;

	content_length = getenv( "CONTENT_LENGTH" );
	http_cookie = getenv( "HTTP_COOKIE" );

	len = apal_str_len( request_uri );
	while ( len > 1 )
	{
		if ( '/' == request_uri[ len - 1 ] )
		{
			request_uri[ len - 1 ] = 0;
			len--;
		}
		else
		{
			break;
		}
	}

	convert_url( request_uri );

	for ( i = 0; i < sizeof( HTTP_REQUEST_HANDLER_MAP ) / sizeof( HTTP_REQUEST_HANDLER_MAP[ 0 ] ); i++ )
	{
		if ( apal_str_cmp( request_method, HTTP_REQUEST_HANDLER_MAP[ i ].request_method ) == 0 )
		{
			HTTP_REQUEST_HANDLER_MAP[ i ].handler(
				request_uri,
				query_string,
				http_accept,
				http_user_agent,
				http_accept_encoding,
				http_accept_language,
				http_cookie,
				content_length );
			break;
		}
	}
}


int main ( int argc, char * argv[ ] )
{
	int rc;
	int listen_fd;
	atexit( exit_handler );


#if STANDALONE_MODE
	rc = OS_LibInit( stdinFds );
	if ( rc != 0 )
	{
		APAL_LOG_ERROR( "Error initializing OS library: %d\n", rc );
		return -8;
	}

	if ( ( listen_fd = OS_CreateLocalIpcFd( FCGI_SOCKET, 10 ) ) == -1 )
	{
		APAL_LOG_ERROR( "OS_CreateLocalIpcFd failed\n" );
		return -9;
	}
	chmod( FCGI_SOCKET, ACCESSPERMS );

	close( STDIN_FILENO );
	if ( listen_fd != FCGI_LISTENSOCK_FILENO )
	{
		dup2( listen_fd, FCGI_LISTENSOCK_FILENO );
		close( listen_fd );
	}
	close( STDOUT_FILENO );
	close( STDERR_FILENO );

	APAL_LOG_INFO( "%s: start FCGI_Accept\n", __func__ );

#endif
	while ( FCGI_Accept() >= 0 )
	{
#if 1
		handle_request(
			getenv( "REQUEST_URI" ),
			getenv( "QUERY_STRING" ),
			getenv( "REQUEST_METHOD" ),
			getenv( "HTTP_ACCEPT" ),
			getenv( "HTTP_USER_AGENT" ),
			getenv( "HTTP_ACCEPT_ENCODING" ),
			getenv( "HTTP_ACCEPT_LANGUAGE" ) );
#else
		//test the server
		char** envp;
		int size = 200;
		char* result;
		extern char **environ;

		for ( envp = environ; *envp; ++envp )
		{
			size += strlen( *envp ) + 11;
		}
		result = (char*) alloca( size );
		strcpy( result, "Status: 200 OK\r\nContent-Type: text/html\r\n\r\n" );
		strcat(
			result,
			"<html><head><title>testcgi</title></head><body><ul>\r\n" );

		for ( envp = environ; *envp; ++envp )
		{
			strcat( result, "<li>" );
			strcat( result, *envp );
			strcat( result, "</li>\r\n" );
		}

		strcat( result, "</ul></body></html>\r\n" );
		printf( "%s", result );
#endif
	}
	return 0;
}

