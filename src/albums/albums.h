#ifndef MUSIC_ALBUMS
#define MUSIC_ALBUMS

typedef struct Album {
    long long id;
    char *title;
    int song_count;
    long long *song_ids;
    struct Album *prev;
    struct Album *next;
} Album;

Album *parse_album_file(char *name); /* Returns album struct constructed from file */
Album *load_albums(); /* Returns first node of album linked list */
Album *create_new_album(Album *albums, long long id, char *title, int song_count, long long *song_ids); /* Returns first node of modified album linked list */
void write_album_file(Album *album);
void add_song_to_album(Album *album, long long song_id);
void remove_song_from_album(Album *album, long long song_id);
Album *delete_album(Album *albums, long long id); /* Returns first node of modified album linked list */

#endif