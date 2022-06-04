/**
 * @file json_parser.h
 * @date 2022-05-21
 * 
 * @copyright Copyright (c) Dumitrescu Alexandra 2022
 * 
 * @brief   Methods used for parsing data into JSON format.
 *          Methods are used for printing information on books and
 *          retrieve errors from a JSON body.
 * 
 */
#ifndef _JSON_PARSER_H

/** Method used for pretty JSON formatting a register request */
char* JSON_compute_register_data(char *username, char* password);

/** Method used for pretty JSON formatting a new book request */
char* JSON_compute_book(char* title, char* author, char* genre, char* publisher, int page_count);

/** Method used for printing the information on books from a JSON formated string */
void JSON_parse_books(char *response);

/** Method used for printing the information on a book from a JSON formated string */
void JSON_parse_book(char *response);

/** Method used for printing the error log from a JSON formated string */
void JSON_parse_error(char *error);

#endif