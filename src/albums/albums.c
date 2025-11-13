#include <stdio.h>
#include <stdlib.h>
#include "utils/utils.h"
#include <dirent.h>
#include <string.h>
#include "albums.h"

Album *parse_album_file(char *f_name) {
    FILE *f=fopen(f_name,"r");
    long long id;
    char *title=malloc(250);
    int song_count;
    fscanf(f,"%lld\n%[^\n]\n%d",&id,title,&song_count);
    long long *song_ids=malloc(sizeof(long long)*song_count);
    for (int i=0; i<song_count; i++) {
        fscanf(f,"%lld",&song_ids[i]);
    }
    fclose(f);
    Album *album=malloc(sizeof(Album));
    album->id=id;
    album->title=title;
    album->song_count=song_count;
    album->song_ids=song_ids;
    album->prev=NULL;
    album->next=NULL;
    return album;
}

Album *load_albums() {
    char dir_name[100];
    strcpy(dir_name,BASE_DIR);
    strcat(dir_name,"/albums");
    DIR *albums_dir=opendir(dir_name);
    if (albums_dir==NULL) {
        make_dir(dir_name);
        albums_dir=opendir(dir_name);
    }
    struct dirent *album_file=readdir(albums_dir);
    Album *albums=NULL;
    Album *album=NULL;
    while (album_file!=NULL) {
        if (strcmp(album_file->d_name,".")==0||strcmp(album_file->d_name,"..")==0) {
            album_file=readdir(albums_dir);
            continue;
        }
        char album_name[356];
        strcpy(album_name,BASE_DIR);
        strcat(album_name,"/albums/");
        strcat(album_name,album_file->d_name);
        Album *next_album=parse_album_file(album_name);
        if (albums==NULL) {
            albums=next_album;
            album=next_album;
        } else {
            album->next=next_album;
            next_album->prev=album;
            album=next_album;
        }
        album_file=readdir(albums_dir);
    }
    closedir(albums_dir);
    return albums;
}

Album *create_new_album(Album *albums, long long id, char *title, int song_count, long long *song_ids) {
    char f_name[250];
    sprintf(f_name,"%s/albums/%lld.txt",BASE_DIR,id);
    FILE *f=fopen(f_name,"w");
    fprintf(f,"%lld\n",id);
    fprintf(f,"%s\n",title);
    fprintf(f,"%d\n",song_count);
    for (int i=0; i<song_count; i++) {
        fprintf(f,"%lld\n",song_ids[i]);
    }
    fclose(f);
    Album *album=parse_album_file(f_name);
    if (albums==NULL) {
        albums=album;
    } else {
        Album *end=albums;
        while (end->next!=NULL) {
            end=end->next;
        }
        end->next=album;
        album->prev=end;
    }
    return albums;
}

void write_album_file(Album *album) {
    char f_name[250];
    sprintf(f_name,"%s/albums/%lld.txt",BASE_DIR,album->id);
    FILE *f=fopen(f_name,"w");
    fprintf(f,"%lld\n",album->id);
    fprintf(f,"%s\n",album->title);
    fprintf(f,"%d\n",album->song_count);
    for (int i=0; i<album->song_count; i++) {
        fprintf(f,"%lld\n",album->song_ids[i]);
    }
    fclose(f);
}

void add_song_to_album(Album *album, long long song_id) {
    album->song_count++;
    album->song_ids=realloc(album->song_ids,album->song_count*sizeof(long long));
    album->song_ids[album->song_count-1]=song_id;
    write_album_file(album);
}

void remove_song_from_album(Album *album, long long song_id) {
    int shift=0;
    for (int i=0; i<album->song_count-1; i++) {
        if (album->song_ids[i]==song_id) {
            shift=1;
        }
        if (shift) {
            album->song_ids[i]=album->song_ids[i+1];
        }
    }
    if (!shift) return;
    album->song_count--;
    album->song_ids=realloc(album->song_ids,album->song_count*sizeof(long long));
    write_album_file(album);
}

Album *delete_album(Album *albums, long long id) {
    Album *album=albums;
    Album *ret=NULL;
    while (album!=NULL) {
        if (album->id==id) {
            if (album->prev!=NULL) album->prev->next=album->next;
            if (album->next!=NULL) album->next->prev=album->prev;
            if (album->prev==NULL) {
                if (album->next==NULL) {
                    ret=NULL;
                } else {
                    ret=album->next;
                }
            } else {
                ret=albums;
            }
            char f_name[250];
            sprintf(f_name,"%s/albums/%lld.txt",BASE_DIR,album->id);
            remove(f_name);
            free(album->title);
            free(album->song_ids);
            free(album);
            break;
        }
        album=album->next;
    }
    return ret;
}