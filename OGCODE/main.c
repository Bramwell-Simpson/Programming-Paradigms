#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

struct ID3v1Tag {
    char tag[3];
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[30];
    int genre;
};

void printID3v1Tag(const struct ID3v1Tag *tag) {
    printf("\nTitle: %s\n", tag->title);
    printf("Artist: %s\n", tag->artist);
    printf("Album: %s\n", tag->album);
    printf("Year: %s\n", tag->year);
    printf("Comment: %s\n", tag->comment);
    printf("Genre: %d\n", tag->genre);
}

struct ID3v1Tag readID3v1Tag(const char *mp3File, struct ID3v1Tag *tag, int print) {
    FILE *file = fopen(mp3File, "rb");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file, exiting.\n");
        struct ID3v1Tag emptyTag;
        return emptyTag;
    }

    fseek(file, -128, SEEK_END);
    fread(tag, sizeof(struct ID3v1Tag), 1, file);

    if (strncmp(tag->tag, "TAG", 3) == 0 && print == 1) {
        printID3v1Tag(tag);
        return *tag;
    } 
    else if(strncmp(tag->tag, "TAG", 3) == 0 && print == 0) {
        return *tag;
    } else {
        fprintf(stderr, "No ID3v1 Tag found!\n");
    }

    fclose(file);
}

void writeID3v1Tag(const char *mp3File, const struct ID3v1Tag *newTag) {
    FILE *file = fopen(mp3File, "rb+");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file, exiting.\n");
        return;
    }

    fseek(file, -128, SEEK_END);
    struct ID3v1Tag tag;
    fread(&tag, sizeof(struct ID3v1Tag), 1, file);

    if (strncmp(tag.tag, "TAG", 3) == 0) {
        printf("Tag Found, continuing...\n");
    } else {
        fprintf(stderr, "No ID3v1 Tag found!\n");
        fclose(file);
        return;
    }

    fseek(file, -128, SEEK_END);

    if (fwrite(newTag, sizeof(struct ID3v1Tag), 1, file) != 1) {
        fprintf(stderr, "Error writing the modified tag.\n");
    } else {
        printf("Changes made successfully\n");
    }

    fclose(file);
} 

int checkEmpty(char temp[30]) {

    if(*temp == '\0') {
        return 1;
    }

    return 0;
}

int main() {
    char question[100];
    struct ID3v1Tag tag, newTag;

    char location[1039] = "../resources/";

    while (1) {
        printf("What would you like to do (read/write/quit) > ");
        fgets(question, sizeof(question), stdin);

        if (strncmp(question, "read", 4) == 0) {
            char mp3File[1025];
            printf("Enter file name: ");
            fgets(mp3File, sizeof(mp3File), stdin);
            mp3File[strcspn(mp3File, "\n")] = 0; // Remove newline character
            
            strcat(location,  mp3File);
            readID3v1Tag(location, &tag, 1);
        }
        else if (strncmp(question, "write", 5) == 0) {
            char mp3File[1025];
            printf("Enter file name: ");
            fgets(mp3File, sizeof(mp3File), stdin);
            mp3File[strcspn(mp3File, "\n")] = 0; // Remove newline character
            
            strcat(location, mp3File);
            struct ID3v1Tag oldTag = readID3v1Tag(location, &tag, 0);

            if (strncmp(oldTag.tag, "TAG", 3) == 0) {
                printf("Tag Found, continuing...\n");
                printf("Press enter to skip to next field\n");
            } else {
                fprintf(stderr, "No ID3v1 Tag found!\n");
                continue;
            }

            char temp[30];

            strcpy(newTag.tag, oldTag.tag);

            //Gets the field, removes the newline and checks if the field is empty or not
            printf("Enter the new title: ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0; // Remove newline character
            checkEmpty(temp)? strcpy(newTag.title, oldTag.title) : strncpy(newTag.title, temp, sizeof(newTag.title));

            printf("Enter the new artist: ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0; 
            checkEmpty(temp)? strcpy(newTag.artist, oldTag.artist) : strncpy(newTag.artist, temp, sizeof(newTag.artist));

            printf("Enter the new album name: ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0; 
            checkEmpty(temp)? strcpy(newTag.album, oldTag.album) : strncpy(newTag.album, temp, sizeof(newTag.album));

            printf("Enter the new year of release: ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0; 
            checkEmpty(temp)? strcpy(newTag.year, oldTag.year) : strncpy(newTag.year, temp, sizeof(newTag.year));

            printf("Enter the new comment: ");
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0; 
            checkEmpty(temp)? strcpy(newTag.comment, oldTag.comment) : strncpy(newTag.comment, temp, sizeof(newTag.comment));

            int newGenre;
            char genre;
            printf("Enter the new genre (1-79): ");
            
            fgets(temp, sizeof(temp), stdin);
            temp[strcspn(temp, "\n")] = 0;
            newGenre = atoi(temp);
            // Check if the input is not empty, and update newTag.genre accordingly
            if (!checkEmpty(temp)) {
                // Assign the value to a temporary variable and then to newTag.genre
                int tempGenre = newGenre;
                newTag.genre = tempGenre;
            } else {
                // If input is empty, use the oldTag.genre value
                newTag.genre = oldTag.genre;
            }

            printID3v1Tag(&newTag);
            writeID3v1Tag(location, &newTag);
        }
        else if (strncmp(question, "quit", 4) == 0) {
            break;
        }
        else {
            printf("Unknown input\n");
        }
    }

    return 0;
}
