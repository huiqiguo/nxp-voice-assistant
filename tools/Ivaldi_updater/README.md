# Preconfigured Ivaldi

NXP provides a package of scripts that can be used for manufacturing programming and reprogramming
devices on the production line without the J-Link. This collection of scripts is called Ivaldi. The Ivaldi package
allows developers to program all the required firmware binaries into a flash device using a single command.

The tool is preconfigured by default and contains Python3.9, latest image binaries, audio files and an already configured environment.
Its main purpose is to flash the board with the latest NXP software by running **FLASH_SVUI_BOARD.bat**.

Optionally, the preconfigured env can be activated in command line and used to run Ivaldi scripts directly and not through the .bat file.
In order to do that you need to update the paths in **env/pyvenv.cfg**. There are two possible options:

- edit paths manually
- run **FLASH_SVUI_BOARD.bat**

# Setup a new environment

First, delete the existent configuration by removing the following folders:

- env
- Ivaldi.egg-info
- Python3.9

To setup a new enviorment, follow the steps below.

# Ivaldi Getting Started

IoT & Security Solutions: i.MX RT Programming and Test Scripts

Modules contained in Ivaldi (Sons of Ivaldi):

- blhost
- cst
- elftosb
- helpers
- onboard
    - aws
- sdphost

## Supported Platforms

- Windows Power Shell and Command Prompt
    - Python3 (Tested with Python 3.10.11)
- Linux
    - Bash (NOTE: Requires login as root user)
    - Python3 (Tested with Python 3.10.6)
- Windows Subsystem for Linux
    - Bash
    - Python3 (Tested with Python 3.8.3)

**KNONW ISSUE**
If working **WSL** environment, there is a **restriction on the path** on which to clone this repository, ie. you don't want to clone under 'user' folder of linux ~/ .
**Prefer cloning under /mnt/c and keep a short path**.

## Dependencies

### Hardware

RT Device must be in serial download mode.

### Software

- Python 3.10.x
    - [NOTE: Might work with other Python 3.x versions, but untested. See above.]
    - Python versions below 3.6 and above 3.10 won't work as they are not compatible with littlefs-python 0.3.0 which we are using
- Pip for Python3

### Certificates

- Image verification / OTA:
    - Follow the Scripts/ota_signing/README.md

## Setup

1. **Installing virtualenv**

```
user@host:~/sln_svui_iot/tools/Ivaldi_updater $ pip install virtualenv
```

2. **Starting virtualenv** (inside the Ivaldi_updater folder)

```
user@host:~/sln_svui_iot/tools/Ivaldi_updater $ virtualenv env
```

**On Linux host only** login as root user (then you can continue with the next steps). NOTE: This step does not apply for Windows Subsystem for Linux.

```
user@host:~/sln_svui_iot/tools/Ivaldi_updater $ sudo su
```

3. **Activating virtualenv**

- Activating virtualenv in **Bash**

```
user@host:~/sln_svui_iot/tools/Ivaldi_updater $ source env/bin/activate
```

- Activating virtualenv in **PowerShell**

```
PS C:\sln_svui_iot\tools\Ivaldi_updater> Set-ExecutionPolicy RemoteSigned
PS C:\sln_svui_iot\tools\Ivaldi_updater> cd env/Scripts
PS C:\sln_svui_iot\tools\Ivaldi_updater\env\Scripts> ./activate.ps1
(env) PS C:\sln_svui_iot\tools\Ivaldi_updater> cd ../..
```

- Activating virtualenv in **CMD Prompt**

```
C:\sln_svui_iot\tools\Ivaldi_updater> env\Scripts\activate
```

4. **Install requirements**

```
(env) user@host:~/sln_svui_iot/tools/Ivaldi_updater $ pip install -r requirements.txt
```

5. **Installing Ivaldi package**

- Installing Ivaldi package in **Bash**

```
(env) user@host:~/sln_svui_iot/tools/Ivaldi_updater $ python setup.py install
```

- Installing Ivaldi package in **CMD Prompt**

```
(env) user@host:~/sln_svui_iot/tools/Ivaldi_updater $ pip install .
```

6. **Build Ivaldi API Docs** (This step is optional)

```
(env) user@host:~/sln_svui_iot/tools/Ivaldi_updater $ cd Docs/Ivaldi
(env) user@host:~/sln_svui_iot/tools/Ivaldi_updater $ make html
(env) user@host:~/sln_svui_iot/tools/Ivaldi_updater $ # Output in Docs/Ivaldi/_build/html
(env) user@host:~/sln_svui_iot/tools/Ivaldi_updater $ cd ../..  # Get back to parent dir
```

## Executing

- For **Open Boot** check:
    - Scripts/[PLATFORM]_open_boot/README.md
- For **Secure Boot** check:
    - Scripts/[PLATFORM]_secure_boot/README.md

