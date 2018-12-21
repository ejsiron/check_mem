OBJS = map_positivepowerprefix.o sanitize_numericinput.o check_mem.o
CC = c++
DEBUG = -g0
CXXFLAGS = -Wall -fomit-frame-pointer -std=c++11 -c
LFLAGS = -Wall
OUT = check_mem
INSTALLDIR = /usr/local/nagios/libexec/
BUILDSTAT = stat $(out) 2>/dev/null | grep Modify
INSTALLEDSTAT = stat $(INSTALLDIR)$(OUT) 2>/dev/null | grep Modify
TARBALL = $(OUT).tar.gz

all : clean $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(OUT)
	
map_positivepowerprefix.o : map_positivepowerprefix.cpp
	$(CC) $(CXXFLAGS) map_positivepowerprefix.cpp
	
sanitize_numericinput.o : sanitize_numericinput.cpp
	$(CC) $(CXXFLAGS) sanitize_numericinput.cpp
	
check_mem.o : check_mem.cpp
	$(CC) $(CXXFLAGS) check_mem.cpp
	
clean :
	\rm -f *.o $(OUT) *.tar.gz
	
install : $(OUT)
	@if [ "$(BUILDSTAT)" != "$(INSTALLEDSTAT)" ];\
	then\
		cp -p $(OUT) $(INSTALLDIR);\
	fi

uninstall : $(INSTALLDIR)$(OUT)
	\rm $(INSTALLDIR)$(OUT)

tar :
	\rm -f $(TARBALL)
	tar zcvf $(TARBALL) --transform 's%^%$(OUT)/%' *.cpp *.h makefile README LICENSE