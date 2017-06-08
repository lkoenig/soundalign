
set(SOUNDALIGN_SRC
  main.cpp
  align.cpp
  align.h
  soundfile.cpp
  soundfile.h
  circular_buffer.h
  feature_extractor.h
  feature_extractor.cpp
  feature.h
  )


add_executable(soundalign ${SOUNDALIGN_SRC})
set_property(TARGET soundalign PROPERTY CXX_STANDARD 11)

target_link_libraries(soundalign kiss_fft ${LIBSNDFILE_LIBRARY})
