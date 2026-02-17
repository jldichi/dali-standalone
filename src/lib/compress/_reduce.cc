/* PROGRAM for COMPRESSION */
/* version 2 includes correction
Sara Kratby, Johannes Kepler University Linz */

#define BREAD_IMPLEMENT
#include <ifound/bred.h>
static void shell(long * v,long * pt,long n)
{
	long p,q,x,k,pk ;
	for (k=(n+3)/5 ; k>0 ; k=(k+1)/3)
  		for (p=0 ; p<n-k ; p++ )
    		if ( (x=v[pt[p+k]]) < v[pt[p]] )  {
      			q=p-k ; pk=pt[p+k] ; pt[p+k]=pt[p] ;
      			while ( q>=0 && v[pt[q]]>x )
        		pt[q+k]=pt[q], q-=k ;
      			pt[q+k]=pk ;
			}
}

static long huffgen(long * f,long *len,long n)
{
	long j,m,mlim=0,u=0 ,
	p,q,x,y,z,bits,r=22,
	*pt, *type, cts[50] , *sum ;
	pt = ( long * ) malloc( 4*n+4) ;

	for (j=n-1 ; j>=0 ; j-- )  {
   		if ((x= f[j]))
   			pt[mlim++]=j, u+=x ;
   		len[j]=0 ;
	}

	type = ( long * ) malloc(8*mlim) ;
	sum=type+mlim ;
	shell(f,pt,mlim) ;
	js :
	for (j=0 ; j<50 ; j++) cts[j]=0 ;
	for (m=0 ; m<mlim ; m++) sum[m]=0x7000000 ;
	m=0 ; p=0 ; q=0 ;

	while (q<mlim-1)
  		if ( (m+1<mlim) && ( (x=f[pt[m+1]])<=sum[p] ) )
     		sum[q]=x+f[pt[m]],
     		type[q++]=0,
     		m+=2 ;
  		else if	( (m<mlim) && (sum[p+1]< f[pt[m]]) || (m==mlim) )
     		sum[q]=sum[p]+sum[p+1],
		    type[q++]=2,
     		p+=2 ;
        else sum[q]=sum[p++]+f[pt[m++]],
            type[q++]=1 ;

	bits=0 ;
	for (j=0 ; j<mlim-1 ; j++) bits+=sum[j] ;
	sum[q-1]=0 ;
	while (q) {
		q-- ;
 		x=type[q] ; y=sum[q]+1 ;
 		if (x==2) sum[--p]=y, sum[--p]=y ;
 		else if (x==1)
    		len[pt[--m]]=y, sum[--p]=y, cts[y]++ ;
    	else len[pt[--m]]=y,len[pt[--m]]=y, cts[y]+=2 ;
	}
	y=len[pt[0]]-23 ;
	if (y>0) {
		x=u>>(r--) ; x++ ;
		z=0 ; q=0 ;
		while ( z>=0) {
			y=f[pt[q]] ; f[pt[q++]]=x ;
  			z+=x-y ;
		}
		f[pt[q-1]] -= z ; goto js ;
	}

	x=0 ;
	for (m=49 ; m>=0 ; m--)
   		y=cts[m], cts[m]=x,
   		x+=y, x>>=1 ;
	for (m=n-1 ; m>=0 ; m--)
   		if (len[m])
   			f[m]=cts[len[m]]++ ;
		free( (char *) pt) ;
		free( (char *) type) ;
		return bits ;
}

static void QQS(long *s,long *e,long *ch,long step)
{
	long list[99],zref,zs,ze,*ss,*ee,dd=0, *chd=ch ;
	goto jss ;

jred : if (dd==0) return ;
	chd=(long *) list[--dd] ;
	e=(long *) list[--dd] ;
	s=(long * ) list[--dd] ;
jss :
	if (s+1>=e) {
		long *chdd=chd ;
  		if (e==s) goto jred ;
  			while (chdd[*s]==chdd[*e]) chdd -=step ;
  			if (chdd[*s]<chdd[*e]) goto jred ;
  			zref= *s ; *s= *e ; *e=zref ; goto jred ;
	}

	if ((zs=chd[*s])==(ze=chd[*e])) {
  		ss=s+1 ;
  		while ( (chd[*ss]==ze) && (ss!=e) ) ss++ ;
  		if (ss==e) {
			chd -=step ; goto jss ;
		}
  		else zs=chd[*ss] ;
	}
	zref=(zs>>1)+(ze>>1)+(ze&zs&1) ;
	ss=s ; ee=e ; goto js2 ;

jss1 :
	zs= *ss ; *ss++ = *ee ; *ee-- =zs ;
js2 :
	while (zref>=chd[*ss]) ss++ ;
	while (zref<chd[*ee]) ee-- ;
	if (ss<ee) goto jss1 ;

	ss-- ; ee++ ;
	if (s-ss)
  		if (e-ee)
     		if (ss-s>e-ee) {
        		list[dd++]=(int) s ;
        		list[dd++]=(int) ss ;
        		list[dd++]=( long ) chd ;
        		s=ee ; goto jss ;
			}
    	 	else {
        		list[dd++]=(int) ee ;
        		list[dd++]=(int) e ;
    	    	list[dd++]=( long ) chd ;
	        	e=ss ; goto jss ;
			}
  		else {
				e=ss ; goto jss ;
		}
	else if (e-ee) {
			s=ee ; goto jss ;
	}
    else
		goto jred ;
}

/*  MAIN PRORAM */
long reduce(char * inbuf,long in_len, char **outbuf, long *outbuf_len, unsigned char complevel)
{
	X_Stream in(in_len,inbuf,in_len);
	X_Stream out(*outbuf_len,*outbuf);
	int buflen = (in_len * (complevel+1))/40;
	buflen += (1000-buflen%1000);
	long *buf = new long[buflen << 1];
	if(!buf)
		return -1;
	int res = _reduce(in,out,buf,buflen);
	delete [] buf;
	*outbuf=(char *)out.getbuf();
	*outbuf_len=out.length();
	return res;
}

long _reduce (X_Stream &fpr,X_Stream &fpw, long *buff, int buflen)
{
/* read file */
	long outct, ctch, dct ;
	long  x,y,z,z1,m,n,p,q;
	long  *bp, *pt;
	long bsize;
// Assign !!!!
	bsize = buflen;
	bp = buff;


	bp+=32 ; bsize-=48 ;
   	pt=bp+bsize+32 ;

#define VM(z) { x=x>>8&0x00ffffff ; x+=(z)<<24 ;\
 tf[z]++ ; bp[m++]=x^0x80000000 ; }

			long *tf, tff[260], blim=bsize-2, mlim ;
			ctch=0 ; outct=0 ; dct=0 ; x=0;

jstart :
			VS(tff,0,260);
			VS(bp-32,0x80000000,33);
			z= -512; m=1 ; y=0 ; tf=tff+2 ; x=0 ;
j1 :
			z1=z ;
			z=fpr.getc() ;
			if (z==EOF) goto j2 ;
			if (m>blim) {fpr.ungetc(z) ; goto j2 ; }
			VM(z) ctch++ ;
			if (z^z1) goto j1 ;
			while ( (z=fpr.getc()) ==z1 ) y++ ;
			ctch+=y+1 ;
			while (y) {
				y-- ;
       			VM(z1^y&1) y=y>>1 ; }
				if ((z^z1)<3) VM(z1^2)
				if (z^EOF) { VM(z) goto j1 ;  }

j2 :
				mlim=m ; tff[0]++ ;

				for ( m= -1 ; m<256 ; m++) tf[m]+=tf[m-1] ;
				tf-- ;
				pt[0]=0 ;

				for (m=1 ; m<mlim ; m++ ) z=(bp[m]>>24)+128&255,
    			pt[tf[z]++]=m ;

/* form pointers to groups size 2 or greater,
then sort each group in size order and set the
back pointer to enhance following sorts    */

		{
					long d[260],jp[260],q=0, k,j ;
					tf-- ;
					m=1 ;
					while ((unsigned long)bp[m]==0x80000000) tf[0]++, m++ ;
					for (j=0 ; j<256 ; j++ )
  						if ( (x=(tf[j+1]-tf[j])) >1 )
    						d[j]=x, jp[q++]=j ;
							shell(d,jp,q) ;
							for (p=0 ; p<q ; p++ ) {
    							j=jp[p] ;
    							QQS(pt+tf[j],pt+tf[j+1]-1,bp-1,4) ;
    							for (m=tf[j] ; m<tf[j+1] ; m++ )
    								bp[pt[m]] = bp[pt[m]]&0xff000000 ^ m+32-tf[j] ;

            				}
		}

/* For the 'next' or predicted values
 construct move to front values and
1-2 zero string coding. insert coding
in bp[pt]+1] */
		{
#define OUT while (bfct>7) { \
  fpw.putc(bff>>bfct-8) ; outct++ ; bfct-=8 ; }
#define PUT(a,b) { bff=bff<<(b)^(a) ; bfct+=b ; }

			long r[257], /* used for move to front */
			trel[260],   /* holds relative frequencies, then codes */
			len[260],    /* holds code lengths */
			s=0,j ,k=0 , bff=0, bfct=0 ;

			VSset(r,257); /* set char list for move to front */
			VS(trel,0,260); /* zero frequencies */
			for (j=0 ; j<mlim ; j++ )  {
				z=bp[x=pt[j]+1]>>24 &255^128 ;
				if (x==mlim) z=256 ;
				p=0 ;
				while (z^r[p]) p++ ;
				q=p ;
				while (p)  r[p]=r[p-1], p--  ;
				r[0]=z ;

				if (q==0) s++ ;
				else {
					while (s)  {
    					s-- ;
 						trel[s&1]++ ;
							pt[k++]=s&1 ;
							s>>=1 ;
 					}
						trel[q+1]++ ;
						pt[k++]= q+1 ;
 				}
 			}
 			while (s)  {
					s-- ;
					trel[s&1]++ ;
					pt[k++]=s&1 ;
					s>>=1 ;
 			}

/* generate code and length table for
encoding  */
			trel[258]=1 ; /* make eof smallest code */
			y=huffgen(trel,len,259) ;

/* output the code length table
 base 4 abreviations for repeated values
or zeros, first nibble is not zero */

			dct-=outct+outct ;
			y=1 ;
			for (m=0,n=0,p=0 ; m<259 ; m++)  {
				z=len[m] ;
				if ( z==0 || z^y )
  				while (n)    {
     				n-- ;
     				PUT(8^n&3,4) OUT
     				n>>=2 ;
				}
				if (z) while (p)  {
   					p-- ;
   					PUT(12^p&3,4) OUT
   					p>>=2 ;
				}
				else {
					p++ ; continue ;
				}
				if (y==z) {
					n++ ; continue ;
				}
				x=z-y+3 ;
				if (x<0) x+=23 ;
				if (x>22) x-=23 ;
				if (x<7) {
					PUT(x+1,4) OUT
    				y=z ; continue ;
				}
				PUT(4,4) PUT(x-7,4) OUT
				y=z ;
			}

			while (n)    {
   				n-- ;
   				PUT(8^n&3,4) OUT
   				n>>=2 ;
			}
			dct +=outct+outct+(bfct>>2) ;
/* output coded values */
			for (m=0 ; m<k ; m++) {
				z=pt[m] ;
				PUT(trel[z],len[z]) OUT
			}

			PUT(trel[258],len[258]) OUT
			PUT(0,16-bfct) OUT

		}

/* check for file completion or
another block */

		z=fpr.getc() ;
		if (z^EOF) { fpr.ungetc(z) ; goto jstart ; }

		return outct;
}

