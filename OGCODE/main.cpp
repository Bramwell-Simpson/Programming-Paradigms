#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <string>
#include <cctype>
#include <cstring>

struct ID3v1Tag {
    char tag[3];
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[30];
    uint8_t genre;
};

int main() {

    std::string question;
    char resLoc[14] = "../resources/";
    
    while(true) {

        question = "";
        std::cout << "What would you like to do (read/write/quit) > ";
        std::getline(std::cin, question);

        if(question == "read") {

            char mp3File[1025];
            int fileSize;

            //Get file to change tags on
            std::cout << "Enter file name: " << std::endl;
            std::cin.getline(mp3File, 1024);

            std::string parsedLocation = std::string(resLoc) + mp3File;

            //Get size of the file
            try {
                fileSize = std::filesystem::file_size(parsedLocation);
                std::cout << "Size of the file in bytes is: " << fileSize << std::endl;
            }
            catch(std::filesystem::filesystem_error e) {
                std::cerr << "Failure: " << e.what() << std::endl;
                break;
            }

            //Initial size aquired, opening the file and checking for errors
            std::ifstream file(parsedLocation, std::ios::binary);
            if(!file.is_open()) {
                std::cerr << "Unable to open file, exiting." << std::endl;
                return 1;
            }

            //Seek to the last 128 bytes of the file
            file.seekg(-128, std::ios::end);
            
            ID3v1Tag tag;
            file.read(reinterpret_cast<char*>(&tag), sizeof(tag));

            if(strncmp(tag.tag, "TAG", 3) == 0) {
                std::cout << "Title: " << tag.title << std::endl;
                std::cout << "Artist: " << tag.artist << std::endl;
                std::cout << "Album: " << tag.album << std::endl;
                std::cout << "Year: " << tag.year << std::endl;
                std::cout << "Comment: " << tag.comment << std::endl;
                std::cout << "Genre: " << static_cast<int>(tag.genre) << std::endl;
            }
            else {
                std::cerr << "No ID3v1 Tag found!" << std::endl;
            } 

            file.close();
        }
        else if(question == "write") {
            
            //Same start as the READ process, except instead of printing it, let's put it into a new tag
            char mp3File[1025];
            int fileSize;
            //Get file to change tags on

            std::cout << "Enter file name: " << std::endl;
            std::cin.getline(mp3File, 1024);

            std::string parsedLocation = std::string(resLoc) + mp3File;

            //Get size of the file
            fileSize = std::filesystem::file_size(parsedLocation);
            std::cout << "Size of the file in bytes is: " << fileSize << std::endl;

            //Initial size aquired, opening the file and checking for errors
            std::fstream file(parsedLocation, std::ios::in | std::ios::out | std::ios::binary);
            if(!file.is_open()) {
                std::cerr << "Unable to open file, exiting." << std::endl;
                return 1;
            }

            //Seek to the last 128 bytes of the file
            file.seekp(-128, std::ios::end);
            
            ID3v1Tag tag;
            file.read(reinterpret_cast<char*>(&tag), sizeof(tag));

            if(strncmp(tag.tag, "TAG", 3) == 0) {
                //TAG FOUND
                std::cout << "Tag Found, continuing..." << std::endl;
            }
            else {
                std::cerr << "No ID3v1 Tag found!" << std::endl;
            }

            ID3v1Tag newTag;
            char temp[30];

            strcpy(newTag.tag, tag.tag);

            std::cout << "Enter the new title (leave blank for no changes): ";
            std::cin.getline(temp, sizeof newTag.title);
            if (temp[0] == '\0' || temp[0] == '\n') {
                strcpy(newTag.title, tag.title);
            } else {
                strcpy(newTag.title, temp);
            }

            std::cout << "Enter the new Artist: ";
            std::cin.getline(temp, sizeof newTag.artist);
            if (temp[0] == '\0' || temp[0] == '\n') {
                strcpy(newTag.artist, tag.artist);
            } else {
                strcpy(newTag.artist, temp);
            }

            std::cout << "Enter the new album name: ";
            std::cin.getline(temp, sizeof newTag.album);
            if (temp[0] == '\0' || temp[0] == '\n') {
                strcpy(newTag.album, tag.album);
            } else {
                strcpy(newTag.album, temp);
            }

            std::cout << "Enter the new year of release: ";
            std::cin.getline(temp, sizeof newTag.year);
            if (temp[0] == '\0' || temp[0] == '\n') {
                strcpy(newTag.year, tag.year);
            } else {
                strcpy(newTag.year, temp);
            }

            std::cout << "Enter a new comment: ";
            std::cin.getline(temp, sizeof newTag.comment);
            if (temp[0] == '\0' || temp[0] == '\n') {
                strcpy(newTag.comment, tag.comment);
            } else {
                strcpy(newTag.comment, temp);
            }

            std::cout << "Enter the genre (refer to ID3v1 Genre list): ";
            std::string genreInput;
            std::getline(std::cin, genreInput);
            if (genreInput.length() == 0) {
                // Genre input is empty, copy the old genre to the newTag
                newTag.genre = tag.genre;
            } else {
                // Genre input is not empty, take the first character as the genre
                newTag.genre = static_cast<uint8_t>(std::stoi(genreInput));
            }

            //All inputs taken, now time to write
            // Go back to the end of the file where the ID3v1 tag resides
            file.seekp(-128, std::ios::end);

            // Write the new tag information to the file
            file.write(reinterpret_cast<const char*>(&newTag), sizeof(newTag));

            std::cout << "Changes made successfully" << std::endl;
            
            //Reset the question and close the file
            question = "";
            file.close();
        }
        else if(question == "quit") {
            break;
        }
        else {
            std::cout << "Unknown input" << std::endl;
        }
    }

    return 0;
}