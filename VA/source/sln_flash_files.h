/*
 * Copyright 2022-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SLN_FLASH_FILES_H_
#define _SLN_FLASH_FILES_H_

/* Used to include app specific files */

/* Authentication certificates.*/
#define ROOT_CA_CERT       "ca_root.dat"
#define APP_A_SIGNING_CERT "app_a_sign_cert.dat"
#define APP_B_SIGNING_CERT "app_b_sign_cert.dat"
#define CRED_SIGNING_CERT  "cred_sign_cert.dat"

/* Certificate to connect to the amazon mqtt broker */
#define clientcredentialCLIENT_CERTIFICATE_PEM "cert.dat"

/* Private key used for encryption for TLS */
#define clientcredentialCLIENT_PRIVATE_KEY_PEM "pkey.dat"

/* Audio files */
#define AUDIO_WW_DETECTED                                           "Additional_Prompts/Common/ww_detected.opus"
#define AUDIO_TONE_TIMEOUT                                          "Additional_Prompts/Common/tone_timeout.opus"

//#if ENABLE_S2I_ASR

//#endif /* ENABLE_S2I_ASR */

#define AUDIO_MUSIC_PLAYBACK_DEMO_EN                                "Additional_Prompts/EN/music_playback_demo_en.opus"
#define AUDIO_ROMANTIC_PIANO                                        "Songs/Common/01_romantic_piano.opus"
#define AUDIO_ROCK                                                  "Songs/Common/02_rock.opus"
#define AUDIO_MOONLIT_BLUES                                         "Songs/Common/03_moonlit_blues.opus"
#define AUDIO_RISE_UP_HIGH                                          "Songs/Common/04_rise_up_high.opus"
#define AUDIO_SLOW_BLUES                                            "Songs/Common/05_slow_blues.opus"

#define AUDIO_BIRDS_OF_A_FEATHER                                    "VA/EN/birds_of_a_feather.opus"
#define AUDIO_BOHEMIAN_RHAPSODY                                     "VA/EN/bohemian_rhapsody.opus"
#define AUDIO_CANON_IN_D                                            "VA/EN/canon_in_d.opus"
#define AUDIO_TITANIUM                                              "VA/EN/titanium.opus"
#define AUDIO_WELCOME                                               "VA/EN/welcome.opus"

#endif /* _SLN_FLASH_FILES_H_ */
