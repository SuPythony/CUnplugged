#ifndef MUSIC_PLAYLIST
#define MUSIC_PLAYLIST

#include "songs/songs.h"

typedef struct Playlist {
    Song *song;
    struct Playlist *prev;
    struct Playlist *next;
    int head;
    int tail;
} Playlist;

typedef struct CurrentState {
    int playing;
    long long playing_song_id;
} CurrentState;

Playlist *add_song_to_playlist(Playlist *playlist, Song *song);
Playlist *add_album_to_playlist(Playlist *playlist, Album *album, Song *songs);
Playlist *remove_song_from_playlist(Playlist *playlist, Song *song);

#endif