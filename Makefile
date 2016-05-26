#*============================----beg-of-source---============================*#

BASE    = _lib

#*---(standard variables)-------------*#
COMP    = gcc -std=gnu89 -c -g -pg -Wall -Wextra
LINK    = gcc
COPY    = cp -f
CLEAN   = rm -f
ECHO    = echo
PRINTF  = @printf

all                : ${BASE} _inst


${BASE}            : ${BASE}.o
	${LINK}  -o ${BASE} ${BASE}.o

${BASE}.o          : ${BASE}.c
	${PRINTF} "\n#---((make library installer))------------\n"
	${COMP}  _lib.c

_inst              : 
	${PRINTF} "\n#---((make program installer))------------\n"
	cp -f _inst.sh    _inst

clean              :
	${PRINTF} "\n#---((clean working files))---------------\n"
	${CLEAN}  *.o
	${CLEAN}  *~
	${CLEAN}  temp*
	${CLEAN}  ${BASE}

remove             :
	${PRINTF} "\n#---((remove existing versions))----------\n"
	${CLEAN}          /usr/local/sbin/_inst
	${CLEAN}          /usr/local/sbin/${BASE}
	

install            :
	${PRINTF} "\n#---((install current versions))----------\n"
	mv -f _inst       /usr/local/sbin/
	chown root:root   /usr/local/sbin/_inst
	chmod 0755        /usr/local/sbin/_inst
	cp -f ${BASE}   /usr/local/sbin/
	chown root:root /usr/local/sbin/${BASE}
	chmod 0755      /usr/local/sbin/${BASE}
	chmod +s        /usr/local/sbin/${BASE}


#*============================----end-of-source---============================*#
