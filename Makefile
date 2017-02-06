WORKDIR=.
VPATH = src
OPATH = obj
IPATH = -Iinc -I../fastdfs-5.05/common -I../fastdfs-5.05/tracker -I../libfastcommon-1.0.7/src

CLIENTO = make_log.o client.o
TRACKERO = make_log.o tracker.o
STORAGEO = make_log.o storage.o

CLIENTOBJ = $(patsubst %.o, $(OPATH)/%.o, $(CLIENTO))
TRACKEROBJ = $(patsubst %.o, $(OPATH)/%.o, $(TRACKERO))
STORAGEOBJ = $(patsubst %.o, $(OPATH)/%.o, $(STORAGEO))

CC=gcc
CFLGS= -g
LIBFLAG = -L$(WORKDIR)/lib

BIN = client tracker storage

all:$(BIN)

client:$(CLIENTOBJ)
	$(CC) $^ -o $@  $(CFLGS) $(IPATH) $(LIBFLAG) -lfastcommon -lfdfsclient

tracker:$(TRACKEROBJ)
	$(CC) $^ -o $@ $(CFLGS) $(IPATH) $(LIBFLAG) -lfastcommon -lpthread

storage:$(STORAGEOBJ)
	$(CC) $^ -o $@ $(CFLGS) $(IPATH) $(LIBFLAG) -lfastcommon -lpthread

$(OPATH)/%.o:%.c
	$(CC) $(CFLGS) -c $< -o $@ $(IPATH)

.PHONY:clean
clean:
	-rm -f $(BIN) $(CLIENTOBJ) $(TRACKEROBJ) $(STORAGEOBJ)
