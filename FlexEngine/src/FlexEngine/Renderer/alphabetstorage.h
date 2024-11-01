/*!*************************************************************************
 * WLVERSE [https://wlverse.web.app] 
 * @file AlphabetStorage.h
 * @brief Header file for storing characters from various language alphabets.
 *
 * This file defines a constant array containing characters from multiple alphabets,
 * including Cyrillic, Latin, numbers, and common symbols. It enables easy access to
 * a diverse set of characters, supporting multilingual text processing within an application.
 *
 * ## Purpose
 * This file provides a unified character array for applications that need multilingual support
 * or symbol handling. The array allows for encoding, searching, and processing tasks that involve
 * a range of character sets.
 *
 * ## Usage
 * - Include this header file in modules where multilingual character support is needed.
 * - Access `ALPHABET` for character referencing, validation, or other processing tasks.
 *
 * @note This file does not handle Unicode or extended scripts and focuses on a basic set
 *       of characters for simplicity and broad accessibility.
 *
 * ## Authors
 * [100%] Soh Wei Jie (weijie.soh@digipen.edu)
 *   - Main Author
 *   - Defined core character sets and managed integration with multilingual support needs.
 *
 * Copyright (c) 2024 DigiPen, All rights reserved.
 ***************************************************************************/

//#ifndef ALPHABETSTORAGE_H
//#define ALPHABETSTORAGE_H
//
//#include <Windows.h>
// // @brief Constant array to store characters from various alphabets and symbols.
////unsigned int const ALPHABET[] = {
////    // Cyrillic lowercase characters
////    L'й', L'э', L'ц', L'у', L'к', L'е', L'н', L'г', L'ш', L'щ',
////    L'з', L'х', L'ъ', L'ф', L'ы', L'в', L'а', L'п', L'р', L'о',
////    L'л', L'д', L'ж', L'э', L'я', L'ч', L'с', L'м', L'и', L'т',
////    L'ь', L'б', L'ю',
////
////    // Cyrillic uppercase characters
////    L'Й', L'Ц', L'У', L'К', L'Е', L'Н', L'Г', L'Ш', L'Щ', L'З',
////    L'Х', L'Ъ', L'Ф', L'Ы', L'В', L'А', L'П', L'Р', L'О', L'Л',
////    L'Д', L'Ж', L'Э', L'Я', L'Ч', L'С', L'М', L'И', L'Т', L'Ь',
////    L'Б', L'Ю',
////
////    // Latin uppercase characters
////    L'Q', L'W', L'E', L'R', L'T', L'Y', L'L', L'I', L'O', L'P',
////    L'A', L'S', L'D', L'F', L'G', L'H', L'J', L'K', L'L', L'Z',
////    L'X', L'C', L'V', L'B', L'N', L'M',
////
////    // Numeric characters
////    L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'0',
////
////    // Special symbols
////    L'-', L'=', L'!', L'@', L'#', L'$', L'%', L'^', L'&', L'*',
////    L'(', L')', L'_', L'+', L'`', L'ё', L'!', L'"', L';', L'%',
////    L':', L'?', L'.', L'/', L'\\', L'<', L'>', L',', L' ',
////
////    // Latin lowercase characters
////    L'q', L'w', L'e', L'r', L't', L'y', L'L', L'i', L'o', L'p',
////    L'a', L's', L'd', L'f', L'g', L'h', L'j', L'k', L'l', L'z',
////    L'x', L'c', L'v', L'b', L'n', L'm',
////
////    // Additional symbols and characters
////    L'[', L']', L'{', L'}', L'U', L'Ё'
////};
//
//unsigned int const ALPHABET[] = 
//{
//    // Cyrillic lowercase characters
//    0x0439, 0x044D, 0x0446, 0x0443, 0x043A, 0x0435, 0x043D, 0x0433, 0x0448, 0x0449,
//    0x0437, 0x0445, 0x044A, 0x0444, 0x044B, 0x0432, 0x0430, 0x043F, 0x0440, 0x043E,
//    0x043B, 0x0434, 0x0436, 0x044D, 0x044F, 0x0447, 0x0441, 0x043C, 0x0438, 0x0442,
//    0x044C, 0x0431, 0x044E,
//
//    // Cyrillic uppercase characters
//    0x0419, 0x0426, 0x0423, 0x041A, 0x0415, 0x041D, 0x0413, 0x0428, 0x0429, 0x0417,
//    0x0425, 0x042A, 0x0424, 0x042B, 0x0412, 0x0410, 0x041F, 0x0420, 0x041E, 0x041B,
//    0x0414, 0x0416, 0x042D, 0x042F, 0x0427, 0x0421, 0x041C, 0x0418, 0x0422, 0x042C,
//    0x0411, 0x042E,
//
//    // Latin uppercase characters
//    0x0051, 0x0057, 0x0045, 0x0052, 0x0054, 0x0059, 0x004C, 0x0049, 0x004F, 0x0050,
//    0x0041, 0x0053, 0x0044, 0x0046, 0x0047, 0x0048, 0x004A, 0x004B, 0x004C, 0x005A,
//    0x0058, 0x0043, 0x0056, 0x0042, 0x004E, 0x004D,
//
//    // Numeric characters
//    0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x0030,
//
//    // Special symbols
//    0x002D, 0x003D, 0x0021, 0x0040, 0x0023, 0x0024, 0x0025, 0x005E, 0x0026, 0x002A,
//    0x0028, 0x0029, 0x005F, 0x002B, 0x0060, 0x0451, 0x0021, 0x0022, 0x003B, 0x0025,
//    0x003A, 0x003F, 0x002E, 0x002F, 0x005C, 0x003C, 0x003E, 0x002C, 0x0020,
//
//    // Latin lowercase characters
//    0x0071, 0x0077, 0x0065, 0x0072, 0x0074, 0x0079, 0x006C, 0x0069, 0x006F, 0x0070 ,
//    0x0061, 0x0073, 0x0064, 0x0066, 0x0067, 0x0068, 0x006A, 0x006B, 0x006C, 0x007A,
//    0x0078, 0x0063, 0x0076, 0x0062, 0x006E, 0x006D,
//
//    // Additional symbols and characters
//    0x005B, 0x005D, 0x007B, 0x007D, 0x0055, 0x0401
//};
//#endif // ALPHABETSTORAGE_H
