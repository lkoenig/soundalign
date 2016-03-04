
set(SOUNDALIGN_SRC
  main.cpp
  soundfile.cpp
  soundfile.h
  circular_buffer.cpp
  circular_buffer.h
  feature_extractor.h
  feature_extractor.cpp
  feature.h
  )


add_executable(soundalign ${SOUNDALIGN_SRC})
target_link_libraries(soundalign kiss_fft ${LIBSNDFILE_LIBRARY})
