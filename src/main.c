#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils/utils.h"
#include "songs/songs.h"
#include "albums/albums.h"
#include <time.h>
#include <string.h>
#include "interface/interface.h"
#include "playlist/playlist.h"
#include "audio_controller/audio_controller.h"
#include "pthread.h"
#include "command_log/command_log.h"

void intro();
void outro();
void free_memory(Song *songs, Album *albums, Playlist *playlist, CurrentState *current_state);

int main() {
    clear();
    srand(time(NULL)); rand();
    init_audio();
    check_base_dir();
    //intro();
    Song *songs=load_songs();
    Album *albums=load_albums();
    Playlist *playlist=NULL;
    CurrentState *current_state=malloc(sizeof(CurrentState));
    current_state->playing=0;
    current_state->playing_song_id=0;
    Screen screen=LIBRARY;
    pthread_t song_state_thread;
    SongStateArgs args = {&playlist,&current_state,&screen};
    pthread_create(&song_state_thread,NULL,manage_song_state,&args);
    int quit=0;
    int data=-1;
    while (1) {
        switch (screen) {
            case LIBRARY:
                screen=library_screen();
                break;
            case SONGS:
                screen=songs_screen(&songs,albums,&playlist,current_state);
                break;
            case ALBUMS:
                AlbumsScreenReturn ret=albums_screen(&albums);
                screen=ret.screen;
                data=ret.album_ind;
                break;
            case LOGS:
                screen=logs_screen();
                break;
            case ADD_SONG:
                screen=add_song_screen(&songs);
                break;
            case ADD_ALBUM:
                screen=add_album_screen(songs,&albums);
                break;
            case VIEW_ALBUM:
                screen=view_album_screen(songs,albums,data);
                break;
            case EDIT_ALBUM:
                screen=edit_album_screen(songs,albums,data);
                break;
            case PLAYLIST:
                screen=playlist_screen(playlist,current_state);
                break;
            case EDIT_PL:
                screen=edit_playlist_screen(&playlist,songs,albums,current_state);
                break;
            case MUSIC_CONTR:
                screen=music_control_screen(playlist,current_state);
                break;
            default:
                quit=1;
        }
        if (quit) break;
    }
    outro();
    uninit_audio();
    free_memory(songs,albums,playlist,current_state);
    return 0;
}

void intro() {
    clear();
    printf("Welcome to\n\n");
    print_title();
    for (int i=1; i<=3; i++) {
        printf("\rLoading");
        for (int k=0; k<i; k++) printf(".");
        fflush(stdout);
        sleep(1);
    }
    erase(strlen("Loading..."));
    printf("Press enter to start listening...");
    getchar();
    add_command("Enter program");
    clear();
}

void outro() {
    clear();
    print_title();
    printf("Bye!\n");
}

void free_memory(Song *songs, Album *albums, Playlist *playlist, CurrentState *current_state) {
    free_songs(songs);
    free_albums(albums);
    free_playlist(playlist);
    free(current_state);
}