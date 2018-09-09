add_executable(client
               main.cpp
               options.cpp
               options.hpp
               ../texture.cpp
               ../texture.hpp
               ../window.cpp
               ../window.hpp
               ../packet.cpp
               ../packet.hpp
               ../codecs/convert.cpp
               ../codecs/convert.hpp
               ../codecs/ffmpeg/decoder.cpp
               ../codecs/ffmpeg/decoder.hpp
               ../channels/bounded.hpp
               ../processors/processor.hpp
               ../processors/processor.cpp
               ../processors/packet_receiver.cpp
               ../processors/packet_receiver.hpp
               ../processors/h264decoder.cpp
               ../processors/h264decoder.hpp
               ../processors/frame_display.cpp
               ../processors/frame_display.hpp
               ../primitives/image.cpp
               ../primitives/image.hpp
               )

target_include_directories(client
                           PRIVATE ..
                           PRIVATE SYSTEM ${CONAN_INCLUDE_DIRS_GLFW}
                           PRIVATE SYSTEM ${CONAN_INCLUDE_DIRS_BOOST}
                           PRIVATE SYSTEM ${CONAN_INCLUDE_DIRS_SPDLOG}
                           )

target_link_libraries(client
                      ${OPENGL_gl_LIBRARY}
                      ${CONAN_LIBS_GLFW}
                      ${CONAN_LIBS_FFMPEG}
                      ${CONAN_LIBS_LIBX264}
                      ${CONAN_LIBS_BOOST}
                      ${CONAN_LIBS_FMT}
                      )

target_compile_options(client PRIVATE ${ENABLED_WARNINGS})
