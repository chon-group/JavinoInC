#include <javino.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

char* recv_buffer[ 256 ];

pthread_mutex_t mutex;

pthread_t thread_id;

int wr_port;
int rd_port;

int in;
int out;
int size;
// Returns a message string
// Returns NULL on error

void* main_loop()
{
    char buffer[ 7 ];
	
	char msg_size_str[5];
	
	int msg_size;

	int nbytes_read;

	while (1) {
	
		nbytes_read = read( rd_port , 
			buffer,  
			sizeof(char) * 6) ;
							
		if ( nbytes_read != (long int)(6*sizeof(char)) ){
			fprintf(stderr, "\nError! Couldn't get message header!");

			return NULL;

		} 
#ifdef __EXTRA_DEBUG_MESSAGES__				
	else {

		fprintf(stderr, 
			"(javino_get_msg) Message header: %s", 
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
			
			
		nbytes_read = read( rd_port,
			msg, 
			sizeof(char) * msg_size );
			
		msg[ msg_size ] = '\0';


		pthread_mutex_lock( &mutex );

		if (size < 256){

			recv_buffer[ in % 256 ] = msg;
			in++;
			size++;

		} else {

			fprintf(stderr, "\nError! Receive buffer is full!");

		}

		pthread_mutex_unlock( &mutex );	
						
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
	}			
	
	//fclose(fd);
}

int javino_init(const char *wr_port_path, const char *rd_port_path){

	if ( wr_port_path == NULL ){
		fprintf(stderr, 
			"\n(javino_init) ERROR: write port not set on Javino init - aborting");

		return -1;
	}

	if ( rd_port_path == NULL ){
		fprintf( stderr, 
			"\n(javino_init) ERROR: read port not set on Javino init - aborting");

		return -2;
	}

	if ( ! strcmp(wr_port_path, rd_port_path) ){

		// Ports are equal, open device as read and write
		wr_port = rd_port = open( wr_port_path, O_RDWR );

		if (wr_port == -1 ){

			perror("(javino_init)");

			return wr_port;
		}

	} else {

		wr_port = open( wr_port_path, O_WRONLY );
		if (wr_port == -1 ){

			perror("(javino_init)");

			return wr_port;
		}		

		rd_port = open( rd_port_path, O_RDONLY );
		if (rd_port == -1 ){

			perror("(javino_init)");

			return wr_port;
		}
	}

	pthread_mutex_init( &mutex, NULL);

	in = out = size = 0;

	for (int i =0; i < 256; i ++ ){

		recv_buffer[ i ] = NULL;

	}

	pthread_create( &thread_id,
		NULL,
		main_loop,
		NULL );

	return 0;

}


char* javino_get_msg(){

	char *msg_ptr;

	pthread_mutex_lock( &mutex );

	if (size > 0){
		msg_ptr = recv_buffer[ out % 256 ];

		recv_buffer[ out % 256 ] = NULL;

		out++;
		size--;
	} else {

		msg_ptr = NULL;

	}

	pthread_mutex_unlock( &mutex );

	return msg_ptr;
}


int javino_send_msg(const char* msg_to_send)
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
			
	int nbytes_written = write( wr_port , 
		msg,         
        (msg_size + 6) * sizeof(char) );
    								
	free( msg ); 

	//fclose( fd );
 
    return nbytes_written;
}


int avaliable_msg(){

	int has_data = 0;


	pthread_mutex_lock( &mutex );

	if (size > 0 ){

		has_data = 1;

	}

	pthread_mutex_unlock( &mutex );

	return has_data;	

}