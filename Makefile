LIBS 			= -L/usr/lib -lglfw3 -lGL -lm -lX11 -lXxf86vm -lXrandr -lpthread -lXi
CC				= gcc -g
INC 			= -I/usr/include/GL -I/home/johan/Projects/procedurella/C_GLFW
BIN 			= SWshading
OBJS 			= SWshading.o tnm084_lab1.o triangleSoup.o noise1234.o cellular.o simplexnoise1234.o
LINKOBJS 	= SWshading.o tnm084_lab1.o triangleSoup.o noise1234.o cellular.o simplexnoise1234.o
CFILES		= SWshading.c tnm084_lab1.c triangleSoup.c noise1234.c cellular.c simplexnoise1234.c
RM       	= rm -f

all: SWshading

clean:
	$(RM) $(OBJS) $(BIN)

$(BIN): $(OBJS)
	gcc $(LINKOBJS) $(INC) $(LIBS) -o $(BIN)

SWshading.o: SWshading.c
	$(CC) -c SWshading.c -o SWshading.o $(INC)

tnm084_lab1.o: tnm084_lab1.c
	$(CC) -c tnm084_lab1.c -o tnm084_lab1.o $(INC)

triangleSoup.o: triangleSoup.c
	$(CC) -c triangleSoup.c -o triangleSoup.o $(INC)

noise1234.o: noise1234.c
	$(CC) -c noise1234.c -o noise1234.o $(INC)

cellular.o: cellular.c
	$(CC) -c cellular.c -o cellular.o $(INC)

simplexnoise1234.o: simplexnoise1234.c
	$(CC) -c simplexnoise1234.c -o simplexnoise1234.o $(INC)
