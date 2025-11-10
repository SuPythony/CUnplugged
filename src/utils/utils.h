#ifndef MUSIC_UTILS
#define MUSIC_UTILS
#include "songs/songs.h"
#include "albums/albums.h"
#include "playlist/playlist.h"

#define BASE_DIR "library"
#define BAR_LENGTH 40

typedef enum Screen {
    QUIT,
    LIBRARY,
    SONGS,
    ALBUMS,
    ADD_SONG,
    ADD_ALBUM,
    VIEW_ALBUM,
    EDIT_ALBUM,
    PLAYLIST,
    EDIT_PL,
    MUSIC_CONTR
} Screen;

typedef struct AlbumsScreenReturn {
    Screen screen;
    int album_ind;
} AlbumsScreenReturn;

typedef struct SongStateArgs {
    Playlist **playlist;
    CurrentState **current_state;
    Screen *screen;
} SongStateArgs;

void clear();
void print_title();
void print_page_heading(Screen page);
void erase(int no_char);
void make_dir(char *name);
void check_base_dir();
int length_songs(Song *song);
int length_albums(Album *album);
int length_playlist(Playlist *list);
void free_songs(Song *first);
void free_albums(Album *first);
void free_playlist(Playlist *first);
void *manage_song_state(void *args);

#endif