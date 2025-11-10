cmake_minimum_required(VERSION 3.31)
project(MusicPlayer C)

set(CMAKE_C_STANDARD 11)
set(INCLUDE_DIRECTORIES ./)
include_directories(src)

add_executable(MusicPlayer src/main.c
        src/utils/utils.c
        src/utils/utils.h
        src/colors/colors.h
        src/songs/songs.c
        src/songs/songs.h
        src/albums/albums.c
        src/albums/albums.h
        src/albums/albums.c
        src/albums/albums.h
        src/lib/miniaudio/miniaudio.c
        src/lib/miniaudio/miniaudio.h
        src/interface/interface.c
        src/interface/interface.h
        src/playlist/playlist.c
        src/playlist/playlist.h
        src/audio_controller/audio_controller.c
        src/audio_controller/audio_controller.h
        src/command_log/command_log.c
        src/command_log/command_log.h)
