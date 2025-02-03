#! /usr/bin/env python3
'''
Copyright 2024 NXP.
NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly
in accordance with the applicable license terms. By expressly accepting such terms or by downloading,
installing, activating and/or otherwise using the software, you are agreeing that you have read, and
that you agree to comply with and are bound by, such license terms. If you do not agree to be bound by
the applicable license terms, then you may not retain, install, activate or otherwise use the software.
'''

import argparse
import os
import sys
import shutil
import wave
from scipy.io import wavfile
import threading
import socket

# Configuration variables. Adjust to your needs
CONVERT_RAW_TO_WAV = True
KEEP_RAW_FILES = False

# Global variables
STOP_PROCESS = False
UDP_PORT = 10001       # Default UDP port

def signal_handler(sig, frame):
    global STOP_PROCESS

    print('--> CTRL + C pressed')
    STOP_PROCESS = True

def wait_cancel_from_user():
    global STOP_PROCESS

    print("Type 'q' and press Enter to cancel the process >>\r\n")
    try:
        for line in sys.stdin:
            cmd = line.rstrip()
            if cmd == 'q':
                print("--> User pressed 'q'")
                break
    except:
        print("--> [WARNING] wait_cancel_from_user exception")

    STOP_PROCESS = True
    print("--> wait_cancel_from_user task done")

def capture_g_afe(mic_sample_size, sock, capture_all=False):
    global STOP_PROCESS

    while(1):
        if STOP_PROCESS:
            print("--> Recording Stopped")
            message = "c"
            try:
                sock.send(message.encode())
            finally:
                sock.close()
            break

        try:
            if STOP_PROCESS == False:
                if capture_all:
                    if sock.type == socket.SOCK_STREAM:
                        data = sock.recv(160 * mic_sample_size * (int(MICS_CNT) + 2), socket.MSG_WAITALL)
                    elif sock.type == socket.SOCK_DGRAM:
                        data = sock.recv(160 * mic_sample_size * (int(MICS_CNT) + 2))

                    if len(data) != (160 * mic_sample_size * (int(MICS_CNT) + 2)):
                        continue
                    else:
                        bytes_written = 0

                    mic1_file.write(data[bytes_written : bytes_written + 320])
                    bytes_written = bytes_written + 320

                    mic2_file.write(data[bytes_written : bytes_written + 320])
                    bytes_written = bytes_written + 320

                    if MICS_CNT == '3':
                        mic3_file.write(data[bytes_written : bytes_written + 320])
                        bytes_written = bytes_written + 320

                    amp_file.write(data[bytes_written : bytes_written + 320])
                    bytes_written = bytes_written + 320

                    clean_audio_file.write(data[bytes_written : bytes_written + 320])
                    bytes_written = bytes_written + 320
                else:
                    if sock.type == socket.SOCK_STREAM:
                        data = sock.recv(160 * 2, socket.MSG_WAITALL)
                    elif sock.type == socket.SOCK_DGRAM:
                        data = sock.recv(160 * 2)

                    if len(data) != (160 * 2):
                        continue
                    else:
                        bytes_written = 0

                    clean_audio_file.write(data)

            if STOP_PROCESS == False:
                if not data:
                    print("--> [WARNING] data empty")
                    continue

        except Exception as e:
            print("--> Recording Error")
            STOP_PROCESS = True

def convert_raw_to_wav(file_path, sample_size=2):
    new_file_path = file_path[:-3] + "wav"

    with open(file_path, "rb") as inp_f:
        data = inp_f.read()
        with wave.open(new_file_path, "wb") as out_f:
            out_f.setnchannels(1)
            out_f.setsampwidth(sample_size) # number of bytes
            out_f.setframerate(16000)
            out_f.writeframesraw(data)

    if not KEEP_RAW_FILES:
        os.remove(file_path)

def convert_raw_to_wav2(file_path, sample_size=2):
    new_file_path = file_path[:-3] + "wav"

    with open(file_path, "rb") as inp_f:
        data = inp_f.read()
        with wave.open(new_file_path, "wb") as out_f:
            out_f.setnchannels(1)
            out_f.setsampwidth(sample_size) # number of bytes
            out_f.setframerate(48000)
            out_f.writeframesraw(data)

    if not KEEP_RAW_FILES:
        os.remove(file_path)


""" Parse the provided parameters """
parser = argparse.ArgumentParser()
parser.add_argument('-m', '--microphones', type=int, required=True, choices=[2, 3], help="Set the number of microphones.")
parser.add_argument('-mss', '--mic-sample-size', type=int, default=2, choices=[2, 4], help="Set microphone one sample's size in bytes.")
parser.add_argument('-f', '--folder', type=str, required=True, help="Folder where to save audio files. Note: overwrite existing folder.")
parser.add_argument('-ip', '--ip', type=str, required=True, help="Board ip")
parser.add_argument('-u', '--udp', action='store_true', help="Use UDP protcol, otherwise TCP will be used")
parser.add_argument('-all', '--all', action='store_true', help="Use UDP protcol, otherwise TCP will be used")
args = parser.parse_args()

# Get the Number of microphones
MICS_CNT = str(args.microphones)

# Get the Test name
test_name = args.folder

# Delete the Test folder with the same name (if exists)
try:
    shutil.rmtree(test_name)
except:
    pass

try:
    os.mkdir(test_name)
except OSError:
    print("--> ERROR: Creation of the directory %s failed" % test_name)
    sys.exit(1)

MIC1_STREAM_PATH                = test_name + "/" + test_name + "_mic1.raw"
MIC2_STREAM_PATH                = test_name + "/" + test_name + "_mic2.raw"
MIC3_STREAM_PATH                = test_name + "/" + test_name + "_mic3.raw"
AMP_STREAM_PATH                 = test_name + "/" + test_name + "_amp.raw"
CLEAN_STREAM_PATH               = test_name + "/" + test_name + "_clean_processed_audio.raw"

if args.all:
    mic1_file        = open(MIC1_STREAM_PATH,  "wb")
    mic2_file        = open(MIC2_STREAM_PATH,  "wb")
    if MICS_CNT == '3':
        mic3_file = open(MIC3_STREAM_PATH, "wb")
    amp_file         = open(AMP_STREAM_PATH,   "w+b")
    clean_audio_file = open(CLEAN_STREAM_PATH, "wb")
else:
    clean_audio_file = open(CLEAN_STREAM_PATH, "wb")

# Send start message
message = "s"
if args.udp:
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
else:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((args.ip, UDP_PORT))
try:
    sock.send(message.encode())
except:
    print('Could not send start command')

# Start recording. User has to type "stop" to cancel the process
stop_thread = threading.Thread(target=wait_cancel_from_user, daemon=True)
stop_thread.start()

capture_g_afe(args.mic_sample_size, sock, args.all)
stop_thread.join(timeout=0.1)

# Close the files
if args.all:
    mic1_file.close()
    mic2_file.close()
    if MICS_CNT == '3':
        mic3_file.close()
    amp_file.close()
    clean_audio_file.close()
else:
    clean_audio_file.close()

# Convert RAW files to WAV files
if CONVERT_RAW_TO_WAV:
    print("--> Converting RAW to WAV")
    if args.all:
        convert_raw_to_wav(MIC1_STREAM_PATH, args.mic_sample_size)
        convert_raw_to_wav(MIC2_STREAM_PATH, args.mic_sample_size)
        if MICS_CNT == '3':
            convert_raw_to_wav(MIC3_STREAM_PATH, args.mic_sample_size)
        convert_raw_to_wav(AMP_STREAM_PATH)
        convert_raw_to_wav(CLEAN_STREAM_PATH)
    else:
        convert_raw_to_wav(CLEAN_STREAM_PATH)