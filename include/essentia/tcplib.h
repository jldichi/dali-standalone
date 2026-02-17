/***************************************************************************
* File TCPLIB.H
* Copyright (C) 1991, Trustees of Columbia University in the
*  City of New York.  Permission is granted to any individual or
*  institution to use, copy, or redistribute this software as long as
*  it is not sold for profit and this copyright notice is retained.
*
* The C code is designed for the small memory model of Microsoft C versions
* 5.1 and 6.00A, with structures packed on one byte boundaries. No other
* options are expected.
*
* Last edit:
* 6 Sept 1991
*
* Modified by Intersoft Argentina (C) 1994,
* Ported to 32 bit Symantec C
*
* Last edit 17 Feb 1994
*
*  $Id: tcplib.h,v 1.1.1.1 1996/04/16 18:51:48 gustavof Exp $
***************************************************************************/

#ifndef TCPLIB_H_
#define TCPLIB_H_
/* Typedefs and constants */

#undef byte
#undef word
#undef longword

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  longword;

typedef int (*procref)();
typedef byte eth_address[6];

#define ETH_MSS 536              /* MSS for Ethernet */
#define tcp_MaxBufSize 1024      /* max bytes to buffer in a tcp socket */
#define udp_MaxBufSize 534       /* max bytes to buffer in a udp socket */
#define MAX_GATE_DATA 2
#define MAX_NAMESERVERS 3
#define MAX_COOKIES 0

/* These are Ethernet protocol numbers but are used for other things too */
#define UDP_PROTO  0x11
#define TCP_PROTO  0x06
#define ICMP_PROTO 0x01

#define TCP_MODE_BINARY  0         /* default mode */
#define TCP_MODE_ASCII   1
#define TCP_MODE_NAGLE   0         /* Nagle algorithm */
#define TCP_MODE_NONAGLE 4
#define UDP_MODE_CHK     0         /* default to having checksums */
#define UDP_MODE_NOCHK   2         /* turn off checksums */

/* Error status */

#define TCP_INIT		0x01		/* Error de inicializacion */
#define TCP_RESOLVE     0x02		/* Cannot resolve addres */
#define TCP_HOSTDOWN    0x03		/* Host is down or gateaway is needed */
#define TCP_SOCKMODE    0x04		/* Unable to set sock mode */
#define TCP_INCOME	    0x05        /* Incoming session */
#define TCP_MYADDR		0x06		/* Unknown my address */
#define TCP_CONFIG		0x07		/* Can't open config file */
#define TCP_USER        0x08		/* No user name defined in tcp.ini*/
#define TCP_UNAME       0x09		/* No uname defined in tcp.ini */

#define TCP_CHECKSUM    0x10        /* Bad checksum received */
#define TCP_RESET       0x11		/* Tcp reset */
#define TCP_PDATT       0x12		/* Cannot attach to PD */
#define TCP_PDINFO		0x13		/* Cannot obtain PD info */
#define TCP_ARP     	0x14		/* Cannot access ARP packet type */
#define TCP_RARP        0x15		/* Cannot access RARP packet type */
#define TCP_IP          0x16        /* Cannot access IP packet type */
#define TCP_ARPR        0x17        /* Error releasing ARP packet type */
#define TCP_RARPR       0x18		/* Error releasing RARP packet type */
#define TCP_IPR         0x19		/* Error releasing IP packet type */
#define TCP_NOSERVER    0x1a		/* No nameserver found */

#define TCP_UNKNOWN		0x3f		/* Unknown error */

/* TCP Socket definition */

#pragma SC align 1
typedef struct tcp_socket {
	struct tcp_socket *next;
	word                    ip_type;                /* always set to TCP_PROTO */
	byte               *err_msg;
	void              (*usr_yield)();
	word                    soc_mode;          /* a logical OR of bits */

	longword                usertimer;        /* ip_timer_set, ip_timer_timeout */
	procref          dataHandler;   /* called with incoming data */
	eth_address      hisethaddr;     /* ethernet address of peer */
	longword                hisaddr;                /* internet address of peer */
	word                    hisport;                /* tcp ports for this connection */
	word                    myport;

	int                      rdatalen;         /* must be signed */
	byte                    rdata[tcp_MaxBufSize];    /* received data */
	word                    rmaxdatalen;    /* normally tcp_MaxBufSize */
	word                    state;            /* connection state */

	longword                acknum;
	longword                seqnum;          /* data ack'd and sequence num */
	long                    timeout;                /* timeout, in milliseconds */
	byte                    unhappy;                /* flag, indicates retransmitting segt's */
	word                    flags;            /* tcp flags word for last packet sent */

	word                    window;          /* other guy's window */
	int                      datalen;               /* number of bytes of data to send */
									/* must be signed */

	byte                    cwindow;                /* Van Jacobson's algorithm */
	byte                    wwindow;

	word                    vj_sa;            /* VJ's alg, standard average */
	word                    vj_sd;            /* VJ's alg, standard deviation */
	longword                vj_last;                /* last transmit time */
	word                    rto;
	byte                    karn_count;      /* count of packets */

/* retransmission timeout proceedure, these are in PC clock ticks */
	longword                rtt_lasttran;      /* last transmission time */
	longword                rtt_smooth;              /* smoothed round trip time */
	longword                rtt_delay;                /* delay for next transmission */
	longword                rtt_time;                  /* time of next transmission */
	word                    mss;
	byte                    data[tcp_MaxBufSize];      /* data to send */
} tcp_Socket;
/* UDP socket definition */
typedef struct udp_socket {
	struct udp_socket *next;
	word                    ip_type;                        /* always set to UDP_PROTO */
	byte               *err_msg;                    /* null when all is ok */
	void              (*usr_yield)();
	word                    soc_mode;                  /* a logical OR of bits */
	longword                usertimer;                /* ip_timer_set, ip_timer_timeout */
	procref          dataHandler;
	eth_address      hisethaddr;             /* peer's ethernet address */
	longword                hisaddr;                        /* peer's internet address */
	word                    hisport;                        /* peer's UDP port */
	word                    myport;
	int                      rdatalen;                 /* must be signed */
	byte                    rdata[udp_MaxBufSize];
} udp_Socket;


/* sock_type used for socket io */

typedef union {
	udp_Socket udp;
	tcp_Socket tcp;
} sock_type;



#pragma SC align __DEFALIGN


/*
 * sock_wait_established()
 *        - waits then aborts if timeout on s connection
 */

#define sock_wait_established(s, seconds, fn, statusptr) \
	if (ip_delay0(s, seconds, fn, statusptr)) goto sock_err;

/* Function prototypes */

#ifdef __cplusplus
extern "C" {
#endif
	sock_read(sock_type *s, byte *dp, int len);
	sock_fastread(sock_type *s, byte *dp, int len);
	sock_write(sock_type *s, byte *dp, int len);
	sock_fastwrite(sock_type *s, byte *dp, int len);
	sock_flush(sock_type *s);
	sock_flushnext(sock_type *s);
	sock_puts(sock_type *s, byte *dp);
	word sock_gets(sock_type *s, byte *dp, int n);
	sock_putc(sock_type *s, byte c);
	sock_getc(sock_type *s);
	word sock_dataready(sock_type *s);
	sock_close(sock_type *s);
	void sock_abort(tcp_Socket *s);
	sock_printf(sock_type *s, byte *format, ...);
	sock_scanf(sock_type *s, byte *format, ...);
	sock_mode(tcp_Socket *s, word mode);  /* see TCP_MODE_... */

	int				tcp_tick(sock_type *s);
	int 			tcp_init(void);
	int 			ip_delay0(sock_type *, int, procref, int *);
	int     	tcp_Init();
	void    	tcp_shutdown();
	int      	tcp_abort(tcp_Socket *s);
/*
 * TCP or UDP specific material, must be used for open's and listens, but
 * sock calls are used for everything else.
 */
	int udp_open(udp_Socket *s, word lport, longword ina, word port,int (*datahandler)());
	int tcp_open(tcp_Socket *s, word lport, longword ina, word port,int (*datahandler)());
	int tcp_listen(tcp_Socket *s, word lport, longword ina, word port,int (*datahandler)(), word timeout);
	int tcp_established(tcp_Socket *s);

	int      add_server(int *counter, int max, longword *array, longword value);
	longword 	resolve(byte *name);
/* Extern variables */

	extern  int last_nameserver;
	extern  longword def_nameservers[MAX_NAMESERVERS];
	extern  longword my_ip_addr;
	extern  longword sin_mask;
	extern  int tcp_errno;
	extern byte *def_domain;
	int __fddebug;
#ifdef __cplusplus
}
#endif

#endif

