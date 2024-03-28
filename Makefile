make:
	gcc -o run.o main_c.c -w
	gcc -o run_SIMD.o main_SIMD.c -w

	./run.o
	python3 0_raw_img_displayer.py

clean:
	rm -f images_readable/*.png
	rm run*.o