#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "utils.h"
#include "colors/colors.h"
#include "songs/songs.h"
#include "albums/albums.h"
#include "playlist/playlist.h"
#include <unistd.h>
#include "audio_controller/audio_controller.h"
#include <string.h>
#include <sys/stat.h>

void clear() {
    fflush(stdout);
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void print_title() {
    printf(CYAN);
    printf("-------------------------------------------------------------------------------------------------------------------------------\n\n");
    printf("   ######         ##        ##  ###      ##  #######    ##        ##        ##     ######       ######    ########  ########\n");
    printf(" ##               ##        ##  ## #     ##  ##     ##  ##        ##        ##   ##           ##          ##        ##      ##\n");
    printf("##                ##        ##  ##  #    ##  ##     ##  ##        ##        ##  ##           ##           ##        ##       ##\n");
    printf("##         #####  ##        ##  ##   #   ##  #######    ##        ##        ##  ##     ####  ##     ####  ########  ##       ##\n");
    printf("##                ##        ##  ##    #  ##  ##         ##        ##        ##  ##       ##  ##       ##  ##        ##       ##\n");
    printf(" ##                ##      ##   ##     # ##  ##         ##         ##      ##    ##      ##   ##      ##  ##        ##      ##\n");
    printf("   ######            ######     ##      ###  ##         ########     ######        ######       ######    ########  ########\n");
    printf("\n-------------------------------------------------------------------------------------------------------------------------------\n\n");
    printf(CRESET);
}

void print_page_heading(Screen page) {
    switch (page) {
        case LIBRARY:
            printf(BLUE);
            printf("#     ##### ####  ####   ###  ####  #   #\n");
            printf("#       #   #   # #   # #   # #   #  # #\n");
            printf("#       #   ####  ####  ##### ####    #\n");
            printf("#       #   #   # #  #  #   # #  #    #\n");
            printf("##### ##### ####  #   # #   # #   #   #\n\n\n");
            printf(CRESET);
            break;
        case SONGS:
            printf(GREEN);
            printf(" ####  ###  #   #  ###   ####\n");
            printf("#     #   # ##  # #     #\n");
            printf(" ###  #   # # # # #  ##  ###\n");
            printf("    # #   # #  ## #   #     #\n");
            printf("####   ###  #   #  ###  ####\n\n\n");
            printf(CRESET);
            break;
        case ALBUMS:
            printf(YELLOW);
            printf(" ###  #     ####  #   # #   #  ####\n");
            printf("#   # #     #   # #   # ## ## #\n");
            printf("##### #     ####  #   # # # #  ###\n");
            printf("#   # #     #   # #   # #   #     #\n");
            printf("#   # ##### ####   ###  #   # ####\n\n\n");
            printf(CRESET);
            break;
        case ADD_SONG:
            printf(GREEN);
            printf(" ###  ####  ####      ####  ###  #   #  ###\n");
            printf("#   # #   # #   #    #     #   # ##  # #\n");
            printf("##### #   # #   #     ###  #   # # # # #  ##\n");
            printf("#   # #   # #   #        # #   # #  ## #   #\n");
            printf("#   # ####  ####     ####   ###  #   #  ###\n\n\n");
            printf(CRESET);
            break;
        case ADD_ALBUM:
            printf(YELLOW);
            printf(" ###  ####  ####      ###  #     ####  #   # #   #\n");
            printf("#   # #   # #   #    #   # #     #   # #   # ## ##\n");
            printf("##### #   # #   #    ##### #     ####  #   # # # #\n");
            printf("#   # #   # #   #    #   # #     #   # #   # #   #\n");
            printf("#   # ####  ####     #   # ##### ####   ###  #   #\n\n\n");
            printf(CRESET);
            break;
        case VIEW_ALBUM:
            printf(YELLOW);
            printf("#   # ##### ##### #   #     ###  #     ####  #   # #   #\n");
            printf("#   #   #   #     #   #    #   # #     #   # #   # ## ##\n");
            printf("#   #   #   ###   #   #    ##### #     ####  #   # # # #\n");
            printf(" # #    #   #     # # #    #   # #     #   # #   # #   #\n");
            printf("  #   ##### #####  # #     #   # ##### ####   ###  #   #\n\n\n");
            printf(CRESET);
            break;
        case EDIT_ALBUM:
            printf(YELLOW);
            printf("##### ####  ##### #####     ###  #     ####  #   # #   #\n");
            printf("#     #   #   #     #      #   # #     #   # #   # ## ##\n");
            printf("###   #   #   #     #      ##### #     ####  #   # # # #\n");
            printf("#     #   #   #     #      #   # #     #   # #   # #   #\n");
            printf("##### ####  #####   #      #   # ##### ####   ###  #   #\n\n\n");
            printf(CRESET);
            break;
        case PLAYLIST:
            printf(MAGENTA);
            printf("####  #      ###  #   # #     #####  #### #####\n");
            printf("#   # #     #   #  # #  #       #   #       #\n");
            printf("####  #     #####   #   #       #    ###    #\n");
            printf("#     #     #   #   #   #       #       #   #\n");
            printf("#     ##### #   #   #   ##### ##### ####    #\n\n\n");
            printf(CRESET);
            break;
        case EDIT_PL:
            printf(MAGENTA);
            printf("##### ####  ##### #####    ####  #      ###  #   # #     #####  #### #####\n");
            printf("#     #   #   #     #      #   # #     #   #  # #  #       #   #       #\n");
            printf("###   #   #   #     #      ####  #     #####   #   #       #    ###    #\n");
            printf("#     #   #   #     #      #     #     #   #   #   #       #       #   #\n");
            printf("##### ####  #####   #      #     ##### #   #   #   ##### ##### ####    #\n\n\n");
            printf(CRESET);
            break;
        case MUSIC_CONTR:
            printf(RED);
            printf("#   # #   #  #### #####  ####\n");
            printf("## ## #   # #       #   #\n");
            printf("# # # #   #  ###    #   #\n");
            printf("#   # #   #     #   #   #\n");
            printf("#   #  ###  ####  #####  ####\n\n\n");
            printf(CRESET);
            break;
        default:
    }
}

void erase(int no_char) {
    for (int i=0; i<no_char; i++) printf("\b");
    for (int i=0; i<no_char; i++) printf(" ");
    for (int i=0; i<no_char; i++) printf("\b");
}

void make_dir(char *name) {
#ifdef _WIN32
    mkdir(name);
#else
    mkdir(name,0777);
#endif
}

void check_base_dir() {
    DIR *dir=opendir(BASE_DIR);
    if (dir==NULL) {
        make_dir(BASE_DIR);
    }
    closedir(dir);
}

int length_songs(Song *list) {
    int ans=0;
    while (list!=NULL) {
        ans++;
        list=list->next;
    }
    return ans;
}
int length_albums(Album *list) {
    int ans=0;
    while (list!=NULL) {
        ans++;
        list=list->next;
    }
    return ans;
}
int length_playlist(Playlist *list) {
    int ans=0;
    while (list!=NULL) {
        ans++;
        list=list->next;
    }
    return ans;
}

void free_songs(Song *list) {
    while (list!=NULL) {
        Song *temp=list->next;
        free(list->title);
        free(list->artist);
        free(list->audio_loc);
        free(list);
        list=temp;
    }
}
void free_albums(Album *list) {
    while (list!=NULL) {
        Album *temp=list->next;
        free(list->title);
        free(list->song_ids);
        free(list);
        list=temp;
    }
}
void free_playlist(Playlist *list) {
    while (list!=NULL) {
        Playlist *temp=list->next;
        free(list);
        list=temp;
    }
}

void *manage_song_state(void *args) {
    Playlist **playlist=((SongStateArgs*)args)->playlist;
    CurrentState **current_state=((SongStateArgs*)args)->current_state;
    Screen *screen=((SongStateArgs*)args)->screen;
    int printed=0;
    while (1) {
        fflush(stdout);
        sleep(1);
        if ((*current_state)->playing) {
            if (finished()) {
                int er=0;
                Playlist *pl=*playlist;
                if (pl==NULL) {
                    (*current_state)->playing=0;
                    (*current_state)->playing_song_id=0;
                    continue;
                }
                while (pl!=NULL) {
                    if (pl->song->id==(*current_state)->playing_song_id) {
                        break;
                    }
                    pl=pl->next;
                }
                Playlist *to_play;
                stop_playing();
                pl->song->loaded=0;
                if (pl->next!=NULL) to_play=pl->next;
                else {
                    while (pl->prev!=NULL) pl=pl->prev;
                    to_play=pl;
                }
                er=load_song(to_play->song->audio_loc);
                if (er) {
                    (*current_state)->playing=0;
                    (*current_state)->playing_song_id=0;
                    continue;
                }
                to_play->song->loaded=1;
                toggle_pause();
                (*current_state)->playing_song_id=to_play->song->id;
                (*current_state)->playing=is_playing();
                if (*screen==MUSIC_CONTR) {
                    clear();
                    print_page_heading(MUSIC_CONTR);
                    pl = *playlist;
                    while (pl != NULL) {
                        int min = pl->song->duration / 60;
                        int sec = pl->song->duration % 60;
                        printf("%s %s - %s %02d:%02d\n", pl->song->id == (*current_state)->playing_song_id ? "->" : "  ",
                               pl->song->title, pl->song->artist, min, sec);
                        pl = pl->next;
                    }
                    printf("\n\n\n%s (P) %s| Next Song (N) | Previous Song (R) | Back (B) | Quit (Q): ",
                           (*current_state)->playing ? "Pause" : "Play",
                           (*current_state)->playing_song_id != 0 ? "| Restart song (S) " : "");
                } else if (*screen==PLAYLIST) {
                    clear();
                    print_page_heading(PLAYLIST);
                    pl=*playlist;
                    while (pl!=NULL) {
                        Song *song=pl->song;
                        int min=song->duration/60;
                        int sec=song->duration%60;
                        printf("%s - %s %02d:%02d %s\n",song->title,song->artist,min,sec,
                            (*current_state)->playing&&(*current_state)->playing_song_id==song->id?"- Now Playing":"");
                        pl=pl->next;
                    }
                    printf("\n\n\nEdit Playlist (E) |%s Back (B) | Quit (Q): ",playlist==NULL?"":" Music Controls (C) |");
                }
            }
        }
        if (*screen==MUSIC_CONTR&&(*current_state)->playing_song_id!=0) {
            int l=song_length();
            int p=cur_song_pos();
            int n=BAR_LENGTH*p/l;
            char s[150];
            sprintf(s,"%s (P) %s| Next Song (N) | Previous Song (R) | Back (B) | Quit (Q): ",(*current_state)->playing?"Pause":"Play",(*current_state)->playing_song_id!=0?"| Restart song (S) ":"");
            erase(strlen(s)+2);
            printf("\033[A\033[A");
            if (printed) {
                erase(BAR_LENGTH+2);
            }
            printf("[");
            for (int i=0; i<n; i++) printf("#");
            for (int i=0; i<BAR_LENGTH-n; i++) printf(" ");
            printf("]\n\n");
            printf("%s",s);
            printed=1;
        } else printed=0;
    }
    return NULL;
}