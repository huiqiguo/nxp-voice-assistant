# Description
Converts all .mp3 files from the current directory and all subdirectories to a specified audio file format.
Requires Linux shell.

# Prerequisites:
1. sudo apt-get install -y libopus-dev
2. sudo apt install ffmpeg

Might also need to run:
dos2unix convert.sh

# Usage guide:
1. Open Linux shell
2. Run: ./convert.sh [type] [format] [encoding freq]

# Arguments
-- type          - wav, opus
-- format        - bin, c_array
-- encoding freq - 16000, 48000

# Usage example
./convert.sh opus bin 16000
