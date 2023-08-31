CC = g++
CFLAGS = `wx-config --cxxflags` -Wno-c++11-extensions -std=c++11
CLIBS = `wx-config --libs` -Wno-c++11-extensions -std=c++11
CLIBSTC = `wx-config --libs stc` -Wno-c++11-extensions
OBJ = preview.o settings.o paletteView.o textView.o imageView.o hexView.o gfxView.o file.o

preview: $(OBJ)
	$(CC) -o preview $(OBJ) $(CLIBS) $(CLIBSTC)

preview.o: preview.cpp preview.h
	$(CC) -c preview.cpp $(CFLAGS)

paletteView.o: paletteView.cpp paletteView.h
	$(CC) -c paletteView.cpp $(CFLAGS)

textView.o: textView.cpp textView.h
	$(CC) -c textView.cpp $(CFLAGS)

imageView.o: imageView.cpp imageView.h
	$(CC) -c imageView.cpp $(CFLAGS)

hexView.o: hexView.cpp hexView.h
	$(CC) -c hexView.cpp $(CFLAGS)

gfxView.o: gfxView.cpp gfxView.h
	$(CC) -c gfxView.cpp $(CFLAGS)

settings.o: settings.cpp preview.h
	$(CC) -c settings.cpp $(CFLAGS)

file.o: file.cpp file.h
	$(CC) -c file.cpp $(CFLAGS)

.PHONY: clean
clean:
	-rm preview $(OBJ)
