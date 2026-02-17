#ifndef LOGPRINT_H
#define LOGPRINT_H

#define LOG
#ifdef LOG
#define LOGPRINT( fmt , ... )    \
        do{ \
            FILE* f = fopen("/tmp/logfifo.txt" , "a" ) ; \
            if( !f )    \
                break ; \
           fprintf(f, "[ %s %d (%s) ]\t",__FILE__,__LINE__,__func__);    \
		   fprintf(f, fmt"\n",##__VA_ARGS__);    \
            fclose( f ) ;   \
        }while( 0 )
#endif

#endif