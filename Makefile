make:
	gcc -o run_V1.o main_c_V1.c -w
	gcc -o run_V2.o main_c_V2.c -w
	gcc -o run_SIMD.o main_SIMD.c -w

	./run_V1.o
	python3 0_raw_img_displayer.py V1
	rm -f output/*.raw

	./run_V2.o
	python3 0_raw_img_displayer.py V2
	rm -f output/*.raw
	
clean:
	rm -f images_readable/*.png
	rm run*.o