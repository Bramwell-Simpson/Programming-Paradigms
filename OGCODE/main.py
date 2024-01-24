import os
import struct

class ID3v1Tag:
    def __init__(self):
        self.tag = bytearray(3)
        self.title = bytearray(30)
        self.artist = bytearray(30)
        self.album = bytearray(30)
        self.year = bytearray(4)
        self.comment = bytearray(30)
        self.genre = 0

class Utils:
    def __init__(self):
        pass
    
    @staticmethod
    def read_into_buffer(file, buffer):
        data = file.read(len(buffer))
        buffer[:len(data)] = data
        
    @staticmethod
    def get_input(prompt, default):
        user_input = input(prompt).strip()
        return user_input.encode('utf-8') if user_input else default

    @staticmethod
    def get_genre_input(prompt, default):
        user_input = input(prompt).strip()
        return int(user_input) if user_input else default

    @staticmethod
    def process_field(new_value, old_value, length):
        return new_value.rstrip(b'\x00').rstrip(b' ').ljust(length, b'\x00') if new_value.strip() else old_value

class MP3TagEditor:
    def __init__(self):
        pass

    def read_tag(self, filename):
        try:
            file_size = os.path.getsize(filename)
            print("Size of the file in bytes is:", file_size)
        except FileNotFoundError as e:
            print(f"Failure: {e}")
            return

        with open(filename, 'rb') as file:
            file.seek(-128, os.SEEK_END)
            tag = ID3v1Tag()
            
            Utils.read_into_buffer(file, tag.tag)
            Utils.read_into_buffer(file, tag.title)
            Utils.read_into_buffer(file, tag.artist)
            Utils.read_into_buffer(file, tag.album)
            Utils.read_into_buffer(file, tag.year)
            Utils.read_into_buffer(file, tag.comment)
            tag.genre = struct.unpack('B', file.read(1))[0]

            if tag.tag.decode() == "TAG":
                print("Title:", tag.title.decode())
                print("Artist:", tag.artist.decode())
                print("Album:", tag.album.decode())
                print("Year:", tag.year.decode())
                print("Comment:", tag.comment.decode())
                print("Genre:", tag.genre)
            else:
                print("No ID3v1 Tag found!")

    def write_tag(self, filename):
        try:
            file_size = os.path.getsize(filename)
            print("Size of the file in bytes is:", file_size)
        except FileNotFoundError as e:
            print(f"Failure: {e}")
            return

        with open(filename, 'r+b') as file:
            file.seek(-128, os.SEEK_END)
            tag = ID3v1Tag()
            
            Utils.read_into_buffer(file, tag.tag)
            Utils.read_into_buffer(file, tag.title)
            Utils.read_into_buffer(file, tag.artist)
            Utils.read_into_buffer(file, tag.album)
            Utils.read_into_buffer(file, tag.year)
            Utils.read_into_buffer(file, tag.comment)
            tag.genre = struct.unpack('B', file.read(1))[0]

            if tag.tag.decode() == "TAG":
                print("Tag Found, continuing...")
            else:
                print("No ID3v1 Tag found!")
                return

            new_tag = ID3v1Tag()

            new_tag.title = Utils.get_input("Enter the new title (leave blank for no changes):", tag.title)
            new_tag.artist = Utils.get_input("Enter the new Artist:", tag.artist)
            new_tag.album = Utils.get_input("Enter the new album name:", tag.album)
            new_tag.year = Utils.get_input("Enter the new year of release:", tag.year)
            new_tag.comment = Utils.get_input("Enter a new comment:", tag.comment)
            new_tag.genre = Utils.get_genre_input("Enter the genre (refer to ID3v1 Genre list):", tag.genre)
                
            new_tag.title = Utils.process_field(new_tag.title, tag.title, 30)
            new_tag.artist = Utils.process_field(new_tag.artist, tag.artist, 30)
            new_tag.album = Utils.process_field(new_tag.album, tag.album, 30)
            new_tag.year = Utils.process_field(new_tag.year, tag.year, 4)
            new_tag.comment = Utils.process_field(new_tag.comment, tag.comment, 30)

            new_tag.genre = new_tag.genre if new_tag.genre else tag.genre
            
            new_tag.tag = "TAG".encode()

            # Go back to the end of the file where the ID3v1 tag resides
            file.seek(-128, os.SEEK_END)
            # Write the new tag information to the file
            file.write(bytes(new_tag.tag))
            file.write(bytes(new_tag.title))
            file.write(bytes(new_tag.artist))
            file.write(bytes(new_tag.album))
            file.write(bytes(new_tag.year))
            file.write(bytes(new_tag.comment))
            file.write(struct.pack('B', new_tag.genre))

            print("Changes made successfully")

def main():
    tag_editor = MP3TagEditor()

    while True:
        question = input("What would you like to do (read/write/quit) > ").lower()

        if question == "read":
            filename = input("Enter file name: ")
            tag_editor.read_tag("../resources/" + filename)
        elif question == "write":
            filename = input("Enter file name: ")
            tag_editor.write_tag("../resources/" + filename)
        elif question == "quit":
            break
        else:
            print("Unknown input")


if __name__ == "__main__":
    main()
