#include <stdio.h>
#include <stdlib.h>
#include "utils/utils.h"
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include "songs.h"
#include "albums/albums.h"

Song *parse_song_file(char *f_name) {
    FILE *f=fopen(f_name,"r");
    long long id;
    char *title=malloc(250), *artist=malloc(250), *audio_loc=malloc(350);
    int duration;
    fscanf(f,"%lld\n%[^\n]\n%[^\n]\n%d\n%[^\n]",&id,title,artist,&duration,audio_loc);
    fclose(f);
    Song *song=malloc(sizeof(Song));
    song->id=id;
    song->title=title;
    song->artist=artist;
    song->duration=duration;
    song->audio_loc=audio_loc;
    song->loaded=0;
    song->prev=NULL;
    song->next=NULL;
    return song;
}

Song *load_songs() {
    char dir_name[100];
    strcpy(dir_name,BASE_DIR);
    strcat(dir_name,"/songs");
    DIR *songs_dir=opendir(dir_name);
    if (songs_dir==NULL) {
        make_dir(dir_name);
        songs_dir=opendir(dir_name);
    }
    char audio_dir[150];
    strcpy(audio_dir,BASE_DIR);
    strcat(audio_dir,"/audios");
    DIR *audios=opendir(audio_dir);
    if (audios==NULL) {
        make_dir(audio_dir);
    }
    closedir(audios);
    struct dirent *song_file=readdir(songs_dir);
    Song *songs=NULL;
    Song *song=NULL;
    while (song_file!=NULL) {
        if (strcmp(song_file->d_name,".")==0||strcmp(song_file->d_name,"..")==0) {
            song_file=readdir(songs_dir);
            continue;
        }
        char song_name[356];
        strcpy(song_name,BASE_DIR);
        strcat(song_name,"/songs/");
        strcat(song_name,song_file->d_name);
        Song *next_song=parse_song_file(song_name);
        if (songs==NULL||song==NULL) {
            songs=next_song;
            song=next_song;
        } else {
            song->next=next_song;
            next_song->prev=song;
            song=next_song;
        }
        song_file=readdir(songs_dir);
    }
    closedir(songs_dir);
    return songs;
}

Song *create_new_song(Song *songs, char *title, char *artist, int dur, char *loc) {
    char f_name[250];
    char id[100];
    sprintf(id,"%d%lld%lld",toupper(title[0]),((long long)rand()*rand())%9000+1000,((long long)rand()*rand())%9000+1000);
    sprintf(f_name,"%s/songs/%s.txt",BASE_DIR,id);
    FILE *f=fopen(f_name,"w");
    fprintf(f,"%s\n",id);
    fprintf(f,"%s\n",title);
    fprintf(f,"%s\n",artist);
    fprintf(f,"%d\n",dur);
    fprintf(f,"%s\n",loc);
    fclose(f);
    Song *song=parse_song_file(f_name);
    if (songs==NULL) {
        songs=song;
    } else {
        Song *aft=songs;
        if (atoll(id)<aft->id) {
            song->next=songs;
            songs->prev=song;
            songs=song;
        } else {
            while (aft->next!=NULL) {
                if (atoll(id)>aft->id&&atoll(id)<aft->next->id) break;
                aft=aft->next;
            }
            song->next=aft->next;
            song->prev=aft;
            aft->next=song;
        }
    }
    return songs;
}

Song *delete_song(Song *songs, long long id, Album *albums) {
    Song *song=songs;
    Song *ret=NULL;
    while (song!=NULL) {
        if (song->id==id) {
            if (song->prev!=NULL) song->prev->next=song->next;
            if (song->next!=NULL) song->next->prev=song->prev;
            if (song->prev==NULL) {
                if (song->next==NULL) {
                    ret=NULL;
                } else {
                    ret=song->next;
                }
            } else {
                ret=songs;
            }
            char f_name[250];
            sprintf(f_name,"%s/songs/%lld.txt",BASE_DIR,song->id);
            remove(f_name);
            free(song->title);
            free(song->artist);
            free(song->audio_loc);
            free(song);
            break;
        }
        song=song->next;
    }
    while (albums!=NULL) {
        remove_song_from_album(albums,id);
        albums=albums->next;
    }
    return ret;
}