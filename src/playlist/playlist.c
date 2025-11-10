#include <stdlib.h>
#include "playlist/playlist.h"
#include "songs/songs.h"
#include "albums/albums.h"

Playlist *add_song_to_playlist(Playlist *playlist, Song *song) {
    if (playlist==NULL) {
        Playlist *pl=malloc(sizeof(Playlist));
        pl->prev=NULL;
        pl->next=NULL;
        pl->song=song;
        return pl;
    }
    Playlist *pl=playlist;
    while (pl->next!=NULL) {
        if (pl->song==song) {
            return playlist;
        }
        pl=pl->next;
    }
    if (pl->song==song) {
        return playlist;
    }
    pl->next=malloc(sizeof(Playlist));
    pl->next->prev=pl;
    pl->next->next=NULL;
    pl->next->song=song;
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
    while (pl!=NULL) {
        if (pl->song->id==song->id) {
            if (pl->prev!=NULL) pl->prev->next=pl->next;
            if (pl->next!=NULL) pl->next->prev=pl->prev;
            if (pl->prev==NULL) {
                if (pl->next==NULL) {
                    ret=NULL;
                } else {
                    ret=pl->next;
                }
            } else {
                ret=playlist;
            }
            free(pl);
            break;
        }
        pl=pl->next;
    }
    return ret;
}