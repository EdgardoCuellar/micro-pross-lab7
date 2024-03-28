make:
	gcc -o Main.o Main.c
	./Main.o 127

convert:
	python3 0_raw_img_displayer.py

clean:
	rm -f images_readable/*.png
	rm -f output/*.raw
	rm -f *.raw
	rm *.o