#include "lib/miniaudio/miniaudio.h"
#include <stdio.h>
#include <stdlib.h>

ma_result result;
ma_engine engine;
ma_sound sound;
int init=0;

void init_audio() {
    ma_engine_init(NULL,&engine);
}

void uninit_audio() {
    ma_sound_stop(&sound);
    ma_engine_uninit(&engine);
}

int load_song(char *path) {
    if (init) ma_sound_uninit(&sound);
    result=ma_sound_init_from_file(&engine,path,0,NULL,NULL,&sound);
    init=1;
    if (result!=MA_SUCCESS) {
        return 1;
    }
    ma_sound_set_looping(&sound,0);
    return 0;
}

void stop_playing() {
    ma_sound_stop(&sound);
}

int is_playing() {
    return (int)ma_sound_is_playing(&sound);
}

void toggle_pause() {
    if (is_playing()) {
        ma_sound_stop(&sound);
    } else {
        ma_sound_start(&sound);
    }
}

int song_length() {
    float ans;
    ma_sound_get_length_in_seconds(&sound,&ans);
    return (int)ans;
}

int cur_song_pos() {
    float ans;
    ma_sound_get_cursor_in_seconds(&sound,&ans);
    return (int)ans;
}

int finished() {
    return (int)ma_sound_at_end(&sound);
}

void restart_song() {
    ma_sound_seek_to_second(&sound,0);
}