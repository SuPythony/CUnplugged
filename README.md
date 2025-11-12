# C-Unlugged
A C based music player and browser.

## Features:

- Add and remove songs in your library.
- Create and delete albums (playlists).
- Create playlist (play queue) for the current run.
- Play songs in background.
- Automatically switch to next song on finishing.
- Logs all user commands in log.txt
- Tested on both Windows and Linux

## How to use

- It is recommended to make the console window full-screen.
- Use the make command to compile the project.
- On startup, wait for the player to load up.
- Select the section you want to navigate to in the library.
- You can manage your songs in the Songs section and all your albums in the Albums section.
- All data is stored (including the log) using files in the library folder created in the same directory as the executable file.
- You can change the directory to store the files changing the BASE_DIR definition in utils.h.
- You need to store the audio files in the audios folder in library.
- Create the playlist in the Playlist section.
- In the music controls, you can play the songs and manage playback.
- When you leave music controls, the songs will continue playing in the background, and proceed as per the playlist.

## Disclaimer

AI was only used for help with the Makefile (making it work on both OS). The miniaudio library was used for playing the audio. Everything else was done without AI and uses no external libraries.
