#
# $Id: make.h,v 1.1.1.1 1996/04/16 18:51:46 gustavof Exp $
#
# Default rules for makefiles
#

.SUFFIXES: .ln .h .c .y .l .c~ .y~ .l~ .h~ .$O .obj .asm .asm~ .C
.SUFFIXES: .a .fm .fm~ .fmo .rp .rp~ .rpo

.c.a :
	$(CC) -c $(CFLAGS) $*.c
	$(AR) rv $@ $(*F).o
	rm $(*F).o

.c.$O :
	$(CC) -c $(CFLAGS) $*.c
	@if [ "$(*F).o" != "$*.$O" ]; then\
		mv $(*F).o $*.$O; \
		echo mv  $(*F).o $*.$O; \
	fi

.c~.$O :
	co -p $< > $*.c
	($(CC) -c $(CFLAGS) $*.c ; rm -f $*.c)
	@if [ "$(*F).o" != "$*.$O" ]; then\
		mv $(*F).o $*.$O; \
		echo mv $(*F).o $*.$O; \
	fi

.C.$O :
	CC -c -DIDEAFIX -g -I$(COMMON)/include -I$(IDEAFIX4)/include $*.C
	@if [ "$(*F).o" != "$*.$O" ]; then\
		mv $(*F).o $*.$O; \
		echo mv  $(*F).o $*.$O; \
	fi

.c.ln :
	lint -c $(CFLAGS) $*.c
	@if [ "$(*F).ln" != "$*.ln" ]; then\
		mv $(*F).ln $*.ln; \
	fi

.c~.ln :
	co -p $< > $*.c
	lint -c $(CFLAGS) $*.c
	rm -f $*.c
	@if [ "$(*F).ln" != "$*.ln" ]; then\
		mv $(*F).ln $*.ln; \
	fi

.h~.h :
	co -p $< > $*.h

.l~.a :
	co -p $< > $*.l
	lex -t $*.l | sed '/^#ident/d' > $*.c
	$(CC) $(CFLAGS) -c $*.c
	rm -f $*.[cl]
	@if [ "$(*F).o" != "$*.$O" ]; then\
		mv $(*F).o $*.$O; \
	fi
	$(AR) rv $@ $(*F).o
	rm $(*F).o

.l.a :
	lex -t $< | sed '/^#ident/d' > $*.c
	$(CC) $(CFLAGS) -c $*.c
	rm -f $*.[c]
	@if [ "$(*F).o" != "$*.$O" ]; then\
		mv $(*F).o $*.$O; \
	fi
	$(AR) rv $@ $(*F).o
	rm $(*F).o

.l.$O :
	lex -t $< | sed '/^#ident/d' > $*.c
	$(CC) $(CFLAGS) -c $*.c
	rm -f $*.[c]
	@if [ "$(*F).o" != "$*.$O" ]; then\
		mv $(*F).o $*.$O; \
	fi

.l.ln :
	lex -t $< | sed '/^#ident/d' > $*.c
	lint -b -c $(CFLAGS) $*.c
	rm -f $*.[c]
	@if [ "$(*F).ln" != "$*.ln" ]; then\
		mv $(*F).ln $*.ln; \
	fi

.l~.$O :
	co -p $< > $*.l
	lex -t $*.l | sed '/^#ident/d' > $*.c
	$(CC) $(CFLAGS) -c $*.c
	rm -f $*.[cl]
	@if [ "$(*F).o" != "$*.$O" ]; then\
		mv $(*F).o $*.$O; \
	fi

.l~.ln :
	co -p $< > $*.l
	lex -t $*.l | sed '/^#ident/d' > $*.c
	lint -b -c $(CFLAGS) $*.c
	rm -f $*.[cl]
	@if [ "$(*F).ln" != "$*.ln" ]; then\
		mv $(*F).ln $*.ln; \
	fi

.y.a :
	yacc $(YFLAGS) $*.y
	mkstatic y.tab.c | sed  '/^#ident/d' > $(*D)/y.tab.c
	$(CC) -c $(CFLAGS) $(*D)/y.tab.c
	-rm -f $(*D)/y.tab.c y.tab.c
	mv y.tab.o $*.$O
	-cmp -s y.tab.h $*.h || mv y.tab.h $*.h
	-rm -f y.tab.[yh]
	$(AR) rv $@ $(*F).o
	rm $(*F).o

.y~.a :
	co -p $< > $*.y
	yacc $(YFLAGS) $*.y
	-rm -f $*.y
	mkstatic y.tab.c | sed  '/^#ident/d' > $(*D)/y.tab.c
	$(CC) -c $(CFLAGS) $(*D)/y.tab.c
	-rm -f $(*D)/y.tab.c y.tab.c
	mv y.tab.o $*.$O
	-cmp -s y.tab.h $*.h || mv y.tab.h $*.h
	-rm -f y.tab.[yh]
	$(AR) rv $@ $(*F).o
	rm $(*F).o

.y.$O :
	yacc $(YFLAGS) $*.y
	mkstatic y.tab.c | sed '/^#ident/d' > $*.c
	$(CC) -c $(CFLAGS) $*.c
	-rm -f $*.c y.tab.c
	-cmp -s y.tab.h $*.h || mv y.tab.h $*.h
	-rm -f y.tab.[yh]
	@-if [ "$(*F).o" != "$*.$O" ]; then\
		mv $(*F).o $*.$O; \
	fi

.y~.$O :
	co -p $< > $*.y
	yacc $(YFLAGS) $*.y
	-rm -f $*.y
	mkstatic y.tab.c | sed  '/^#ident/d' > $(*D)/y.tab.c
	$(CC) -c $(CFLAGS) $(*D)/y.tab.c
	-rm -f $(*D)/y.tab.c y.tab.c
	mv y.tab.o $*.$O
	-cmp -s y.tab.h $*.h || mv y.tab.h $*.h
	-rm -f y.tab.[yh]

.y~.ln :
	co -p $< > $*.y
	yacc $(YFLAGS) $*.y
	-rm -f $*.y
	mkstatic y.tab.c | sed  '/^#ident/d' > $(*D)/y.tab.c
	lint -b -c $(CFLAGS) $(*D)/y.tab.c
	-rm -f $(*D)/y.tab.c y.tab.c
	mv y.tab.ln $*.ln
	-rm -f y.tab.[yh]

.fm~.fmo :
	co -p $< > $*.fm
	fgen $*.fm
	@if [ "$(*F).fmo" != "$*.fmo" ]; then\
		mv $(*F).fmo $*.fmo; \
	fi
	@if [ "$(*F).fmh" != "$*.fmh" ]; then\
		mv $(*F).fmh $*.fmh; \
	fi
	rm -f $*.fm

.fm.fmo :
	fgen $*.fm
	@if [ "$(*F).fmo" != "$*.fmo" ]; then\
		mv $(*F).fmo $*.fmo; \
	fi
	@if [ "$(*F).fmh" != "$*.fmh" ]; then\
		mv $(*F).fmh $*.fmh; \
	fi

.rp~.rpo :
	co -p $< > $*.rp
	rgen $*.rp
	@if [ "$(*F).rpo" != "$*.rpo" ]; then\
		mv $(*F).rpo $*.rpo; \
	fi
	@if [ "$(*F).rph" != "$*.rph" ]; then\
		mv $(*F).rph $*.rph; \
	fi
	rm -f $*.rp

.rp.rpo :
	rgen $*.rp
	@if [ "$(*F).rpo" != "$*.rpo" ]; then\
		mv $(*F).rpo $*.rpo; \
	fi
	@if [ "$(*F).rph" != "$*.rph" ]; then\
		mv $(*F).rph $*.rph; \
	fi

.asm.obj :
	vpix 'masm -Ml $*.asm'
	-mv $(*F).obj $*.obj

.asm~.obj :
	co -p $< > $*.c
	vpix 'masm -Ml $*.asm'
	-mv $(*F).obj $*.obj
	rm -f $*.asm
