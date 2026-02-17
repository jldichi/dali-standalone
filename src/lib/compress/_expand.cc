/* EXPANSION ROUTINE */
#define BREAD_IMPLEMENT
#include <ifound/bred.h>

/* MAIN ROUTINE */
long expand(char * inbuf,long in_len, char **outbuf, long *outbuf_len, unsigned char complevel)
{
// warning !!! outbuf must be new 'ed directoly because I can realloc it !!!
	X_Stream in(in_len,inbuf,in_len);
	X_Stream out(*outbuf_len,*outbuf);
	int buflen = ((complevel +1 ) *in_len) /10,ratio =2;
	buflen += (1000-buflen%1000);
	while(1) {
		long *buf = new long[buflen+16];
		if(!buf)
			return -1;
		int res = _expand(in,out,buflen+16,buf,ratio);
		delete [] buf;
		in.rewind();
		out.rewind();
		switch(res) {
			case -3:
				buflen *= 2;
				break;
			case -6:
				ratio *= 2;
				break;
			default:
				*outbuf=(char *)out.getbuf();
				*outbuf_len=out.length();
				return res;
				break;
		}
	}
}

long _expand(X_Stream &fpr,X_Stream &fpw,long bsize,long *v,long ratio)
{
	long zn, x=0;
	long y,z,z1,m,n,p ;
		while (1){
#define RD while (bfct<max8) { z=fpr.getc() ;\
if (z==EOF) return -1 ;  bff=(bff<<8)+z ; bfct+=8 ; }

			long bff=0, bfct=0, k, s,
 			max,max8=8, mask, ysum=ratio*bsize,

			len[260],   /* code length table */
			cts[25],    /* counts of lengths, code starts */
			code[260],   /* gives codes for lengths */
			tab[25],  /* start codes for lengths, left aligned */
			tabch[25],    /* starts in output list for length */
			tabval[260],  /* output list ordered by code */
			r[260],      /* move to front table */
			val8[260],    /* fast value table  */
			len8[260],  /* fast length table, indexed by nextcode */
			tf[260] ;   /* frequency table to restore original order */


			bff=0 ; bfct=0 ;
			VS(len,0,260);
			VS(cts,0,25);
			VSset(r,257);
			VS(tab,0,25);
			VS(tabch,0,25);
			VS(tabval,0,260);
			VS(len8,9,260);
			VS(tf,0,260);
/* generate length table len, chain
same lengths, start gives chain starts,
cts has number of each length */

			{
				long chain[260], /* chains same lengths */
					start[260], /* starts of chains */
 					x=1 , s=0, t=0 ;
					VS(start,-1,260);

				for ( m=0 ; m<259 ; )   {
  					RD
  					z=bff>>bfct-4&15 ; bfct-=4;
  					if (z>11) {
						m+= (z-11)<<s ;
        				s+=2 ; t=0 ;
        				continue ;
					}
  					if (z>7)  {
						s=0 ;
    					for ( p=z-7<<t ; p>0 ; p-- ) {
       						len[m]=x ;
       						cts[x]++ ;
       						chain[m]=start[x] ;
       						start[x]=m++ ;
						}
    					t+=2 ;
    					continue ;
	        		}
  					if (z^4) z=x+z-4 ;
  					else {
						z=bff>>bfct-4&15 ;
    					bfct-=4 ;
    					z+=x+4 ;
    				}
		  			if ( z>23 ) z-=23 ;
  					if ( z<1 )  z+=23 ;
  					x=len[m]=z ;
  					s=0 ; t=0 ;
  					cts[z]++ ;
  					chain[m]=start[z] ;
  					start[z]=m++ ;
        		}

/* generate the decoding tables */

				max= len[258] ;if (max>8) max8=max ;
				mask=1<<max; mask-- ;
				x=0 ;
				for (m=max ; m>=0 ; m--) {
					if (x&1)
         				return -2 ;
					y=cts[m] ;
					cts[m]=x>>=1 ;
					x+=y ;
				}

				k=0 ;
				for (m=max ; m>0 ; m--)  {
  					x=tab[m]=cts[m]<<max-m ;
  					z=start[m] ;
  					tabch[m]=k ;
  					while (z+1)
    					code[z]=cts[m]++,
    					tabval[k++]=z,
    					z=chain[z] ;
					}
					for ( m=0 ; m<259 ; m++)
   						if ( (x=len[m] ))
   							if ( x<9 )
      						for ( n=0, y=code[m]<<8-x ; n<1<<8-x ; n++ )
         						val8[y+n]=m, len8[y+n]=x ;
			}

/* output cycle to decode, remove relative coding
detect end, and list restored values */

			y=0 ; k=0 ; s=0 ; z1=0 ;
jo :
			RD
			z=bff>>bfct-8&255 ;
			if ( (m=len8[z]) <9 )   {
  				z=val8[z] ; bfct-=m ;
            }
			else  {
  				x=bff>>bfct-max & mask ;
  				while (x<tab[m]) m++ ;
  				x-=tab[m] ;
  				x>>=max-m ;
  				x+=tabch[m] ;
  				bfct-=m ;
  				z=tabval[x] ;
      		}

/* z in range 0-258, 01 give repeats */

			if (z<2) {
   				y+=z+1<<s ;
   				s++ ;
   				goto jo ;
			}
			if (k+y>bsize)
            	return -3 ;
			while (y)  y--,
   				v[k++]=z1<<23^tf[z1]++ ;
			if (z<258) {
				z-- ; p=z ; z1=r[p] ;
				while (p) r[p]=r[p-1], p-- ;
				r[0]=z1 ;
				s=0 ;
				v[k++]=z1<<23^tf[z1]++ ;
   				goto jo ;
			}
			if (tf[256]^1)
     			return -4 ;
/* unravel v and unfilter output */

			n=0 ; z=1 ; s=0 ; k-- ;
			for (m=0 ; m<256 ; m++)
				y=tf[m], tf[m]=z, z+=y ;
			y=0 ; z1= -2 ;
			while ( k-->0 ) {
				x=z1 ;
				z=v[n] ;
				z1=z>>23 &511 ;
				fpw.putc(z1) ;
				n=(z&0x007fffff) + tf[z1] ;
				if (x^z1) continue ;

je :
				z=v[n] ;
				z1=z>>23 &511 ;
      			n=(z&0x007fffff) + tf[z1] ;
      			k-- ;
      			if (k<0)
               		return -5 ;
    			if ( (z1^x)<2) {
    				y+=(z1^x)+1<<s++ ;
        			goto je ;
				}
      			else {
					ysum-=y ;
					if (ysum<0)
						return -6 ;
         			while (y) {
						fpw.putc(x) ; y-- ;
					}
    			}
      			if ( (z1^x)==2 )
         			z1= -2 ;
      			else {
					 fpw.putc(z1) ;
				}
				s=0 ;
			}

/* unless EOF get next part block */

			while ( (z=fpr.getc())==0) ;
			if (z==EOF) break ;
			fpr.ungetc(z) ;
		}
		return 0;
}

static char *errtab[] = {	"No error",
					"End of file too early " ,
					"False decode tables",
					"Blocksize too small" ,
					"Not exactly one end of text in block" ,
					"Data wrong, too long",
					"Too many repeats, faulty data else increase option r",
					"Unknown error"

			};

const char *expand_err(int err)
{
		if(err <0) err = -err;
		return errtab[err > MAX_EXP_ERR ? MAX_EXP_ERR: err];
}
