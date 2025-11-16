#ifndef MUSIC_INTERFACE
#define MUSIC_INTERFACE

#include "utils/utils.h"
#include "songs/songs.h"
#include "albums/albums.h"
#include "playlist/playlist.h"

Screen library_screen();
Screen songs_screen(Song **songs, Album *albums, Playlist **playlist, CurrentState *current_state);
AlbumsScreenReturn albums_screen(Album **albums);
Screen add_song_screen(Song **songs);
Screen add_album_screen(Song *songs, Album **albums);
Screen view_album_screen(Song *songs, Album *albums, int album_ind);
Screen edit_album_screen(Song *songs, Album *albums, int album_ind);
Screen logs_screen();
Screen playlist_screen(Playlist *playlist, CurrentState *current_state);
Screen edit_playlist_screen(Playlist **playlist, Song *songs, Album *albums, CurrentState *current_state);
Screen music_control_screen(Playlist *playlist, CurrentState *current_state);

#endif