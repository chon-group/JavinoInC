#include <javino.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// Returns a message string
// Returns NULL on error

char* javino_get_msg(const char* port)
{
    char buffer[ 7 ];
	
	char msg_size_str[5];
	
	int msg_size;

	FILE* fd = fopen(port, "r");
	
	int nbytes_read = fread(buffer, 
		sizeof(char), 
		6,
		fd);
						
	if ( nbytes_read != (long int)(6*sizeof(char)) ){
		fprintf(stderr, "Error! Couldn't get message header!");

        return NULL;

	} else {
			
		fprintf(stderr, 
			"%s", 
			buffer);
	}
		
	msg_size_str[0] = '0';		
	msg_size_str[1] = 'x';		
	msg_size_str[2] = buffer[4];
	msg_size_str[3] = buffer[5];
	msg_size_str[4] = '\0';
		
	fprintf(stderr, 
		"\n(get msg) Msg size (str): %s", 
		msg_size_str);		
		
	msg_size = strtol(msg_size_str,
		NULL,
		0);
		
	fprintf(stderr, 
    	"\n(get msg) Msg size (int): %d\n", 
		msg_size);
			
	char *msg = (char*)malloc(
		sizeof(char)* (msg_size + 1) );			
			
			
	nbytes_read = fread( msg, 
		sizeof(char), 
		msg_size,
		fd);
			
	msg[ msg_size ] = '\0';
						
	if ( (long unsigned)nbytes_read != msg_size*sizeof(char) ){

		fprintf(stderr, 
			"\nError! Expected %lu bytes read, got %lu",
			msg_size*sizeof(char),
			(long unsigned)nbytes_read );
	
    } else {
            
			
	}
					
	fprintf(stderr,
		"\n(get msg) MSG: %s", 
		msg);								
	
	fclose(fd);

    return msg;
}


int javino_send_msg(const char* port, const char* msg_to_send)
{		

	FILE* fd = fopen(port, "w");	
		
	fprintf(stderr, 
        "\nMessage to send: %s", 
		msg_to_send);
		
	int msg_size = strlen( msg_to_send );
		
	char *msg = (char*) malloc( 
        sizeof(char) * ( msg_size + 1 ) );
		
	char hex_str[ 5 ];
		
	sprintf( hex_str, "%x", msg_size);
		
	if ( msg_size < 16 ){
		hex_str[ 1 ] = hex_str[ 0 ];
		hex_str[ 0 ] = '0';
	}		
		
	fprintf(stderr, 
		"\nmsg_size (hex): %s",
		hex_str);
			
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
		
	fprintf(stderr, 
        "\nFormatted Javino MSG: %s",
		msg);
			
	int nbytes_written = fwrite( msg, 
        sizeof(char), 
        msg_size + 6, 
        fd );
    								
	free( msg );  

    return nbytes_written;
}