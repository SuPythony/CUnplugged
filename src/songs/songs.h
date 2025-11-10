#ifndef MUSIC_SONGS
#define MUSIC_SONGS

#include "albums/albums.h"

typedef struct Song {
    long long id;
    char *title;
    char *artist;
    int duration;
    char *audio_loc;
    int loaded;
    struct Song *prev;
    struct Song *next;
} Song;

Song *parse_song_file(char *name);
Song *load_songs();
Song *create_new_song(Song *songs, long long id, char *title, char *artist, int dur, char *loc);
Song *delete_song(Song *songs, long long id, Album *albums);

#endif