COMPILE = CC -c -O   # Command to compile a module from .c to .o
LINK =    CC        # Command to link a program


myldpc:	channel.o      \
        matrix.o    \
        ldpc.o         \
        msg.o          \
        main.o
	$(LINK) channel.o matrix.o ldpc.o msg.o  main.o -lsunmath  -o ldpc.run


channel.o: channel.cpp
	$(COMPILE) channel.cpp
matrix.o: matrix.cpp
	$(COMPILE) matrix.cpp
ldpc.o: ldpc.cpp
	$(COMPILE) ldpc.cpp
msg.o:  msg.cpp
	$(COMPILE) msg.cpp
main.o: main.cpp
	$(COMPILE) main.cpp
	
