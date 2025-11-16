#include <stdlib.h>
#include "playlist/playlist.h"
#include "songs/songs.h"
#include "albums/albums.h"

Playlist *add_song_to_playlist(Playlist *playlist, Song *song) {
    if (playlist==NULL) {
        Playlist *pl=malloc(sizeof(Playlist));
        pl->prev=pl;
        pl->next=pl;
        pl->song=song;
        pl->head=1;
        pl->tail=1;
        return pl;
    }
    Playlist *pl=playlist;
    while (1) {
        if (pl->song==song) {
            return playlist;
        }
        if (pl->tail) break;
        pl=pl->next;
    }
    pl->next=malloc(sizeof(Playlist));
    pl->next->prev=pl;
    pl->next->next=playlist;
    pl->next->song=song;
    pl->tail=0;
    pl->next->tail=1;
    pl->next->head=0;
    playlist->prev=pl->next;
    return playlist;
}

Playlist *add_album_to_playlist(Playlist *playlist, Album *album, Song *songs) {
    Playlist *pl=playlist;
    for (int i=0; i<album->song_count; i++) {
        long long id=album->song_ids[i];
        Song *song=songs;
        while (song!=NULL) {
            if (song->id==id) {
                pl=add_song_to_playlist(pl,song);
                break;
            }
            song=song->next;
        }
    }
    return pl;
}

Playlist *remove_song_from_playlist(Playlist *playlist, Song *song) {
    Playlist *pl=playlist;
    Playlist *ret=playlist;
    if (pl==NULL) return ret;
    while (1) {
        if (pl->song->id==song->id) {
            if (pl->head&&pl->tail) {
                ret=NULL;
                free(pl);
                break;
            }
            pl->prev->next=pl->next;
            pl->next->prev=pl->prev;
            if (pl->head) {
                pl->next->head=1;
                ret=pl->next;
            }
            free(pl);
            break;
        }
        if (pl->tail) break;
        pl=pl->next;
    }
    return ret;
}