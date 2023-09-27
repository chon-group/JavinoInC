#include <javino.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include <unistd.h>
#include <fcntl.h>


// Returns a message string
// Returns NULL on error

char* javino_get_msg(int port)
{
    char buffer[ 7 ];
	
	char msg_size_str[5];
	
	int msg_size;


	//FILE* fd = fopen(port, "r");
	
	int nbytes_read = read(port, 
		buffer, 
		sizeof(char) * 6) ;
						
	if ( nbytes_read != (long int)(6*sizeof(char)) ){
		fprintf(stderr, "\nError! Couldn't get message header!");

        return NULL;

	} 
#ifdef __EXTRA_DEBUG_MESSAGES__				
	else {

		fprintf(stderr, 
			"\n(javino_get_msg) Message received header: %s", 
			buffer);
	}
#endif	
		
	msg_size_str[0] = '0';		
	msg_size_str[1] = 'x';		
	msg_size_str[2] = buffer[4];
	msg_size_str[3] = buffer[5];
	msg_size_str[4] = '\0';

#ifdef __EXTRA_DEBUG_MESSAGES__						
	fprintf(stderr, 
		"\n(javino_get_msg) Message size (string): %s", 
		msg_size_str);
#endif		
		
	msg_size = strtol(msg_size_str,
		NULL,
		0);

#ifdef __EXTRA_DEBUG_MESSAGES__						
	fprintf(stderr, 
    	"\n(javino_get_msg) Message size (int): %d\n", 
		msg_size);
#endif
			
	char *msg = (char*)malloc(
		sizeof(char)* (msg_size + 1) );			
			
			
	nbytes_read = read( port, 
		msg, 
		sizeof(char) * msg_size );
			
	msg[ msg_size ] = '\0';
						
	if ( (long unsigned)nbytes_read != msg_size*sizeof(char) ){

		fprintf(stderr, 
			"\n(javino_get_msg) Error: Expected %lu bytes read, got %lu",
			msg_size*sizeof(char),
			(long unsigned)nbytes_read );

		free( msg );

		return NULL;
	
    } else {
            
			
	}

#ifdef __DEBUG__							
	fprintf(stderr,
		"\n(javino_get_msg) Message received: %s", 
		msg);
#endif						
	
	//fclose(fd);

    return msg;
}


int javino_send_msg(int port, const char* msg_to_send)
{		

	//FILE* fd = fopen(port, "w");	

#ifdef __EXTRA_DEBUG_MESSAGES__		
	fprintf(stderr, 
        "\n(javino_send_msg) Message to send: %s", 
		msg_to_send);
	if ( fd == NULL ){

		fprintf(stderr, "(javino_send_msg) Error: couldn't open port %s for writing!", port);
		return 0;

	}
#endif		
		
	int msg_size = strlen( msg_to_send );
		
	char *msg = (char*) malloc( 
        sizeof(char) * ( msg_size + 1 ) );
		
	char hex_str[ 5 ];
		
	sprintf( hex_str, "%x", msg_size);
		
	if ( msg_size < 16 ){
		hex_str[ 1 ] = hex_str[ 0 ];
		hex_str[ 0 ] = '0';
	}		


#ifdef __EXTRA_DEBUG_MESSAGES__
	fprintf(stderr, 
		"\nmsg_size (hex): %s",
		hex_str);
#endif		
			
    msg[ 0 ] = 'f';
    msg[ 1 ] = 'f';
    msg[ 2 ] = 'f';
    msg[ 3 ] = 'e';
    msg[ 4 ] = hex_str[ 0 ];		
    msg[ 5 ] = hex_str[ 1 ];
		
	int i;
	for ( i = 0; i < msg_size; i++){

		msg[ 6 + i ] = msg_to_send[ i ];
		
	}
		
	msg[ 6 + i ] = '\0';
		
#ifdef __DEBUG__		
	fprintf(stderr, 
        "\n(javino_send_message) Javino message to send: %s",
		msg);
#endif		
			
	int nbytes_written = write( port , 
		msg,         
        (msg_size + 6) * sizeof(char) );
    								
	free( msg ); 

	//fclose( fd );
 
    return nbytes_written;
}