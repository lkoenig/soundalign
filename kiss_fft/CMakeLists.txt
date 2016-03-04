
set(KISS_FFT_SRC
	kiss_fft.c
	kiss_fft.h
	kiss_fftr.c
	kiss_fftr.h
	_kiss_fft_guts.h
)


add_library(kiss_fft ${KISS_FFT_SRC})

