#ifndef MUSIC_AUDIO_CONTROLLER
#define MUSIC_AUDIO_CONTROLLER

void init_audio();
void uninit_audio();
int load_song(char *path);
void stop_playing();
int is_playing();
void toggle_pause();
int song_length();
int cur_song_pos();
int finished();
void restart_song();

#endif