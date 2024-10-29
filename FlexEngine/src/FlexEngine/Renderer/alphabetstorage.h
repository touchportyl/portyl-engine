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

#ifndef ALPHABETSTORAGE_H
#define ALPHABETSTORAGE_H

 // @brief Constant array to store characters from various alphabets and symbols.
unsigned int const ALPHABET[] = {
    // Cyrillic lowercase characters
    L'й', L'э', L'ц', L'у', L'к', L'е', L'н', L'г', L'ш', L'щ',
    L'з', L'х', L'ъ', L'ф', L'ы', L'в', L'а', L'п', L'р', L'о',
    L'л', L'д', L'ж', L'э', L'я', L'ч', L'с', L'м', L'и', L'т',
    L'ь', L'б', L'ю',

    // Cyrillic uppercase characters
    L'Й', L'Ц', L'У', L'К', L'Е', L'Н', L'Г', L'Ш', L'Щ', L'З',
    L'Х', L'Ъ', L'Ф', L'Ы', L'В', L'А', L'П', L'Р', L'О', L'Л',
    L'Д', L'Ж', L'Э', L'Я', L'Ч', L'С', L'М', L'И', L'Т', L'Ь',
    L'Б', L'Ю',

    // Latin uppercase characters
    L'Q', L'W', L'E', L'R', L'T', L'Y', L'L', L'I', L'O', L'P',
    L'A', L'S', L'D', L'F', L'G', L'H', L'J', L'K', L'L', L'Z',
    L'X', L'C', L'V', L'B', L'N', L'M',

    // Numeric characters
    L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'0',

    // Special symbols
    L'-', L'=', L'!', L'@', L'#', L'$', L'%', L'^', L'&', L'*',
    L'(', L')', L'_', L'+', L'`', L'ё', L'!', L'"', L';', L'%',
    L':', L'?', L'.', L'/', L'\\', L'<', L'>', L',', L' ',

    // Latin lowercase characters
    L'q', L'w', L'e', L'r', L't', L'y', L'L', L'i', L'o', L'p',
    L'a', L's', L'd', L'f', L'g', L'h', L'j', L'k', L'l', L'z',
    L'x', L'c', L'v', L'b', L'n', L'm',

    // Additional symbols and characters
    L'[', L']', L'{', L'}', L'U', L'Ё'
};

#endif // ALPHABETSTORAGE_H
