/*
 * Bafang / APT 8X0C / 560C display firmware
 *
 * Based on OpenSourceEBike firmware with Copyrights (C) to Casainho, 2018, 2019, 2020
 * https://github.com/OpenSourceEBike/Color_LCD
 *
 * Adaptation for MPe (ebikecomputer.com)  Copyright (C) BIKEL.PL Marek Przybylak, Maciej Gibowicz 2020, 2021
 *
 * Released under the GPL License, Version 3
 */

#include "texts.h"
#include "eeprom.h"
#include <string.h>

static uint16_t text_language;
uint16_t text_language_edit;

static const char MPe_texts_mainMenu_PL[Text_mainMenu_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"Bateria i czujnik pradu  /  1-9",
		"Manetka gazu  /  13-18, 130-144",
		"Odczyt predkosci  /  25-28",
		"Czujnik hamulca  /  35",
		"Odczyt temperatury  /  40-44",
		"Reg. PID i tempomat  /  50-65",
		"Konfiguracja PAS  /  70-126",
		"Ustawienia wstepne",
		"Ekran glowny nr 2",
		"Ustawienia wyswietlacza"};
static const char MPe_texts_mainMenu_ENG[Text_mainMenu_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"Battery and current  /  1-9",
		"Throttle  /  13-18, 130-144",
		"Speed reading  /  25-28",
		"Brake sensor  /  35",
		"Temperature reading  /  40-44",
		"PID reg. and cruise c. /  50-65",
		"Pedal assist PAS  /  70-126",
		"Presets",
		"Main screen No. 2",
		"Display configuration"};
char MPe_texts_mainMenu[Text_mainMenu_numberOfTexts][MPE_MAX_TEXT_LENGTH];

static const char MPe_texts_menuBatteryCurrent_PL[Text_menuBatteryCurrent_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"1. Pojemnosc\nbaterii Ah",
		"2. Pojemnosc\nbaterii Wh",
		"3. Napiecie odciecia\nnapedu",
		"4. Napiecie pelnej\nbaterii",
		"5. Czulosc cz. pradu\n mV/A",
		"6. Kierunek\npomiaru",
		"7. Wsp. dzielnika\nnapiecia",
		"8. Pomin. spr.\ncz. pradu",
		"9. Prad zabezp.\nminus"};
static const char MPe_texts_menuBatteryCurrent_ENG[Text_menuBatteryCurrent_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"1. Battery\ncapacity Ah",
		"2. Battery\ncapacity Wh",
		"3. Low voltage\ncutoff",
		"4. Full battery\nvoltage",
		"5. Current resolution\nmV/A",
		"6. Current measure\ndirection",
		"7. Voltage divider\nratio",
		"8. Disable current\nsens. check",
		"9. Negative current\nprotection"};
char MPe_texts_menuBatteryCurrent[Text_menuBatteryCurrent_numberOfTexts][MPE_MAX_TEXT_LENGTH];

static const char MPe_texts_menuAdvConfigGasThrottle_PL[Text_menuAdvConfigGasThrottle_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"13. Wyjscie manetki\nTOT min.",
		"14. Wyjscie manetki\nTOT maks.",
		"15. Wejscie manetki\nTIN min.",
		"16. Wejscie manetki\nTIN maks.",
		"17. Reset manetki\nwl/wyl",
		"18. Bezpieczne\nnapiecie TIN",
		"135. Tryb manetki\nst. 1",
		"136. Tryb manetki\nst. 2",
		"137. Tryb manetki\nst. 3",
		"138. Tryb manetki\nst. 4",
		"139. Tryb manetki\nst. 5",
		"130. Ogr. manetki\nst. 1",
		"131. Ogr. manetki\nst. 2",
		"132. Ogr. manetki\nst. 3",
		"133. Ogr. manetki\nst. 4",
		"134. Ogr. manetki\nst. 5",
		"140. Szybkosc\nmanetki st. 1",
		"141. Szybkosc\nmanetki st. 2",
		"142. Szybkosc\nmanetki st. 3",
		"143. Szybkosc\nmanetki st. 4",
		"144. Szybkosc\nmanetki st. 5",
};
static const char MPe_texts_menuAdvConfigGasThrottle_ENG[Text_menuAdvConfigGasThrottle_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"13. Throttle\nout min.",
		"14. Throttle\nout max.",
		"15. Throttle\nin min. ",
		"16. Throttle\nin max.",
		"17. Throttle\nreset on/off",
		"18. Throttle\nsafe voltage",
		"135. Throttle\nmode 1",
		"136. Throttle\nmode 2",
		"137. Throttle\nmode 3",
		"138. Throttle\nmode 4",
		"139. Throttle\nmode 5",
		"130. Throttle\nlimit 1",
		"131. Throttle\nlimit 2",
		"132. Throttle\nlimit 3",
		"133. Throttle\nlimit 4",
		"134. Throttle\nlimit 5",
		"140. Throttle\nramp up 1",
		"141. Throttle\nramp up 2",
		"142. Throttle\nramp up 3",
		"143. Throttle\nramp up 4",
		"144. Throttle\nramp up 5",
};
char MPe_texts_menuAdvConfigGasThrottle[Text_menuAdvConfigGasThrottle_numberOfTexts][MPE_MAX_TEXT_LENGTH];

static const char MPe_texts_menuSpeedReadingConfig_PL[Text_menuSpeedReadingConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"25. Jednostka\n km/h  /  mph",
		"26. Obwod kola",
		"27. Il. magnesow\ncz. predkosci",
		"28. Przelozenie\nnapedu"

		};
static const char MPe_texts_menuSpeedReadingConfig_ENG[Text_menuSpeedReadingConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"25. Unit  kph / mph",
		"26. Wheel perimeter",
		"27. No. of speed\nsensor magnets",
		"28. Gear ratio"
		};
char MPe_texts_menuSpeedReadingConfig[Text_menuSpeedReadingConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH];

static const char MPe_texts_menuBrakeSensor_PL[Text_menuBrakeSensor_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"35. Kierunek  NO / NC "};
static const char MPe_texts_menuBrakeSensor_ENG[Text_menuBrakeSensor_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"35. Direction  NO / NC "};
char MPe_texts_menuBrakeSensor[Text_menuBrakeSensor_numberOfTexts][MPE_MAX_TEXT_LENGTH];

static const char MPe_texts_menuReadingTempConfig_PL[Text_menuReadingTempConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"40. Jednostka  *C / *F",
		"41. Typ czujnika\ntemp. 1",
		"42. Typ czujnika\ntemp. 2",
		"43. Prog przegrzania\ntemp. 1",
		"44. Prog przegrzania\ntemp. 2"};
static const char MPe_texts_menuReadingTempConfig_ENG[Text_menuReadingTempConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"40. Unit  *C / *F",
		"41. Temp. 1 type",
		"42. Temp. 2 type",
		"43. Temp. 1\noverheat limit",
		"44. Temp. 2\noverheat limit"};
char MPe_texts_menuReadingTempConfig[Text_menuReadingTempConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH];

static const char MPe_texts_menuPIDConfig_PL[Text_menuPIDConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"50. Wspolczynnik P",
		"51. Wspolczynnik I",
		"52. Wspolczynnik D",
		"53. Wspolczynnik\nwolny P",
		"54. Wspolczynnik\nwolny I",
		"55. Wspolczynnik\nwolny D",
		"56. Prog aktywacji\nPID wolny",
		"59. Wsp. predkosci\nPAS min.",
		"60. Wartosc maks.\nPWM dla PID",
		"61. Predkosc narast.\nwsp. pr.",
		"62. Tempomat\nmoc min.",
		"63. Tempomat\nmoc maks.",
		"64. Tempomat predk.\nnar. mocy",
		"65. Tempomat maks.\npredkosc"};
static const char MPe_texts_menuPIDConfig_ENG[Text_menuPIDConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"50. Coefficient P",
		"51. Coefficient I",
		"52. Coefficient D",
		"53. Coefficient\nslow P",
		"54. Coefficient\nslow I",
		"55. Coefficient\nslow D",
		"56. Slow PID\nthreshold",
		"59. Speed factor\nPAS min.",
		"60. PAS PID PWM\nout max.",
		"61. Speed factor\nramp up",
		"62. Cruise ctrl.\npower min.",
		"63. Cruise ctrl.\npower max.",
		"64. Cruise ctrl.\npower ramp up",
		"65. Cruise ctrl.\nmax. speed"};
char MPe_texts_menuPIDConfig[Text_menuPIDConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH];

static const char MPe_texts_menuPasModeConfig_PL[Text_menuPasModeConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"70. Uruchom w tr.\nzabl. wl/wyl",
		"71. Tryb zablokowany\nwl/wyl",
		"72. Ogr. predkosci\ntrybu zabl.",
		"73. Ogr. mocy\ntrybu zabl.",
		"74. Ilosc\nmagnesow PAS",
		"75. Moc wspom.\nst. 1",
		"76. Moc wspom.\nst. 2",
		"77. Moc wspom.\nst. 3",
		"78. Moc wspom.\nst. 4",
		"79. Moc wspom.\nst. 5",
		"80. Ogr. predkosci\nst. 1",
		"81. Ogr. predkosci\nst. 2",
		"82. Ogr. predkosci\nst. 3",
		"83. Ogr. predkosci\nst. 4",
		"84. Ogr. predkosci\nst. 5",
		"85. Min. predkosc\nst. 1",
		"86. Min. predkosc\nst. 2",
		"87. Min. predkosc\nst. 3",
		"88. Min. predkosc\nst. 4",
		"89. Min. predkosc\nst. 5",
		"90. Kadencja min.\nst. 1",
		"91. Kadencja min.\nst. 2",
		"92. Kadencja min.\nst. 3",
		"93. Kadencja min.\nst. 4",
		"94. Kadencja min.\nst. 5",
		"95. Kadencja maks.\nst. 1",
		"96. Kadencja maks.\nst. 2",
		"97. Kadencja maks.\nst. 3",
		"98. Kadencja maks.\nst. 4",
		"99. Kadencja maks.\nst. 5",
		"100. Narastanie mocy\nPAS st. 1",
		"101. Narastanie mocy\nPAS st. 2",
		"102. Narastanie mocy\nPAS st. 3",
		"103. Narastanie mocy\nPAS st. 4",
		"104. Narastanie mocy\nPAS st. 5",
		"105. Wzmocn. PAS\nmoc st. 1",
		"106. Wzmocn. PAS\nmoc st. 2",
		"107. Wzmocn. PAS\nmoc st. 3",
		"108. Wzmocn. PAS\nmoc st. 4",
		"109. Wzmocn. PAS\nmoc st. 5",
		"110. Wzmocn. PAS\nczas st. 1",
		"111. Wzmocn. PAS\nczas st. 2",
		"112. Wzmocn. PAS\nczas st. 3",
		"113. Wzmocn. PAS\nczas st. 4",
		"114. Wzmocn. PAS\nczas st. 5",
		"115. Wzmocn. PAS\npredk. st. 1",
		"116. Wzmocn. PAS\npredk. st. 2",
		"117. Wzmocn. PAS\npredk. st. 3",
		"118. Wzmocn. PAS\npredk. st. 4",
		"119. Wzmocn. PAS\npredk. st. 5",
		"120. Wzmocn. PAS\npr.nar.mocy",
		"121. Czas odswiez.\nkadencji",
		"122. Aktywuj czujnik\nnacisku",
		"123. Masa startowa\nna pedale",
		"124. Cz. nacisku\nADC min.",
		"125. Cz. nacisku\nADC maks.",
		"126. Cz. nacisku\nkgF maks."};
static const char MPe_texts_menuPasModeConfig_ENG[Text_menuPasModeConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"70. Auto limit\nmode on/off",
		"71. Limit mode\non/off",
		"72. Limit mode\nmax. speed",
		"73. Limit mode\nmax. power",
		"74. No. of PAS\nmagnets",
		"75. Power of\nassist 1",
		"76. Power of\nassist 2",
		"77. Power of\nassist 3",
		"78. Power of\nassist 4",
		"79. Power of\nassist 5",
		"80. Speed limit of\nassist 1",
		"81. Speed limit of\nassist 2",
		"82. Speed limit of\nassist 3",
		"83. Speed limit of\nassist 4",
		"84. Speed limit of\nassist 5",
		"85. Min. speed of\nassist 1",
		"86. Min. speed of\nassist 2",
		"87. Min. speed of\nassist 3",
		"88. Min. speed of\nassist 4",
		"89. Min. speed of\nassist 5",
		"90. Min. cadence of\nassist 1",
		"91. Min. cadence of\nassist 2",
		"92. Min. cadence of\nassist 3",
		"93. Min. cadence of\nassist 4",
		"94. Min. cadence of\nassist 5",
		"95. Max. cadence of\nassist 1",
		"96. Max. cadence of\nassist 2",
		"97. Max. cadence of\nassist 3",
		"98. Max. cadence of\nassist 4",
		"99. Max. cadence of\nassist 5",
		"100. PAS power\nramp up 1",
		"101. PAS power\nramp up 2",
		"102. PAS power\nramp up 3",
		"103. PAS power\nramp up 4",
		"104. PAS power\nramp up 5",
		"105. PAS boost\npower 1",
		"106. PAS boost\npower 2",
		"107. PAS boost\npower 3",
		"108. PAS boost\npower 4",
		"109. PAS boost\npower 5",
		"110. PAS boost\ntime 1",
		"111. PAS boost\ntime 2",
		"112. PAS boost\ntime 3",
		"113. PAS boost\ntime 4",
		"114. PAS boost\ntime 5",
		"115. PAS boost max.\nspeed 1",
		"116. PAS boost max.\nspeed 2",
		"117. PAS boost max.\nspeed 3",
		"118. PAS boost max.\nspeed 4",
		"119. PAS boost max.\nspeed 5",
		"120. PAS boost\nramp up",
		"121. Cadence\nrefresh time",
		"122. Enable\ntorque sensor",
		"123. Startup mass\non pedal",
		"124. Torque sensor\nADC min.",
		"125. Torque sensor\nADC max.",
		"126. Torque sensor\nkgF max."};

char MPe_texts_menuPasModeConfig[Text_menuPasModeConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH];

static const char MPe_texts_menuPresetsConfig_PL[Text_menuPresetsConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"Dystans calkowity",
		"Ilosc cykli ladowania"};
static const char MPe_texts_menuPresetsConfig_ENG[Text_menuPresetsConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"Odometer",
		"No. of batt.\ncharge cycles"};
char MPe_texts_menuPresetsConfig[Text_menuPresetsConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH];

static const char MPe_texts_menuSelectedParam_PL[Text_menuSelectedParam_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"Pole 1\n (kol.  1,  wiersz  1)",
		"Pole 2\n (kol. 2,  wiersz  1)",
		"Pole 3\n (kol.  1,  wiersz 2)",
		"Pole 4\n (kol. 2,  wiersz 2)",
		"Pole 5\n (kol.  1,  wiersz 3)",
		"Pole 6\n (kol. 2,  wiersz 3)",
		"Pole 7\n (kol.  1,  wiersz 4)",
		"Pole 8\n (kol. 2,  wiersz 4)"};
static const char MPe_texts_menuSelectedParam_ENG[Text_menuSelectedParam_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"Field 1\n (column  1,  row  1)",
		"Field 2\n (column 2,  row  1)",
		"Field 3\n (column  1,  row 2)",
		"Field 4\n (column 2,  row 2)",
		"Field 5\n (column  1,  row 3)",
		"Field 6\n (column 2,  row 3)",
		"Field 7\n (column  1,  row 4)",
		"Field 8\n (column 2,  row 4)"};
char MPe_texts_menuSelectedParam[Text_menuSelectedParam_numberOfTexts][MPE_MAX_TEXT_LENGTH];

static const char MPe_texts_menuDisplayConfig_PL[Text_menuDisplayConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"Godzina",
		"Minuty",
		"Podswietlenie dzien",
		"Podswietlenie noc",
		"Auto wyl. [min]",
		"Widocznosc zegara",
		"Widocznosc temp. 1",
		"Widocznosc temp. 2",
		"Polski (0) / English (1)",
		"Odwrocenie\nprzyciskow +/-"};
static const char MPe_texts_menuDisplayConfig_ENG[Text_menuDisplayConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"Clock hours",
		"Clock minutes",
		"Brightness day",
		"Brightness night",
		"Auto power off [min]",
		"Clock visibility",
		"Temp. 1  visibility",
		"Temp. 2  visibility",
		"Polski (0) / English (1)",
		"Buttons +/- inverted"};
char MPe_texts_menuDisplayConfig[Text_menuDisplayConfig_numberOfTexts][MPE_MAX_TEXT_LENGTH];

#ifdef USE_WITH_8X0C
static const char MPe_texts_statScreen_PL[Texts_statScreen_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"Czas w ruchu:",

		"Dystans",
		"calkowity:",
		"dzienny:",
		"pozostaly:",

		"Predkosc",
		"srednia:",
		"maks.:",

		"Bateria",
		"napiecie:",
		"prad aktualny:",
		"prad maks.:",
		"moc aktualna:",
		"moc maks.:",

		"Bateria cd",
		"pojemnosc:",
		"zuzyto Ah:",
		"zuzyto Wh:",
		"pozostalo:",
		"ilosc cykli:",
		"zuzycie en:",

		"Temperatura",
		"temp. 1:",
		"temp. 2:",

		"Inne",
		"kadencja:",
		"manetka nap.:",
		"cz. nacisku ADC:",
		"masa na pedale:",
		"wersja MPe:"};
static const char MPe_texts_statScreen_ENG[Texts_statScreen_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"Time in motion:",

		"Distance",
		"total:",
		"daily:",
		"remaining:",

		"Speed",
		"average:",
		"max.:",

		"Battery",
		"voltage:",
		"current:",
		"max. current:",
		"power:",
		"max. power:",

		"Battery cont.",
		"capacity:",
		"consumed Ah:",
		"consumed Wh:",
		"remaining:",
		"No. of cycles:",
		"en. consum.:",

		"Temperature",
		"temp. 1:",
		"temp. 2:",

		"Other",
		"cadence:",
		"throttle in vol.:",
		"torque s. ADC:",
		"mass on pedal:",
		"MPe version:"};
char MPe_texts_statScreen[Texts_statScreen_numberOfTexts][MPE_MAX_TEXT_LENGTH];
#endif

#ifdef USE_WITH_560C
static const char MPe_texts_statScreen_PL[Texts_statScreen_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"W ruchu:",

		"Dystans",
		"calkowity:",
		"dzienny:",
		"pozostaly:",

		"Predkosc",
		"srednia:",
		"maks.:",

		"Bateria",
		"napiecie:",
		"prad:",
		"prad maks.:",
		"moc:",
		"moc maks.:",

		"Bateria cd",
		"pojemnosc:",
		"zuzyto Ah:",
		"zuzyto Wh:",
		"pozostalo:",
		"ilosc cykli:",
		"zuzycie en:",

		"Temperatura",
		"temp. 1:",
		"temp. 2:",

		"Inne",
		"kadencja:",
		"man. nap.:",
		"cz. nac. ADC:",
		"m. na pedale:",
		"wersja MPe:"};
static const char MPe_texts_statScreen_ENG[Texts_statScreen_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"Time in motion:",

		"Distance",
		"total:",
		"daily:",
		"remaining:",

		"Speed",
		"average:",
		"max.:",

		"Battery",
		"voltage:",
		"current:",
		"max. current:",
		"power:",
		"max. power:",

		"Battery cont.",
		"capacity:",
		"consumed Ah:",
		"consumed Wh:",
		"remaining:",
		"No. of cycles:",
		"en. consum.:",

		"Temperature",
		"temp. 1:",
		"temp. 2:",

		"Other",
		"cadence:",
		"throttle in vol.:",
		"torque s. ADC:",
		"mass on pedal:",
		"MPe version:"};
char MPe_texts_statScreen[Texts_statScreen_numberOfTexts][MPE_MAX_TEXT_LENGTH];
#endif
#ifdef USE_WITH_8X0C
static const char MPe_texts_mainScreen2_PL[Texts_mainScreen2_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"PREDKOSC AKTUALNA",
		"DYSTANS POZOSTALY",
		"NALADOWANIE BATERII",
		"DYSTANS DZIENNY",
		"MOC AKTUALNA",
		"TEMP 1",
		"STOPIEN WSP.",
		"DYSTANS CALKOWITY",
		"PREDKOSC SREDNIA",
		"PREDKOSC MAKS.",
		"CZAS W RUCHU",
		"NAPIECIE BATERII",
		"PRAD AKTUALNY",
		"PRAD MAKS.",
		"MOC MAKSYMALNA",
		"ZUZYCIE ENERGII",
		"POJEMNOSC BATERII",
		"ZUZYTO BATERII Ah",
		"TEMP 2",
		"IL. CYKLI LADOWANIA",
		"STATUS HAMULCA",
		"STATUS TEMPOMATU",
		"WERSJA MPe",
		"ST. TRYBU LIMIT",
		"ZUZYTO BATERII Wh",
		"STATUS OSTRZEZENIA",
		"KADENCJA",
		"NAPIECIE MANETKI",
		"CZ. NAC. ADC",
		"MASA NA PEDALE"};

static const char MPe_texts_mainScreen2_ENG[Texts_mainScreen2_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"PRESENT SPEED",
		"DISTANCE TO GO",
		"BATTERY LEVEL",
		"TRIP",
		"PRESENT POWER",
		"TEMP 1",
		"ASSIST LEVEL",
		"ODOMETER",
		"AVERAGE SPEED",
		"MAX. SPEED",
		"TIME IN MOTION",
		"BATTERY VOLTAGE",
		"PRESENT CURRENT",
		"MAX. CURRENT",
		"MAX. POWER",
		"ENERGY CONSUMPTION",
		"BATTERY CAPACITY",
		"BATTERY USED Ah",
		"TEMP 2",
		"CHARGE CYCLES NO.",
		"BRAKE STATUS",
		"C.CONTROL STATUS",
		"MPe VERSION",
		"LIMIT MODE STATUS",
		"BATTERY USED Wh",
		"WARNING STATUS",
		"CADENCE",
		"THROTTLE IN VOLTAGE",
		"TORQUE SENS. ADC",
		"MASS ON PEDAL"};
char MPe_texts_mainScreen2[Texts_mainScreen2_numberOfTexts][MPE_MAX_TEXT_LENGTH];
#endif

#ifdef USE_WITH_560C
static const char MPe_texts_mainScreen2_PL[Texts_mainScreen2_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"PREDKOSC",
		"DYSTANS POZOST.",
		"% NALADOWANIA",
		"DYSTANS DZIENNY",
		"MOC AKTUALNA",
		"TEMP 1",
		"STOPIEN WSP.",
		"DYSTANS CALK.",
		"PREDK. SREDNIA",
		"PREDK. MAKS.",
		"CZAS W RUCHU",
		"NAPIECIE BATERII",
		"PRAD AKTUALNY",
		"PRAD MAKS.",
		"MOC MAKS.",
		"ZUZYCIE EN.",
		"POJ. BATERII",
		"ZUZYTO Ah",
		"TEMP 2",
		"IL. CYKLI LAD.",
		"STATUS HAMULCA",
		"TEMPOMAT",
		"WERSJA MPe",
		"TRYB LIMIT",
		"ZUZYTO Wh",
		"STATUS OSTRZ.",
		"KADENCJA",
		"NAPIECIE MAN.",
		"CZ. NAC. ADC",
		"MASA NA PEDALE"};

static const char MPe_texts_mainScreen2_ENG[Texts_mainScreen2_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"SPEED",
		"DIST. TO GO",
		"BATTERY LEVEL",
		"TRIP",
		"PRESENT POWER",
		"TEMP 1",
		"ASSIST LEVEL",
		"ODOMETER",
		"AV. SPEED",
		"MAX. SPEED",
		"IN MOTION",
		"BATTERY VOL.",
		"CURRENT",
		"MAX. CURRENT",
		"MAX. POWER",
		"EN. CONS.",
		"BATTERY CAP.",
		"USED Ah",
		"TEMP 2",
		"CHARGE NO.",
		"BRAKE STATUS",
		"C.CTRL STATUS",
		"MPe VERSION",
		"LIMIT MODE",
		"BATTERY Wh",
		"WARNING",
		"CADENCE",
		"THROTTLE VOL.",
		"TRQ. S. ADC",
		"MASS ON PEDAL"};
char MPe_texts_mainScreen2[Texts_mainScreen2_numberOfTexts][MPE_MAX_TEXT_LENGTH];
#endif

static const char MPe_texts_configScreen_PL[Text_configScreen_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"Kalibracja czujnika pradu:",
		"przytrzymaj (-) i (o)"};
static const char MPe_texts_configScreen_ENG[Text_configScreen_numberOfTexts][MPE_MAX_TEXT_LENGTH] = {
		"Calibration of current sensor:",
		"hold (-) and (o)"};
char MPe_texts_configScreen[Text_configScreen_numberOfTexts][MPE_MAX_TEXT_LENGTH];

void texts_init(void)
{
	text_language_edit = text_language = (uint16_t)eeprom_readParam(eepromParam_language);
	if (text_language >= languages_count) text_language_edit = text_language = languages_polish;
	texts_setLanguage(text_language);
}
void texts_setLanguage(Text_languages_t language)
{
	if (language == languages_polish)
	{
		for (uint8_t i = 0; i < Text_mainMenu_numberOfTexts; i++) strncpy(MPe_texts_mainMenu[i], MPe_texts_mainMenu_PL[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuBatteryCurrent_numberOfTexts; i++) strncpy(MPe_texts_menuBatteryCurrent[i], MPe_texts_menuBatteryCurrent_PL[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuAdvConfigGasThrottle_numberOfTexts; i++) strncpy(MPe_texts_menuAdvConfigGasThrottle[i], MPe_texts_menuAdvConfigGasThrottle_PL[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuSpeedReadingConfig_numberOfTexts; i++) strncpy(MPe_texts_menuSpeedReadingConfig[i], MPe_texts_menuSpeedReadingConfig_PL[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuBrakeSensor_numberOfTexts; i++) strncpy(MPe_texts_menuBrakeSensor[i], MPe_texts_menuBrakeSensor_PL[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuReadingTempConfig_numberOfTexts; i++) strncpy(MPe_texts_menuReadingTempConfig[i], MPe_texts_menuReadingTempConfig_PL[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuPIDConfig_numberOfTexts; i++) strncpy(MPe_texts_menuPIDConfig[i], MPe_texts_menuPIDConfig_PL[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuPasModeConfig_numberOfTexts; i++) strncpy(MPe_texts_menuPasModeConfig[i], MPe_texts_menuPasModeConfig_PL[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuPresetsConfig_numberOfTexts; i++) strncpy(MPe_texts_menuPresetsConfig[i], MPe_texts_menuPresetsConfig_PL[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuSelectedParam_numberOfTexts; i++) strncpy(MPe_texts_menuSelectedParam[i], MPe_texts_menuSelectedParam_PL[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuDisplayConfig_numberOfTexts; i++) strncpy(MPe_texts_menuDisplayConfig[i], MPe_texts_menuDisplayConfig_PL[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Texts_statScreen_numberOfTexts; i++) strncpy(MPe_texts_statScreen[i], MPe_texts_statScreen_PL[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_configScreen_numberOfTexts; i++) strncpy(MPe_texts_configScreen[i], MPe_texts_configScreen_PL[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Texts_mainScreen2_numberOfTexts; i++) strncpy(MPe_texts_mainScreen2[i], MPe_texts_mainScreen2_PL[i], MPE_MAX_TEXT_LENGTH);
	}
	else if (languages_english)
	{
		for (uint8_t i = 0; i < Text_mainMenu_numberOfTexts; i++) strncpy(MPe_texts_mainMenu[i], MPe_texts_mainMenu_ENG[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuBatteryCurrent_numberOfTexts; i++) strncpy(MPe_texts_menuBatteryCurrent[i], MPe_texts_menuBatteryCurrent_ENG[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuAdvConfigGasThrottle_numberOfTexts; i++) strncpy(MPe_texts_menuAdvConfigGasThrottle[i], MPe_texts_menuAdvConfigGasThrottle_ENG[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuSpeedReadingConfig_numberOfTexts; i++) strncpy(MPe_texts_menuSpeedReadingConfig[i], MPe_texts_menuSpeedReadingConfig_ENG[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuBrakeSensor_numberOfTexts; i++) strncpy(MPe_texts_menuBrakeSensor[i], MPe_texts_menuBrakeSensor_ENG[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuReadingTempConfig_numberOfTexts; i++) strncpy(MPe_texts_menuReadingTempConfig[i], MPe_texts_menuReadingTempConfig_ENG[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuPIDConfig_numberOfTexts; i++) strncpy(MPe_texts_menuPIDConfig[i], MPe_texts_menuPIDConfig_ENG[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuPasModeConfig_numberOfTexts; i++) strncpy(MPe_texts_menuPasModeConfig[i], MPe_texts_menuPasModeConfig_ENG[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuPresetsConfig_numberOfTexts; i++) strncpy(MPe_texts_menuPresetsConfig[i], MPe_texts_menuPresetsConfig_ENG[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuSelectedParam_numberOfTexts; i++) strncpy(MPe_texts_menuSelectedParam[i], MPe_texts_menuSelectedParam_ENG[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_menuDisplayConfig_numberOfTexts; i++) strncpy(MPe_texts_menuDisplayConfig[i], MPe_texts_menuDisplayConfig_ENG[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Texts_statScreen_numberOfTexts; i++) strncpy(MPe_texts_statScreen[i], MPe_texts_statScreen_ENG[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Text_configScreen_numberOfTexts; i++) strncpy(MPe_texts_configScreen[i], MPe_texts_configScreen_ENG[i], MPE_MAX_TEXT_LENGTH);
		for (uint8_t i = 0; i < Texts_mainScreen2_numberOfTexts; i++) strncpy(MPe_texts_mainScreen2[i], MPe_texts_mainScreen2_ENG[i], MPE_MAX_TEXT_LENGTH);
	}
}
void texts_synchro(void)
{
	if (text_language_edit != text_language)
	{
		text_language = text_language_edit;
		texts_setLanguage(text_language);
		eeprom_saveParam(eepromParam_language, text_language);
	}
}
