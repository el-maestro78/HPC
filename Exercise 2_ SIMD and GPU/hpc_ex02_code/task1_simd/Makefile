all: force1d_serial force1d_serial_opt force1d_auto_vect force1d_avx

force1d_serial: force1d-ini.c
	gcc -o force1dserial force1d-ini.c

force1d_serial_opt: force1d-ini.c
	gcc -Ofast -o force1d_serial_opt force1d-ini.c

force1d_auto_vect: force1d-ini.c
	gcc -Ofast -ftree-vectorizer-verbose=6 -o force1d_auto_vect force1d-ini.c

force1d_avx: force1d.c
	gcc -Ofast -mavx -o force1d_avx force1d.c -fopt-info

.PHONY:
clean:
	rm -f force1d_serial force1d_serial_opt force1d_auto_vect force1d_avx
