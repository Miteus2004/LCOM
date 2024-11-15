# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/Controlers/
.PATH: ${.CURDIR}/Controlers/Keyboard
.PATH: ${.CURDIR}/Controlers/Graphic
.PATH: ${.CURDIR}/Controlers/Timers
.PATH: ${.CURDIR}/Controlers/Mouse
.PATH: ${.CURDIR}/Controlers/RTC
.PATH: ${.CURDIR}/GameControlers
.PATH: ${.CURDIR}/xpm
.PATH: ${.CURDIR}/xpm/numbers
.PATH: ${.CURDIR}/xpm/letters


# source code files to be compiled 
SRCS = 	main.c timer.c utils.c graphics.c KBC.c keyboard.c mouse.c rtc.c aimTrainControler.c gamehandlers.c statecontroler.c gamesprites.c sprites.c buffering.c reactgamecontroller.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>