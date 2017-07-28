SRC=main.c ftp.c
DST=ftp
LIB=-lpthread

$(DST):$(SRC)
	gcc -o $@ $^ $(LIB) 

.PHONY:clean
clean:
	rm -f $(DST)
