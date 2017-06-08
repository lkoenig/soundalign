# Sound Align

## Description
Given two sound files are a time transformation one of each-other (no pitch change).
This small utility will try to provide an alignement of the reference file on the dergaded file. In other word, for each time (sampled of course) on the reference which time correspond on the degraded file.

This utility has been developed with measuring variable delay in Voice over IP telephomy system. 

### Limitation
For now, this small utility only works if:
  * input and output have the same sampling frequency
  * input and output *have to* have only one channel

### Principle
Dynamic time warping


## How to build

### Software Requirements
  * Eigen3 (http://eigen.tuxfamily.org): This is a library for doing linear algebra
  * libsndfile (http://www.mega-nerd.com/libsndfile/): Small library to read audio files
  * cmake
