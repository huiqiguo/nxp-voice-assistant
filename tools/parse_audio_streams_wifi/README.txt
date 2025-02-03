# Description
Saves the audio streams sent by the Smart Voice UI as .wav files. If WiFi audio dump is enabled the board will forward the streams over TCP or UDP.
Requires python 3.7.0 or higher.


# Usage guide:
1. Run from Command Prompt or Linux terminal


# Arguments:
1. Required:
        -m,  --microphones - Set the number of microphones.
        -f,  --folder      - Folder where to save audio files. Note: overwrite existing folder.
        -ip, --ip          - IP address of the SVUI board sending audio data over WiFi
2. Optional:
        -mss, --mic-sample-size - Set microphone one sample's size in bytes. - default=2
        -all, --all             - Capture all audio streams: mics, amplifier, clean stream
        -u,   --udp             - Use UDP protcol, otherwise TCP will be used


# Windows usage example
python parse_audio_streams_wifi.py -m 3 -f testfolder -ip 192.168.1.1
