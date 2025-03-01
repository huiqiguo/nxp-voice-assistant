/*
 * Copyright 2023-2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef VIT_FR_FR_STRINGS_TO_PROMPTS_VIT_H_
#define VIT_FR_FR_STRINGS_TO_PROMPTS_VIT_H_

#if ENABLE_VIT_ASR

#include "sln_flash_files.h"

const void * const prompts_ww_fr[] = {
    AUDIO_WW_DETECTED, // "Salut NXP"
};

const void * const prompts_elevator_fr[] = {
    AUDIO_FIRST_FLOOR_FR,    // "Premier étage"
    AUDIO_SECOND_FLOOR_FR,   // "Deuxième étage"
    AUDIO_THIRD_FLOOR_FR,    // "Troisième étage"
    AUDIO_FOURTH_FLOOR_FR,   // "Quatrième étage"
    AUDIO_FIFTH_FLOOR_FR,    // "Cinquième étage"
    AUDIO_MAIN_LOBBY_FR,     // "Entrée principale"
    AUDIO_GROUND_FLOOR_FR,   // "Rez-de-chaussée"
    AUDIO_BASEMENT_FLOOR_FR, // "Sous-sol"
    AUDIO_OPEN_DOOR_FR,      // "Ouvrir porte"
    AUDIO_CLOSE_DOOR_FR,     // "Fermer porte"
    AUDIO_DEMO_NAME_TEST_FR, // "Changer de démo"
    AUDIO_LANGUAGE_TEST_FR,  // "Change language"
    AUDIO_OK_FR,             // "Ascenseur"
    AUDIO_OK_FR,             // "Lave-Linge"
    AUDIO_OK_FR,             // "Maison Connectée"
    AUDIO_OK_FR,             // "Anglais"
    AUDIO_OK_FR,             // "Francais"
    AUDIO_OK_FR,             // "Allemand"
    AUDIO_OK_FR,             // "Chinois"
};

const void * const prompts_washing_machine_fr[] = {
    AUDIO_DELICATE_FR,       // "Lavage délicat"
    AUDIO_NORMAL_FR,         // "Lavage normal"
    AUDIO_HEAVY_DUTY_FR,     // "Lavage en profondeur"
    AUDIO_WHITES_FR,         // "Lavage blanc"
    AUDIO_START_FR,          // "Commencer"
    AUDIO_CANCEL_FR,         // "Annuler"
    AUDIO_DEMO_NAME_TEST_FR, // "Changer de démo"
    AUDIO_LANGUAGE_TEST_FR,  // "Change language"
    AUDIO_OK_FR,             // "Ascenseur"
    AUDIO_OK_FR,             // "Lave-Linge"
    AUDIO_OK_FR,             // "Maison Connectée"
    AUDIO_OK_FR,             // "Anglais"
    AUDIO_OK_FR,             // "Francais"
    AUDIO_OK_FR,             // "Allemand"
    AUDIO_OK_FR,             // "Chinois"
};

const void * const prompts_smart_home_fr[] = {
    AUDIO_TURN_ON_THE_LIGHTS_FR,  // "Allumer lumière"
    AUDIO_TURN_OFF_THE_LIGHTS_FR, // "Éteindre lumière"
    AUDIO_TEMPERATURE_HIGHER_FR,  // "Augmenter température"
    AUDIO_TEMPERATURE_LOWER_FR,   // "Diminuer température"
    AUDIO_OPEN_THE_WINDOW_FR,     // "Ouvrir fenêtre"
    AUDIO_CLOSE_THE_WINDOW_FR,    // "Fermer fenêtre"
    AUDIO_MAKE_IT_BRIGHTER_FR,    // "Augmenter luminosité"
    AUDIO_MAKE_IT_DARKER_FR,      // "Diminuer luminosité"
    AUDIO_DEMO_NAME_TEST_FR,      // "Changer de démo"
    AUDIO_LANGUAGE_TEST_FR,       // "Change language"
    AUDIO_OK_FR,                  // "Ascenseur"
    AUDIO_OK_FR,                  // "Lave-Linge"
    AUDIO_OK_FR,                  // "Maison Connectée"
    AUDIO_OK_FR,                  // "Anglais"
    AUDIO_OK_FR,                  // "Francais"
    AUDIO_OK_FR,                  // "Allemand"
    AUDIO_OK_FR,                  // "Chinois"
};

#endif /* ENABLE_VIT_ASR */
#endif /* VIT_FR_FR_STRINGS_TO_PROMPTS_VIT_H_ */
