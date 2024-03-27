CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 -g
TFLAGS=$(shell pkg-config --cflags --libs check) --coverage -lm

.PHONY: all install uninstall dist dvi test style gcov_report rebuild clean

all: install

install:
	mkdir s21_3D_viewer
	$(CC) $(CFLAGS) $(shell pkg-config --cflags gtk4 MagickWand) -o 3D_viewer *.c $(shell pkg-config --libs gtk4 MagickWand) -lm
	mv 3D_viewer s21_3D_viewer/

uninstall:
	rm -rf s21_3D_viewer/

dist:
	tar -zcvf s21_3D_viewer.tar.gz s21_3D_viewer/

dvi:
	open README.html

tests:
	$(CC) $(CFLAGS) -o tests Tests/*.c $(TFLAGS) -g
	./tests

style:
	clang-format -i Tests/*.c Tests/*.h *.c *.h

gcov_report: tests
	lcov -t "s21_3D_viewer" -o s21_3D_viewer.info -c -d . 
	genhtml -o report s21_3D_viewer.info 

rebuild: clean tests

clean:
	rm -rf report/ s21_3D_viewer/ tests *.gzip *.tar *.gcno *.gcda *.info *.jpeg *.bmp *.gif 2>/dev/null
