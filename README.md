# **NXP Voice Assistant**

## **Project Summary**
This project utilises the NXP i.MX RT106V board to develop a voice assistant tool. The pre-configured version of the project is capable of playing music on demand and comes with relevant voice-controlled features such as volume adjustment. For details, refer to [Quick Start](#quick-start).

## **Prerequisites**
### 1. Hardware
- NXP i.MX RT106V development board.
- USB Type-C cable.
- J-link for flashing or debugging (should be included in the dev kit box).

### 2. Download Python
- Download from https://www.python.org/downloads/
- Works with Python versions 3.6 to 3.10.

### 3. Set Up Virtual Environment and Install Ivaldi Package
- Open Command Prompt and navigate to the tools\Ivaldi_updater directory.
- Install virtualenv: `c:\tools\Ivaldi_updater> pip install virtualenv`
- Create virtual env: `c:\tools\Ivaldi_updater> virtualenv env`
- Activate virtual env: `c:\tools\Ivaldi_updater> env\Scripts\activate`
- Install requirements: `(env) c:\tools\Ivaldi_updater> pip install -r requirements.txt`
- Install Ivaldi: `(env) c:\tools\Ivaldi_updater> pip install .`

### 4. Set Up MCUXpresso
*Required for project customisation, e.g. changing MCU parameters or adding personalised prompts*

### 4.1 Download MCUXpresso IDE
- Go to https://www.nxp.com/mcuxpresso/ide
- Click on the "Downloads" orange button.
- Go to the "MCUXpresso Integrated Development Environment (IDE)" section and click "Download".
- Under the "Previous" tab, select version 11.10.0.
- Agree with the "Software Terms and Conditions", and click on the download link for your system.

### 4.2 Download MCUXpresso SDK
- Go to https://mcuxpresso.nxp.com/
- Click on "Select Development Board".
- Search for "RT1060 EVKC".
- Select "MIMXRT1060-EVKC".
- Ensure that the SDK version is 2.16.0 and click on "Build SDK".
- Select "MCUXpresso IDE" as the toolchain and click on "Build SDK".
- Once available, click on "Download".

### 4.3. Install SDK in the IDE
- Open MCUXpresso IDE.
- By default, the "Welcome" page will be displayed. You may close it.
- In the "Installed SDKs" section, drag and drop the previously downloaded SDK zip folder.
- Click on "OK" when asked to confirm the SDK installation.

## **Quick Start**
### 1. Using the Pre-Configured Demo
- The pre-configured demo is available in the tools\Ivaldi_updater folder.
- With the board powered off, put it in serial downloader mode by moving jumper J61 to connect pins 2 and 3 (towards the buttons).
> [!NOTE]
> Do not move the jumper when the board is powered on.
- Connect the board to your computer via the USB Type-C cable.
- Start the FLASH_SVUI_BOARD.bat script to update the board.
- When the update is done, disconnect the board, move the jumper to the initial position connecting pins 1 and 2 (towards the speaker) and reboot the board.

### 2. Commands
This section summarises the commands that the pre-configured board can recognise and respond to. 
- Wake Word: Hey NXP
- Music Playback: Start/play/stop/pause music, next/previous song
- Volume Control: Increase/decrease/min/max volume, mute

For a full list of expressions, refer to *VA\local_voice\S2I\en\VIT_Model_en_Audioplayer_expression.txt*.

### 3. Barge-In Feature
The pre-configured board comes with a barge-in feature, i.e. commands can be detected when audio is playing. If the wake word is detected when audio is playing, the audio will be temporarily paused and the LED on the board will light up in blue, indicating that the board is listening for the next command.

### 4. Set Up Shell Interface via Tera Term
> [!NOTE]
> This has only been tested with Windows OS. Tera Term is potentially incompatible with MacOS/Linux.
> For MacOS/Linux users, consider using PuTTY instead.
- Download Tera Term from https://github.com/TeraTermProject/teraterm/releases
- Ensure that Jumper J61 is connecting pins 1 and 2 (towards the speaker) and connect the board to your computer via the USB Type-C cable.
- Open Tera Term and select "Serial".
- Choose the port corresponding to the one that the board is connected to.
- When the terminal is open, you may call `version` to check the device version, `commands` for a list of commands specific to the project, and `help` for a list of commands regarding board configuration.

## **Further Development**
### 1. NXP Tutorials
The NXP Application Code Hub (https://github.com/nxp-appcodehub/rd-mcu-svui) comes with an "examples" folder that includes a set of tutorials for customising the board. 

To start from scratch and follow the tutorials, import the "sln_svui_iot_local_demo" folder in MCUXpresso IDE.

To build on the pre-configured project, import the "VA" folder instead. 

Adding a new EN S2I model: https://github.com/nxp-appcodehub/rd-mcu-svui/tree/main/examples/S2I/example_0
Adding a new EN VIT model: https://github.com/nxp-appcodehub/rd-mcu-svui/tree/main/examples/VIT/example_1
Adding new audio prompts: https://github.com/nxp-appcodehub/rd-mcu-svui/tree/main/examples/VIT/example_4

### 2. Modifying the Welcome Announcement
- Follow this [example](https://github.com/nxp-appcodehub/rd-mcu-svui/tree/main/examples/VIT/example_4) to generate the opus file for your customised welcome announcement and add the new header line into your project under *source/sln_flash_files.h*.
- If your project is built on the "sln_svui_iot_local_demo" folder, edit *local_voice/VIT/en.en_voice_demos_vit.h*. Add `AUDIO_WELCOME` (or whatever you defined the name as) instead of `NULL` to play the new welcome announcement prompt.
``` 
const sln_voice_demo_t test_demo_en =
{
    ww_en,                          // wake word strings
    cmd_test_demo_en,               // command strings
    actions_ww_en,                  // wake word actions
    actions_test_demo_en,           // command actions
    prompts_ww_en,                  // wake word prompts
    prompts_test_demo_en,           // command prompts
    AUDIO_WELCOME,                  // prompt for demo announcement
    NUM_ELEMENTS(ww_en),            // number of wake words
    NUM_ELEMENTS(cmd_test_demo_en), // number of commands
    (void *)VIT_Model_en,           // pointer to model
    ASR_ENGLISH,                    // what language is used
    ASR_CMD_TEST_DEMO,              // what demo is used
    LANG_STR_EN,                    // language string
    DEMO_STR_TEST_DEMO,             // demo string
};
```
- If your project is built on the "VA" folder, edit *source/main.c*. Add `AUDIO_WELCOME` (or whatever you defined the name as) instead of `NULL` to play the new welcome announcement prompt.
```
static void announce_demo_s2i(uint16_t demo, uint32_t volume)
{
    char *prompt = NULL;
    switch (demo)
    {
        case ASR_VA:
            prompt = AUDIO_WELCOME;
            break;
    }
}
```

### 3. Adding Your Own Music
- Ensure that your projct is built on the "VA" folder.
- Follow this [example](https://github.com/nxp-appcodehub/rd-mcu-svui/tree/main/examples/VIT/example_4) to generate the opus files for your chosen songs and add the new header lines into your project under *source/sln_flash_files.h*.
- Go to *source/app_layer_nxp_s2i.c*. Edit `NUM_SONGS` and `s_songsList` according to the audio files you have.
```
#define NUM_SONGS 4

static char* s_songsList[NUM_SONGS] = {AUDIO_BIRDS_OF_A_FEATHER, AUDIO_BOHEMIAN_RHAPSODY, AUDIO_CANON_IN_D, AUDIO_TITANIUM};
```

### 4. Updating the Board
- In *source/app.h*, it is good practice to update the application version (`APP_BLD_VER`) because it will be an easy way to check that the board was indeed updated, by calling the command `version` in the shell. 
- Build the project in MCUXpresso, then generate a .bin file from the .axf file.
- Rename the .bin file to have the prefix "sln_svui_iot_", e.g. sln_svui_iot_newproject.
- Move the .bin file into the *tools/Ivaldi_updater/Image_Binaries* folder.
- Update *tools/Ivaldi_updater/Scripts/sln_platforms_config/sln_svui_iot_config/board_config.py* to reflect the name of your .bin file.
```
MAIN_APP_NAME = 'newproject'
```
- Ensure that the board is in serial downloader mode and run the FLASH_SVUI_BOARD.bat script to update the board.