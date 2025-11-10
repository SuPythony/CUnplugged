#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "songs/songs.h"
#include "utils/utils.h"
#include "colors/colors.h"
#include "albums/albums.h"
#include "playlist/playlist.h"
#include "audio_controller/audio_controller.h"
#include "command_log/command_log.h"

Screen library_screen() {
    int sel=0;
    char *opt[]={"Songs", "Albums", "Playlist"};
    int n=3;
    while (1) {
        clear();
        print_page_heading(LIBRARY);
        for (int i=0; i<n; i++) {
            printf("[%s] %s\n",sel==i?"#":" ",opt[i]);
        }
        printf("\n\nDown (D) | Up (U) | Select (S) | Quit (Q): ");
        unsigned char inp=getchar();
        if (inp=='D'||inp=='d') {
            add_command("Move down (Library)");
            if (sel<n-1) sel++;;
        } else if (inp=='U'||inp=='u') {
            add_command("Move up (Library)");
            if (sel>0) sel--;
        } else if (inp=='S'||inp=='s') {
            add_command("Select (Library)");
            break;
        } else if (inp=='Q'||inp=='q') {
            add_command("Quit (Library)");
            sel=-1;
            break;
        }
    }
    if (sel==0) {
        return SONGS;
    } else if (sel==1) {
        return ALBUMS;
    } else if (sel==2) {
        return PLAYLIST;
    } else {
        return QUIT;
    }
}

Screen songs_screen(Song **songs, Album *albums, Playlist **playlist, CurrentState *current_state) {
    int sel=0;
    Screen res=QUIT;
    int n=length_songs(*songs);
    while (1) {
        clear();
        print_page_heading(SONGS);
        if (*songs==NULL) {
            printf("No songs added in library yet");
            printf("\n\n\nAdd a song (A) | Back (B) | Quit (Q): ");
            fflush(stdin);
            unsigned char inp=getchar();
            if (inp=='A'||inp=='a') {
                add_command("Add song (Songs)");
                res=ADD_SONG;
                break;
            } else if (inp=='B'||inp=='b') {
                add_command("Back (Songs)");
                res=LIBRARY;
                break;
            } else if (inp=='Q'||inp=='q') {
                add_command("Quit (Songs)");
                break;
            }
        } else {
            Song *song=*songs;
            int ind=0;
            while (song!=NULL) {
                int min=song->duration/60;
                int sec=song->duration%60;
                printf("[%s] %s - %s %02d:%02d\n",ind==sel?"#":" ",song->title,song->artist,min,sec);
                ind++;
                song=song->next;
            }
            printf("\n\n\nDown (D) | Up (U) | Add a song (A) | Delete song (X) | Back (B) | Quit (Q): ");
            fflush(stdin);
            unsigned char inp=getchar();
            if (inp=='D'||inp=='d') {
                add_command("Move down (Songs)");
                if (sel<n-1) sel++;
            } else if (inp=='U'||inp=='u') {
                add_command("Move up (Songs)");
                if (sel>0) sel--;
            } else if (inp=='A'||inp=='a') {
                add_command("Add song (Songs)");
                res=ADD_SONG;
                break;
            } else if (inp=='X'||inp=='x') {
                add_command("Delete song (Songs)");
                printf("\nAre you sure you want to delete the selected song?");
                printf("\nYes (Y) | No (*): ");
                fflush(stdin);
                unsigned char inp2=getchar();
                if (inp2=='Y'||inp2=='y') {
                    add_command("Yes (Songs)");
                    ind=0;
                    song=*songs;
                    while (song!=NULL) {
                        if (ind==sel) {
                            if (current_state->playing&&current_state->playing_song_id==song->id) {
                                printf("Song currently playing\nPress enter to continue...");
                                fflush(stdin);
                                getchar();
                                break;
                            }
                            *playlist=remove_song_from_playlist(*playlist,song);
                            *songs=delete_song(*songs,song->id,albums);
                            break;
                        }
                        ind++;
                        song=song->next;
                    }
                    sel=0;
                    n=length_songs(*songs);
                } else {
                    add_command("No (Songs)");
                }
            } else if (inp=='B'||inp=='b') {
                add_command("Back (Songs)");
                res=LIBRARY;
                break;
            } else if (inp=='Q'||inp=='q') {
                add_command("Quit (Songs)");
                break;
            }
        }
    }
    return res;
}

Screen add_song_screen(Song **songs) {
    clear();
    print_page_heading(ADD_SONG);
    char title[200], artist[200], audio_file[200];
    title[0]='\0';
    artist[0]='\0';
    audio_file[0]='\0';
    int duration=0;
    while (strlen(title)==0) {
        printf("Enter song title: ");
        fflush(stdin);
        scanf(" %[^\n]",title);
    }
    while (strlen(artist)==0) {
        printf("Enter song artist(s) name: ");
        fflush(stdin);
        scanf(" %[^\n]",artist);
    }
    while (duration==0) {
        printf("Enter song duration (in seconds): ");
        fflush(stdin);
        scanf("%d",&duration);
    }
    printf("\n(Save the song audio file in library/songs/audios)\n");
    while (strlen(audio_file)==0) {
        printf("Enter audio file name (with extension, without path): ");
        fflush(stdin);
        scanf(" %[^\n]",audio_file);
    }
    fflush(stdin);
    char *audio_loc=malloc(250);
    strcpy(audio_loc,BASE_DIR);
    strcat(audio_loc,"/audios/");
    strcat(audio_loc,audio_file);
    *songs=create_new_song(*songs,(long long)rand()*rand(),title,artist,duration,audio_loc);
    char c[250];
    sprintf(c,"Song (%s) added (Add Song)",title);
    add_command(c);
    printf("\n\nSong added successfully!\n");
    printf("Press enter to return...");
    fflush(stdin);
    getchar();
    return SONGS;
}

AlbumsScreenReturn albums_screen(Album **albums) {
    int sel=0;
    AlbumsScreenReturn res={QUIT,-1};
    int n=length_albums(*albums);
    while (1) {
        clear();
        print_page_heading(ALBUMS);
        if (*albums==NULL) {
            printf("No albums added in library yet");
            printf("\n\n\nAdd an album (A) | Back (B) | Quit (Q): ");
            fflush(stdin);
            unsigned char inp=getchar();
            if (inp=='A'||inp=='a') {
                add_command("Add albums (Albums)");
                res.screen=ADD_ALBUM;
                break;
            } else if (inp=='B'||inp=='b') {
                add_command("Back (Albums)");
                res.screen=LIBRARY;
                break;
            } else if (inp=='Q'||inp=='q') {
                add_command("Quit (Albums)");
                break;
            }
        } else {
            Album *album=*albums;
            int ind=0;
            while (album!=NULL) {
                printf("[%s] %s - %d songs\n",ind==sel?"#":" ",album->title,album->song_count);
                ind++;
                album=album->next;
            }
            printf("\n\n\nDown (D) | Up (U) | Add an album (A) | Delete album (X) | View Album (V) | Edit Album (E) | Back (B) | Quit (Q): ");
            fflush(stdin);
            unsigned char inp=getchar();
            if (inp=='D'||inp=='d') {
                add_command("Move down (Albums)");
                if (sel<n-1) sel++;
            } else if (inp=='U'||inp=='u') {
                add_command("Move up (Albums)");
                if (sel>0) sel--;
            } else if (inp=='A'||inp=='a') {
                add_command("Add album (Albums)");
                res.screen=ADD_ALBUM;
                break;
            } else if (inp=='X'||inp=='x') {
                add_command("Delete album (Albums)");
                printf("\nAre you sure you want to delete the selected album?");
                printf("\nYes (Y) | No (*): ");
                fflush(stdin);
                unsigned char inp2=getchar();
                if (inp2=='Y'||inp2=='y') {
                    add_command("Yes (Albums)");
                    ind=0;
                    album=*albums;
                    while (album!=NULL) {
                        if (ind==sel) {
                            *albums=delete_album(*albums,album->id);
                            break;
                        }
                        ind++;
                        album=album->next;
                    }
                    sel=0;
                    n=length_albums(*albums);
                } else {
                    add_command("No (Albums)");
                }
            } else if (inp=='V'||inp=='v') {
                add_command("View album (Albums)");
                res.screen=VIEW_ALBUM;
                res.album_ind=sel;
                break;
            } else if (inp=='E'||inp=='e') {
                add_command("Edit album (Albums)");
                res.screen=EDIT_ALBUM;
                res.album_ind=sel;
                break;
            } else if (inp=='B'||inp=='b') {
                add_command("Back (Albums)");
                res.screen=LIBRARY;
                break;
            } else if (inp=='Q'||inp=='q') {
                add_command("Quit (Albums)");
                break;
            }
        }
    }
    return res;
}

Screen add_album_screen(Song *songs, Album **albums) {
    clear();
    print_page_heading(ADD_ALBUM);
    if (songs==NULL) {
        printf("No songs added in library yet\n.");
        printf("Press enter to return...");
        fflush(stdin);
        getchar();
        return ALBUMS;
    }
    char title[200];
    title[0]='\0';
    while (strlen(title)==0) {
        printf("Enter album title: ");
        fflush(stdin);
        scanf(" %[^\n]",title);
    }
    int sel=0;
    int n=length_songs(songs);
    int is_sel[n];
    for (int i=0; i<n; i++) is_sel[i]=0;
    while (1) {
        clear();
        print_page_heading(ADD_ALBUM);
        Song *song=songs;
        int ind=0;
        while (song!=NULL) {
            int min=song->duration/60;
            int sec=song->duration%60;
            printf(is_sel[ind]?NORMAL:DIM);
            printf("[%s] %s - %s %02d:%02d\n",ind==sel?"#":" ",song->title,song->artist,min,sec);
            printf(CRESET);
            ind++;
            song=song->next;
        }
        printf("\n\n\nDown (D) | Up (U) | %s | Save (S) | Cancel (C) | Quit (Q): ",is_sel[sel]?"Remove from album (R)":"Add to album (A)");
        fflush(stdin);
        unsigned char inp=getchar();
        if (inp=='D'||inp=='d') {
            add_command("Move down (Add Album)");
            if (sel<n-1) sel++;
        } else if (inp=='U'||inp=='u') {
            add_command("Move up (Add Album)");
            if (sel>0) sel--;
        } else if (!is_sel[sel]&&(inp=='A'||inp=='a')) {
            add_command("Add song (Add Album)");
            is_sel[sel]=1;
        } else if (is_sel[sel]&&(inp=='R'||inp=='r')) {
            add_command("Remove song (Add Album)");
            is_sel[sel]=0;
        } else if (inp=='S'||inp=='s') {
            add_command("Save (Add Album)");
            int song_count=0;
            for (int i=0; i<n; i++) {
                if (is_sel[i]) song_count++;
            }
            if (song_count==0) {
                printf("Select at least one song\n");
                printf("Press enter to continue...");
                fflush(stdin);
                getchar();
                continue;
            }
            long long *song_ids=malloc(song_count*sizeof(long long));
            ind=0;
            int ind2=0;
            song=songs;
            while (song!=NULL) {
                if (is_sel[ind]) {
                    song_ids[ind2]=song->id;
                    ind2++;
                }
                ind++;
                song=song->next;
            }
            *albums=create_new_album(*albums,(long long)rand()*rand(),title,song_count,song_ids);
            printf("\n\nAlbum added successfully!\n");
            printf("Press enter to return...");
            fflush(stdin);
            getchar();
            break;
        } else if (inp=='C'||inp=='c') {
            add_command("Cancel (Add Album)");
            break;
        } else if (inp=='Q'||inp=='q') {
            add_command("Quit (Add Album)");
            return QUIT;
        }
    }
    return ALBUMS;
}

Screen view_album_screen(Song *songs, Album *albums, int album_ind) {
    clear();
    print_page_heading(VIEW_ALBUM);
    int ind=0;
    Album *album=albums;
    while (ind<album_ind) {
        album=album->next;
        ind++;
    }
    printf("Title: %s\n",album->title);
    printf("%d Songs\n\n",album->song_count);
    Song *song=songs;
    while (song!=NULL) {
        int in_album=0;
        for (int i=0; i<album->song_count; i++) {
            if (album->song_ids[i]==song->id) {
                in_album=1;
                break;
            }
        }
        if (in_album) {
            int min=song->duration/60;
            int sec=song->duration%60;
            printf("%s - %s %02d:%02d\n",song->title,song->artist,min,sec);
        }
        song=song->next;
    }
    printf("\n");
    printf("Press enter to return...");
    fflush(stdin);
    getchar();
    return ALBUMS;
}

Screen edit_album_screen(Song *songs, Album *albums, int album_ind) {
    int ind=0;
    Album *album=albums;
    while (ind<album_ind) {
        album=album->next;
        ind++;
    }
    int sel=0;
    int n=length_songs(songs);
    int is_sel[n];
    for (int i=0; i<n; i++) is_sel[i]=0;
    ind=0;
    Song *song=songs;
    while (song!=NULL) {
        int in_album=0;
        for (int i=0; i<album->song_count; i++) {
            if (album->song_ids[i]==song->id) {
                in_album=1;
                break;
            }
        }
        if (in_album) {
            is_sel[ind]=1;
        }
        song=song->next;
        ind++;
    }
    while (1) {
        clear();
        print_page_heading(EDIT_ALBUM);
        printf("Title: %s\n",album->title);
        printf("%d Songs\n\n",album->song_count);
        song=songs;
        ind=0;
        while (song!=NULL) {
            int min=song->duration/60;
            int sec=song->duration%60;
            printf(is_sel[ind]?NORMAL:DIM);
            printf("[%s] %s - %s %02d:%02d\n",ind==sel?"#":" ",song->title,song->artist,min,sec);
            printf(CRESET);
            ind++;
            song=song->next;
        }
        printf("\n\n\nDown (D) | Up (U) | %s | Save (S) | Cancel (C) | Quit (Q): ",is_sel[sel]?"Remove from album (R)":"Add to album (A)");
        fflush(stdin);
        unsigned char inp=getchar();
        if (inp=='D'||inp=='d') {
            add_command("Move down (Edit Album)");
            if (sel<n-1) sel++;
        } else if (inp=='U'||inp=='u') {
            add_command("Move up (Edit Album)");
            if (sel>0) sel--;
        } else if (!is_sel[sel]&&(inp=='A'||inp=='a')) {
            add_command("Add song (Edit Album)");
            is_sel[sel]=1;
        } else if (is_sel[sel]&&(inp=='R'||inp=='r')) {
            add_command("Remove song (Edit Album)");
            is_sel[sel]=0;
        } else if (inp=='S'||inp=='s') {
            add_command("Save (Edit Album)");
            int song_count=0;
            for (int i=0; i<n; i++) {
                if (is_sel[i]) song_count++;
            }
            if (song_count==0) {
                printf("Select at least one song\n");
                printf("Press enter to continue...");
                fflush(stdin);
                getchar();
                continue;
            }
            long long *song_ids=realloc(album->song_ids,song_count*sizeof(long long));
            ind=0;
            int ind2=0;
            song=songs;
            while (song!=NULL) {
                if (is_sel[ind]) {
                    song_ids[ind2]=song->id;
                    ind2++;
                }
                ind++;
                song=song->next;
            }
            album->song_count=song_count;
            album->song_ids=song_ids;
            write_album_file(album);
            printf("\n\nAlbum edited successfully!\n");
            printf("Press enter to return...");
            fflush(stdin);
            getchar();
            break;
        } else if (inp=='C'||inp=='c') {
            add_command("Cancel (Edit Album)");
            break;
        } else if (inp=='Q'||inp=='q') {
            add_command("Quit (Edit Album)");
            return QUIT;
        }
    }
    return ALBUMS;
}

Screen playlist_screen(Playlist *playlist, CurrentState *current_state) {
    Screen res=QUIT;
    while (1) {
        clear();
        print_page_heading(PLAYLIST);
        if (playlist==NULL) {
            printf("Playlist is empty");
        } else {
            Playlist *pl=playlist;
            while (pl!=NULL) {
                Song *song=pl->song;
                int min=song->duration/60;
                int sec=song->duration%60;
                printf("%s - %s %02d:%02d %s\n",song->title,song->artist,min,sec,
                    current_state->playing&&current_state->playing_song_id==song->id?"- Now Playing":"");
                pl=pl->next;
            }
        }
        printf("\n\n\nEdit Playlist (E) |%s Back (B) | Quit (Q): ",playlist==NULL?"":" Music Controls (C) |");
        fflush(stdin);
        unsigned char inp=getchar();
        if (inp=='E'||inp=='e') {
            add_command("Edit Playlist (Playlist)");
            res=EDIT_PL;
            break;
        } else if (playlist!=NULL&&(inp=='C'||inp=='c')) {
            add_command("Music Controls (Playlist)");
            res=MUSIC_CONTR;
            break;
        } else if (inp=='B'||inp=='b') {
            add_command("Back (Playlist)");
            res=LIBRARY;
            break;
        } else if (inp=='Q'||inp=='q') {
            add_command("Quit (Playlist)");
            break;
        }
    }
    return res;
}

Screen edit_playlist_screen(Playlist **playlist, Song *songs, Album *albums, CurrentState *current_state) {
    Screen res=QUIT;
    int sel=0;
    int n=length_songs(songs);
    int n2=length_albums(albums);
    int is_sel[n];
    for (int i=0; i<n; i++) is_sel[i]=0;
    int ind=0;
    Song *song=songs;
    while (song!=NULL) {
        int in_pl=0;
        Playlist *pl=*playlist;
        while (pl!=NULL) {
            if (pl->song==song) {
                in_pl=1;
                break;
            }
            pl=pl->next;
        }
        if (in_pl) {
            is_sel[ind]=1;
        }
        song=song->next;
        ind++;
    }
    while (1) {
        clear();
        print_page_heading(EDIT_PL);
        ind=0;
        printf("Current Playlist:\n\n");
        if (*playlist==NULL) {
            printf("Playlist is empty\n");
        } else {
            Playlist *pl=*playlist;
            while (pl!=NULL) {
                song=pl->song;
                int min=song->duration/60;
                int sec=song->duration%60;
                printf("%s - %s %02d:%02d %s\n",song->title,song->artist,min,sec,
                    current_state->playing&&current_state->playing_song_id==song->id?"- Now Playing":"");
                pl=pl->next;
            }
        }
        printf("\n\n");
        printf("Songs:\n\n");
        song=songs;
        if (song==NULL) {
            printf("No songs added yet\n");
        } else {
            while (song!=NULL) {
                int min=song->duration/60;
                int sec=song->duration%60;
                printf(is_sel[ind]?NORMAL:DIM);
                printf("[%s] %s - %s %02d:%02d\n",ind==sel?"#":" ",song->title,song->artist,min,sec);
                printf(CRESET);
                ind++;
                song=song->next;
            }
        }
        printf("\nAlbums:\n\n");
        Album *album=albums;
        if (album==NULL) {
            printf("No albums added yet\n");
        } else {
            while (album!=NULL) {
                printf("[%s] %s\n",ind==sel?"#":" ",album->title);
                ind++;
                album=album->next;
            }
        }
        printf("\n\n\nDown (D) | Up (U) | %s | Add all songs (L) | Clear playlist (C) | Back (B) | Quit (Q): ",sel<n&&is_sel[sel]?"Remove from playlist (R)":"Add to playlist (A)");
        fflush(stdin);
        unsigned char inp=getchar();
        if (inp=='D'||inp=='d') {
            add_command("Move down (Edit Playlist)");
            if (sel<n+n2-1) sel++;
        } else if (inp=='U'||inp=='u') {
            add_command("Move up (Edit Playlist)");
            if (sel>0) sel--;
        } else if ((sel>=n||!is_sel[sel])&&(inp=='A'||inp=='a')) {
            if (sel<n) {
                add_command("Add song (Edit Playlist)");
                is_sel[sel]=1;
                song=songs;
                for (int i=0; i<sel; i++) {
                    song=song->next;
                }
                *playlist=add_song_to_playlist(*playlist,song);
            } else {
                add_command("Add album (Edit Playlist)");
                album=albums;
                for (int i=0; i<sel-n; i++) {
                    album=album->next;
                }
                *playlist=add_album_to_playlist(*playlist,album,songs);
                ind=0;
                song=songs;
                while (song!=NULL) {
                    int in_album=0;
                    for (int i=0; i<album->song_count; i++) {
                        if (album->song_ids[i]==song->id) {
                            in_album=1;
                            break;
                        }
                    }
                    if (in_album) {
                        is_sel[ind]=1;
                    }
                    ind++;
                    song=song->next;
                }
            }
        } else if (sel<n&&is_sel[sel]&&(inp=='R'||inp=='r')) {
            add_command("Remove song (Edit Playlist)");
            song=songs;
            for (int i=0; i<sel; i++) {
                song=song->next;
            }
            if (current_state->playing&&current_state->playing_song_id==song->id) {
                printf("Song currently playing\nPress enter to continue...");
                fflush(stdin);
                getchar();
                continue;
            }
            is_sel[sel]=0;
            *playlist=remove_song_from_playlist(*playlist,song);
        } else if (inp=='L'||inp=='l') {
            add_command("Add all songs (Edit Playlist)");
            song=songs;
            for (int i=0; i<n; i++) {
                if (is_sel[i]) {
                    song=song->next;
                    continue;
                }
                is_sel[i]=1;
                *playlist=add_song_to_playlist(*playlist,song);
                song=song->next;
            }
        } else if (inp=='C'||inp=='c') {
            add_command("Clear playlist (Edit Playlist)");
            song=songs;
            for (int i=0; i<n; i++) {
                if (is_sel[i]) {
                    if (current_state->playing&&current_state->playing_song_id==song->id) {
                        printf("Song currently playing\nPress enter to continue...");
                        fflush(stdin);
                        getchar();
                        continue;
                    }
                    is_sel[i]=0;
                    *playlist=remove_song_from_playlist(*playlist,song);
                }
                song=song->next;
            }
        } else if (inp=='B'||inp=='b') {
            add_command("Back (Edit Playlist)");
            res=PLAYLIST;
            break;
        } else if (inp=='Q'||inp=='q') {
            add_command("Quit (Edit Playlist)");
            break;
        }
    }
    return res;
}

Screen music_control_screen(Playlist *playlist, CurrentState *current_state) {
    Screen res=QUIT;
    while (1) {
        clear();
        print_page_heading(MUSIC_CONTR);
        Playlist *pl=playlist;
        while (pl!=NULL) {
            int min=pl->song->duration/60;
            int sec=pl->song->duration%60;
            printf("%s %s - %s %02d:%02d\n",pl->song->id==current_state->playing_song_id?"->":"  ",pl->song->title,pl->song->artist,min,sec);
            pl=pl->next;
        }
        printf("\n\n\n%s (P) %s| Next Song (N) | Previous Song (R) | Back (B) | Quit (Q): ",current_state->playing?"Pause":"Play",current_state->playing_song_id!=0?"| Restart song (S) ":"");
        fflush(stdin);
        unsigned char inp=getchar();
        if (inp=='P'||inp=='p') {
            if (current_state->playing) add_command("Pause song (Music Control)");
            else add_command("Play song (Music Control)");
            int er=0;
            long long id_to_play=current_state->playing_song_id;
            if (current_state->playing_song_id==0) {
                id_to_play=playlist->song->id;
            }
            pl=playlist;
            while (pl!=NULL) {
                if (pl->song->id==id_to_play) {
                    if (!pl->song->loaded) {
                        er=load_song(pl->song->audio_loc);
                        pl->song->loaded=1;
                    }
                    break;
                }
                pl=pl->next;
            }
            if (er) {
                current_state->playing=0;
                printf("Can't open audio file.\n");
                printf("Press enter to continue...");
                fflush(stdin);
                getchar();
                continue;
            }
            toggle_pause();
            current_state->playing_song_id=id_to_play;
            current_state->playing=is_playing();
        } else if (current_state->playing_song_id!=0&&(inp=='S'||inp=='s')) {
            add_command("Restart song (Music Control)");
            restart_song();
        } else if (inp=='N'||inp=='n') {
            add_command("Next song (Music Control)");
            int er=0;
            if (current_state->playing_song_id==0) continue;
            pl=playlist;
            while (pl!=NULL) {
                if (pl->song->id==current_state->playing_song_id) {
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
                current_state->playing=0;
                printf("Can't open audio file.\n");
                printf("Press enter to continue...");
                fflush(stdin);
                getchar();
                continue;
            }
            to_play->song->loaded=1;
            toggle_pause();
            current_state->playing_song_id=to_play->song->id;
            current_state->playing=is_playing();
        } else if (inp=='R'||inp=='r') {
            add_command("Previous song (Music Control)");
            int er=0;
            if (current_state->playing_song_id==0) continue;
            pl=playlist;
            while (pl!=NULL) {
                if (pl->song->id==current_state->playing_song_id) {
                    break;
                }
                pl=pl->next;
            }
            Playlist *to_play;
            stop_playing();
            pl->song->loaded=0;
            if (pl->prev!=NULL) to_play=pl->prev;
            else {
                while (pl->next!=NULL) pl=pl->next;
                to_play=pl;
            }
            er=load_song(to_play->song->audio_loc);
            if (er) {
                current_state->playing=0;
                printf("Can't open audio file.\n");
                printf("Press enter to continue...");
                fflush(stdin);
                getchar();
                continue;
            }
            to_play->song->loaded=1;
            toggle_pause();
            current_state->playing_song_id=to_play->song->id;
            current_state->playing=is_playing();
        } else if (inp=='B'||inp=='b') {
            add_command("Back (Music Control)");
            res=PLAYLIST;
            break;
        } else if (inp=='Q'||inp=='q') {
            add_command("Quit (Music Control)");
            stop_playing();
            current_state->playing_song_id=0;
            current_state->playing=0;
            break;
        }
    }
    return res;
}