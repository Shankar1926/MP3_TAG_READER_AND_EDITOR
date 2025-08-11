🎵 MP3 Tag Reader & Editor in C
A terminal-based MP3 Tag Reader & Editor application written in C that reads and edits ID3v2 metadata tags from MP3 files.
Developed as part of the Certified Embedded Professional (ECEP) course at Emertxe Information Technologies, Bangalore.

📌 Project Highlights
🔹 View MP3 metadata such as Title, Artist, Album, Year, Genre, Composer, Lyricist, and Comments

🔹 Edit specific metadata tags with new values

🔹 Support for ID3v2.3 tags (partial support for ID3v2.2, excludes ID3v2.4)

🔹 Command-line interface for quick and flexible operations

🔹 Modular code with separate .c and .h files

🔹 Error handling for missing or invalid MP3 files

🧠 What I Learned
📍 MP3 file structure and ID3v2 metadata format

📍 File I/O in binary mode (fopen, fread, fwrite)

📍 Command-line argument parsing for multi-functional tools

📍 Modular programming with reusable functions

📍 Working with character arrays and string manipulation in C

📍 Writing maintainable CLI tools in C

⚙️ Tech Stack
Component	Details
Language	C (GCC)
Data Format	MP3 (ID3v2 tags)
Platform	Linux terminal
Tools	VS Code, GCC Compiler, Git

📂 Project Structure
bash
Copy
Edit
├── main.c          # Entry point of the program
├── view_tag.c      # Logic for reading/viewing MP3 tags
├── edit_tag.c      # Logic for editing MP3 tags
├── view_tag.h      # Header for viewing functions
├── edit_tag.h      # Header for editing functions
├── types.h         # Common type definitions
├── Makefile        # Build automation
└── README.md       # Project documentation
🚀 How to Run
1️⃣ Compile the Project
bash
Copy
Edit
make
2️⃣ View MP3 Metadata
bash
Copy
Edit
./mp3_reader -v <filename.mp3>
Example:

bash
Copy
Edit
./mp3_reader -v song.mp3
3️⃣ Edit a Specific Tag
bash
Copy
Edit
./mp3_reader -e <tag_option> <new_value> <filename.mp3>
Example:

bash
Copy
Edit
./mp3_reader -e -t "New Song Title" song.mp3
Tag Options:

Option	Description
-t	Title
-a	Artist
-A	Album
-y	Year
-g	Genre
-c	Comment
-l	Lyricist
-m	Composer

4️⃣ Show Help
bash
Copy
Edit
./mp3_reader -h
📝 Example Output
Viewing:

yaml
Copy
Edit
----- MP3 TAGS -----
Title     : Shape of You
Artist    : Ed Sheeran
Album     : ÷ (Divide)
Year      : 2017
Genre     : Pop
Composer  : Ed Sheeran
Lyricist  : Ed Sheeran
Comment   : No comments
Editing:

vbnet
Copy
Edit
INFO: Editing Tag...
INFO: Writing new value to frame TIT2 (Title)
INFO: Tag updated successfully!
🔍 How It Works – ID3v2 Tag Reading
Reads MP3 file header to locate the ID3v2 tag section

Searches for specific Frame IDs like TIT2 (Title), TPE1 (Artist), TALB (Album)

Displays the stored metadata values

For editing, creates a modified copy of the MP3 file with updated metadata
