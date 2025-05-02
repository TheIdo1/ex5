/******************
Name: Ido Itzhak
ID: 324074012
Assignment: Ex5
*******************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Song {
    char *title;
    char *artist;
    int year;
    char *lyrics;
    int streams;
} Song;

typedef struct Playlist {
    char *name;
    Song **songs;
    int songsNum;
} Playlist;

void printPlaylistsMenu();

void getPlaylistName(Playlist *playlist);

int choosePlaylist(Playlist *allPlaylists, int numOfPlaylists);

void usePlaylist(Playlist *playlist);

void addSong(Playlist *playlist);

void getSongName(Playlist *playlist);

void getArtistName(Playlist *playlist);

void getLyrics(Playlist *playlist);

void playSong(Playlist *playlist, int songChoice);

void deleteSong(Playlist *playlist, int songIndex);

void freeSong(Playlist *playlist, int songIndex);

void toLastSong(Playlist *playlist, int songIndex);

void swapSong(Playlist *playlist, int index1, int index2);

void sortByYear(Playlist *playlist);

void helperSortByYear(Playlist *playlist, Song **songs, int length, int start);

int minYear(Song **songs, int start, int end);

void sortByStreamsUp(Playlist *playlist);

void helperSortByStreamsUp(Playlist *playlist, Song **songs, int length, int start);

int minStreams(Song **songs, int start, int end);

void sortByStreamsDown(Playlist *playlist);

void helperSortByStreamsDown(Playlist *playlist, Song **songs, int length, int start);

int maxStreams(Song **songs, int start, int end);

void sortByAlphabet(Playlist *playlist);

void helperSortByAlphabet(Playlist *playlist, Song **songs, int length, int start);

int minLetter(Song **songs, int start, int end);

char toLower(char letter);

void removePlaylist(Playlist *allPlaylists, int choice, int numOfPlaylists);

void freePlaylist(Playlist *playlist);

void freeAllSongs(Playlist *playlist);

void toLastPlaylist(Playlist *allPlaylists, int playlistIndex, int numOfPlaylists);

void swapPlaylists(Playlist *allPlaylists, int index1, int index2);


int main() {
    int mainChoice, numOfPlaylists = 0;
    Playlist *allPlaylists;
    // if (!allPlaylists) {
    //     exit(1);
    // }
    do {
        printPlaylistsMenu();
        scanf(" %d", &mainChoice);
        switch (mainChoice) {
            //Watch playlists
            case 1:
                while (1) {
                    int addPlaylistChoice = choosePlaylist(allPlaylists, numOfPlaylists);
                    if (addPlaylistChoice == numOfPlaylists) {
                        break;
                    }
                    usePlaylist(&allPlaylists[addPlaylistChoice]);
                }
                break;

            // add playlists
            case 2:
                if (numOfPlaylists == 0) {
                    allPlaylists = malloc((sizeof(Playlist)));
                }
                if (numOfPlaylists > 0) {
                    allPlaylists = realloc(allPlaylists, (numOfPlaylists + 1) * sizeof(Playlist));
                    if (!allPlaylists) {
                        exit(1);
                    }
                }
                printf("Enter playlist's name:\n");
            // -1 because the first playlist is 0.
                getPlaylistName(&allPlaylists[numOfPlaylists]);
            //initialize playlist
                allPlaylists[numOfPlaylists].songsNum = 0;
                allPlaylists[numOfPlaylists].songs = malloc(sizeof(Song *));
                numOfPlaylists++;
                break;

            // remove playlists
            case 3:
                int removePlaylistChoice = choosePlaylist(allPlaylists, numOfPlaylists);
                if (removePlaylistChoice == numOfPlaylists) {
                    break;
                }
                removePlaylist(allPlaylists, removePlaylistChoice, numOfPlaylists);
                if (numOfPlaylists > 1) {
                    allPlaylists = realloc(allPlaylists, (numOfPlaylists - 1) * sizeof(Playlist));
                    if(!allPlaylists) {
                        exit(1);
                    }
                }
                if(numOfPlaylists==1) {
                    free(allPlaylists);
                }
                numOfPlaylists--;
                printf("Playlist deleted.\n");
                break;
            case 4:
                break;
            default:
                printf("Invalid option\n");
                break;
        }
    } while (mainChoice != 4);

    for (int i = 0; i < numOfPlaylists; i++) {
        freePlaylist(&allPlaylists[i]);
    }
    free(allPlaylists);
    printf("Goodbye!\n");
}

void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

//use of "magic numbers" in order for the test to work in the submit...
void getPlaylistName(Playlist *playlist) {
    //get user input to buffer.
    scanf(" ");
    int numOfChars = 0;
    char *temp = malloc(sizeof(char));
    if(!temp) {
        exit(1);
    }
    char tempChar;
    while (1) {
        scanf("%c", &tempChar);
        //means we got to the end of the input.
        if (tempChar == '\n') {
            break;
        }
        numOfChars++;
        temp = (char *) realloc(temp, (numOfChars + 1) * sizeof(char));
        if(!temp) {
            exit(1);
        }
        //because we start counting from 0, and take into account \0
        temp[numOfChars - 1] = tempChar;
    }
    //-1 so it will work in submit..
    temp[numOfChars - 1] = '\0';
    //copy string to the right place, free unused memory.
    playlist->name = (char *) malloc((numOfChars + 1) * sizeof(char));
    if(!playlist->name) {
        exit(1);
    }
    strcpy(playlist->name, temp);
    free(temp);
}

int choosePlaylist(Playlist *allPlaylists, int numOfPlaylists) {
    int choice;
    while (1) {
        printf("Choose a playlist:\n");
        for (int i = 0; i < numOfPlaylists; i++) {
            printf("\t%d. %s\n", i + 1, allPlaylists[i].name);
        }
        printf("\t%d. Back to main menu\n", numOfPlaylists + 1);
        scanf("%d", &choice);
        if (choice > 0 && choice <= numOfPlaylists + 1) {
            //-1 to compensate on the fact that we printed from 1, but counting from 0
            return choice - 1;
        }
        printf("Invalid option\n");
    }
}

void usePlaylist(Playlist *playlist) {
    int choice;
    printf("playlist %s:\n", playlist->name);
    while (1) {
        printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
        scanf("%d", &choice);
        switch (choice) {
            //show playlist
            case 1:
                int songChoice = 0;
                for (int i = 0; i < playlist->songsNum; i++) {
                    printf("%d. Title: %s\n\tArtist: %s\n\tReleased: %d\n\tStreams: %d\n\n", i + 1,
                           playlist->songs[i]->title, playlist->songs[i]->artist, playlist->songs[i]->year,
                           playlist->songs[i]->streams);
                }
                do {
                    printf("choose a song to play, or 0 to quit:\n");
                    scanf("%d", &songChoice);
                    if (songChoice == 0) {
                        break;
                    }
                    if (songChoice < 0 || songChoice > playlist->songsNum) {
                        printf("Invalid choice\n");
                        break;
                    }
                    playSong(playlist, songChoice - 1);
                } while (songChoice != 0);
                break;
            // add song
            case 2:
                addSong(playlist);
                break;
            // delete song
            case 3:
                int deleteSongChoice;
                for (int i = 0; i < playlist->songsNum; i++) {
                    printf("%d. Title: %s\n\tArtist: %s\n\tReleased: %d\n\tStreams: %d\n\n", i + 1,
                           playlist->songs[i]->title, playlist->songs[i]->artist, playlist->songs[i]->year,
                           playlist->songs[i]->streams);
                }
                printf("choose a song to delete, or 0 to quit:\n");
                scanf("%d", &deleteSongChoice);
                if (deleteSongChoice == 0) {
                    break;
                }
                deleteSong(playlist, deleteSongChoice - 1);
                printf("Song deleted successfully.\n");
                break;

            //sort
            case 4:
                int sortChoice;
                printf(
                    "choose:\n\t1. sort by year\n\t2. sort by streams - ascending order"
                    "\n\t3. sort by streams - descending order\n\t4. sort alphabetically\n");
                scanf("%d", &sortChoice);
                switch (sortChoice) {
                    //sort by year
                    case 1:
                        sortByYear(playlist);
                        break;

                    //sort playlist by streams - ascending order
                    case 2:
                        sortByStreamsUp(playlist);
                        break;

                    //sort playlist by streams - descending order
                    case 3:
                        sortByStreamsDown(playlist);
                        break;

                    // sort playlist alphabetically.
                    case 4:
                        sortByAlphabet(playlist);
                        break;

                    default:
                        break;
                }
                printf("sorted\n");
                break;

            //play all
            case 5:
                for (int i = 0; i < playlist->songsNum; i++) {
                    playSong(playlist, i);
                }
                break;
            // exit
            case 6:
                return;
            default:
                printf("Invalid option\n");
                break;
        }
    }
}

void addSong(Playlist *playlist) {
    playlist->songs = realloc(playlist->songs, (playlist->songsNum + 1) * sizeof(Song *));
    if(!playlist->songs) {
        exit(1);
    }
    playlist->songs[playlist->songsNum] = malloc(sizeof(Song));
    if(!playlist->songs[playlist->songsNum]) {
        exit(1);
    }
    printf("Enter song's details\n");
    printf("Title:\n");
    getSongName(playlist);
    printf("Artist:\n");
    getArtistName(playlist);
    printf("Year of release:\n");
    scanf("%d", &(playlist->songs[playlist->songsNum]->year));
    printf("Lyrics:\n");
    getLyrics(playlist);
    //initialize song streams.
    playlist->songs[playlist->songsNum]->streams = 0;
    playlist->songsNum++;
}

void getSongName(Playlist *playlist) {
    //get user input to buffer.
    scanf(" ");
    // start from 1 to have enough space to '\0'
    int numOfChars = 0;
    char *temp = malloc(sizeof(char));
    if(!temp) {
        exit(1);
    }
    char tempChar;
    while (1) {
        scanf("%c", &tempChar);
        //means we got to the end of the input.
        if (tempChar == '\n') {
            break;
        }
        numOfChars++;
        temp = (char *) realloc(temp, (numOfChars + 1) * sizeof(char));
        if(!temp) {
            exit(1);
        }
        //because we start counting from 0, and take into account \0
        temp[numOfChars - 1] = tempChar;
    }
    //-1 so it will work in submit..
    temp[numOfChars] = '\0';
    //copy string to the right place, free unused memory.
    playlist->songs[playlist->songsNum]->title = (char *) malloc((numOfChars + 1) * sizeof(char));
    strcpy(playlist->songs[playlist->songsNum]->title, temp);
    free(temp);
}

void getArtistName(Playlist *playlist) {
    //get user input to buffer.
    scanf(" ");
    // start from 1 to have enough space to '\0'
    int numOfChars = 1;
    char *temp = malloc(sizeof(char));
    if(!temp) {
        exit(1);
    }
    char tempChar;
    while (1) {
        scanf("%c", &tempChar);
        //means we got to the end of the input.
        if (tempChar == '\n') {
            break;
        }
        numOfChars++;
        temp = (char *) realloc(temp, numOfChars * sizeof(char));
        if(!temp) {
            exit(1);
        }
        //because we start counting from 0, and take into account \0
        temp[numOfChars - 2] = tempChar;
    }
    temp[numOfChars - 1] = '\0';
    //copy string to the right place, free unused memory.
    playlist->songs[playlist->songsNum]->artist = (char *) malloc(numOfChars * sizeof(char));
    strcpy(playlist->songs[playlist->songsNum]->artist, temp);
    free(temp);
}

void getLyrics(Playlist *playlist) {
    //get user input to buffer.
    scanf(" ");
    // start from 1 to have enough space to '\0'
    int numOfChars = 1;
    char *temp = malloc(sizeof(char));
    if(!temp) {
        exit(1);
    }
    char tempChar;
    while (1) {
        scanf("%c", &tempChar);
        //means we got to the end of the input.
        if (tempChar == '\n') {
            break;
        }
        numOfChars++;
        temp = (char *) realloc(temp, numOfChars * sizeof(char));
        if(!temp) {
            exit(1);
        }
        //because we start counting from 0, and take into account \0
        temp[numOfChars - 2] = tempChar;
    }
    temp[numOfChars - 1] = '\0';
    //copy string to the right place, free unused memory.
    playlist->songs[playlist->songsNum]->lyrics = (char *) malloc(numOfChars * sizeof(char));
    strcpy(playlist->songs[playlist->songsNum]->lyrics, temp);
    free(temp);
}

void playSong(Playlist *playlist, int songChoice) {
    printf("Now playing %s:\n", playlist->songs[songChoice]->title);
    printf("$ %s $\n", playlist->songs[songChoice]->lyrics);
    playlist->songs[songChoice]->streams++;
}

void deleteSong(Playlist *playlist, int songIndex) {
    toLastSong(playlist, songIndex);
    //removed song is now placed in last position; freeing its memory. (-1 because counting from 0)
    freeSong(playlist, playlist->songsNum - 1);
    playlist->songs = realloc(playlist->songs, (playlist->songsNum - 1) * sizeof(Song *));
    if(!playlist->songs) {
        exit(1);
    }
    playlist->songsNum--;
}

void freeSong(Playlist *playlist, int songIndex) {
    free(playlist->songs[songIndex]->title);
    free(playlist->songs[songIndex]->artist);
    free(playlist->songs[songIndex]->lyrics);
    free(playlist->songs[songIndex]);
}

void toLastSong(Playlist *playlist, int songIndex) {
    for (int i = songIndex + 1; i < playlist->songsNum; i++) {
        swapSong(playlist, i, i - 1);
    }
}

void swapSong(Playlist *playlist, int index1, int index2) {
    Song *temp;
    temp = playlist->songs[index1];
    playlist->songs[index1] = playlist->songs[index2];
    playlist->songs[index2] = temp;
}

//sort playlist by year (low to high)
void sortByYear(Playlist *playlist) {
    helperSortByYear(playlist, playlist->songs, playlist->songsNum, 0);
}

void helperSortByYear(Playlist *playlist, Song **songs, int length, int start) {
    if (start == length) {
        return;
    }
    int minIndex = minYear(songs, start, length);
    swapSong(playlist, start, minIndex);
    helperSortByYear(playlist, songs, length, start + 1);
}

int minYear(Song **songs, int start, int end) {
    int temp = start;
    for (int i = start; i < end; i++) {
        if (songs[i]->year < songs[temp]->year) {
            temp = i;
        }
    }
    return temp;
}

//Sort by streams, low to high
void sortByStreamsUp(Playlist *playlist) {
    helperSortByStreamsUp(playlist, playlist->songs, playlist->songsNum, 0);
}

void helperSortByStreamsUp(Playlist *playlist, Song **songs, int length, int start) {
    if (start == length) {
        return;
    }
    int minIndex = minStreams(songs, start, length);
    swapSong(playlist, start, minIndex);
    helperSortByStreamsUp(playlist, songs, length, start + 1);
}

int minStreams(Song **songs, int start, int end) {
    int temp = start;
    for (int i = start; i < end; i++) {
        if (songs[i]->streams < songs[temp]->streams) {
            temp = i;
        }
    }
    return temp;
}

//Sort by streams, high to low
void sortByStreamsDown(Playlist *playlist) {
    helperSortByStreamsDown(playlist, playlist->songs, playlist->songsNum, 0);
}

void helperSortByStreamsDown(Playlist *playlist, Song **songs, int length, int start) {
    if (start == length) {
        return;
    }
    int maxIndex = maxStreams(songs, start, length);
    swapSong(playlist, start, maxIndex);
    helperSortByStreamsDown(playlist, songs, length, start + 1);
}

int maxStreams(Song **songs, int start, int end) {
    int temp = start;
    for (int i = start; i < end; i++) {
        if (songs[i]->streams > songs[temp]->streams) {
            temp = i;
        }
    }
    return temp;
}

//Sort alphabetically
void sortByAlphabet(Playlist *playlist) {
    helperSortByAlphabet(playlist, playlist->songs, playlist->songsNum, 0);
}

void helperSortByAlphabet(Playlist *playlist, Song **songs, int length, int start) {
    if (start == length) {
        return;
    }
    int minIndex = minLetter(songs, start, length);
    swapSong(playlist, start, minIndex);
    helperSortByAlphabet(playlist, songs, length, start + 1);
}

int minLetter(Song **songs, int start, int end) {
    int temp = start;
    for (int i = start; i < end; i++) {
        if (toLower(songs[i]->title[0]) < toLower(songs[temp]->title[0])) {
            temp = i;
        }
    }
    return temp;
}

char toLower(char letter) {
    if (letter >= 'A' && letter <= 'Z') {
        letter = letter + ('a' - 'A');
    }
    return letter;
}

//remove Playlist
void removePlaylist(Playlist *allPlaylists, int choice, int numOfPlaylists) {
    toLastPlaylist(allPlaylists, choice, numOfPlaylists);
    //removed song is now placed in last position; freeing its memory. (-1 because counting from 0)
    freePlaylist(&allPlaylists[numOfPlaylists - 1]);
}

void freePlaylist(Playlist *playlist) {
    free(playlist->name);
    freeAllSongs(playlist);
    free(playlist->songs);
}

void freeAllSongs(Playlist *playlist) {
    for (int i = 0; i < playlist->songsNum; i++) {
        freeSong(playlist, i);
    }
}

void toLastPlaylist(Playlist *allPlaylists, int playlistIndex, int numOfPlaylists) {
    for (int i = playlistIndex + 1; i < numOfPlaylists; i++) {
        swapPlaylists(allPlaylists, i - 1, i);
    }
}

void swapPlaylists(Playlist *allPlaylists, int index1, int index2) {
    Playlist temp;
    //copy index1 to temp
    temp.name = malloc((strlen(allPlaylists[index1].name) * sizeof(char)) + 1);
    if(!temp.name) {
        exit(1);
    }
    strcpy(temp.name, allPlaylists[index1].name);
    temp.songs = malloc(allPlaylists[index1].songsNum * (sizeof(Song *)));
    if(!temp.songs) {
        exit(1);
    }
    temp.songsNum = allPlaylists[index1].songsNum;
    for (int i = 0; i < temp.songsNum; i++) {
        temp.songs[i] = allPlaylists[index1].songs[i];
    }

    //copy index2 to index1
    allPlaylists[index1].name = realloc(allPlaylists[index1].name, strlen(allPlaylists[index2].name) + 1);
    if(!allPlaylists[index1].name) {
        exit(1);
    }
    strcpy(allPlaylists[index1].name, allPlaylists[index2].name);
    allPlaylists[index1].songs = realloc(allPlaylists[index1].songs, allPlaylists[index2].songsNum * sizeof(Song *));
    if(!allPlaylists[index1].songs) {
        exit(1);
    }
    for (int i = 0; i < allPlaylists[index2].songsNum; i++) {
        allPlaylists[index1].songs[i] = allPlaylists[index2].songs[i];
    }
    allPlaylists[index1].songsNum = allPlaylists[index2].songsNum;
    //copy temp to index2
    allPlaylists[index2].name = realloc(allPlaylists[index2].name, strlen(temp.name) + 1);
    if(!allPlaylists[index2].name) {
        exit(1);
    }
    strcpy(allPlaylists[index2].name, temp.name);
    allPlaylists[index2].songs = realloc(allPlaylists[index2].songs, temp.songsNum * sizeof(Song *));
    if(!allPlaylists[index2].songs) {
        exit(1);
    }
    for (int i = 0; i < temp.songsNum; i++) {
        allPlaylists[index2].songs[i] = temp.songs[i];
    }
    allPlaylists[index2].songsNum = temp.songsNum;
    //free temp
    free(temp.name);
    free(temp.songs);
}
