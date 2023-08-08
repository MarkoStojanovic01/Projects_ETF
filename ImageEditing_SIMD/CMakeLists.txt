cmake_minimum_required(VERSION 3.25)
project(aor2Projekat)

set(CMAKE_CXX_STANDARD 17)

add_executable(aor2Projekat Header/stb_image.h Cpp/main.cpp Header/stb_image_write.h Cpp/Image.cpp Header/Image.h Header/avx_mathfun.h)

# Enable SSE4.1,SSE4.2 and AVX instructions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1 -msse4.2 -mavx -mavx2")
