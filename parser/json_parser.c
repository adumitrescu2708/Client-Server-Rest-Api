/**
 * @file json_parser.c
 * 
 * @copyright Copyright Dumitrescu Alexandra (c) 2022
 */
#include "json_parser.h"
#include "../utils/constants.h"
#include "parson.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief Parses strings retrieved from stdin into
 *        JSON format and returns a string with
 *        pretty format
 * 
 * @param username
 * @param password 
 * @return pretty JSON format
 */
char* JSON_compute_register_data(char *username, char* password) {
    JSON_Value* value = json_value_init_object();
    JSON_Object* object = json_value_get_object(value);
    char* register_data = NULL;

    json_object_set_string(object, "username", username);
    json_object_set_string(object, "password", password);

    register_data = json_serialize_to_string_pretty(value);
    json_value_free(value);
    return register_data;
}
/**
 * @brief Parses the strings with fields of a book retrieved from stdin
 *       into JSON format and returns a pretty string
 * 
 * @param title 
 * @param author 
 * @param genre 
 * @param publisher 
 * @param page_count 
 * @return pretty format
 */
char* JSON_compute_book(char* title, char* author, char* genre, char* publisher, int page_count) {
    JSON_Value* value = json_value_init_object();
    JSON_Object* object = json_value_get_object(value);
    char* book_data = NULL;

    json_object_set_string(object, "title", title);
    json_object_set_string(object, "genre", genre);
    json_object_set_string(object, "author", author);
    json_object_set_number(object, "page_count", page_count);
    json_object_set_string(object, "publisher", publisher);


    book_data = json_serialize_to_string_pretty(value);
    json_value_free(value);
    return book_data;
}

/**
 * @brief From a given server message, we break each JSON element
 *        in the array list of JSON from the body and
 *        parse it into a JSON object. We then, retrieve and print each field
 * 
 * @param response retrieved from server
 */
void JSON_parse_books(char *response) {
    int start_idx = -1, end_idx = -1;
    char book_data[1000];
    for(int i = 0; i < strlen(response); i++) {
        if(response[i] == '{') {
            start_idx = i;
        }
        if(response[i] == '}' && start_idx != -1) {
            
            /** Obtain the start index and end index of the JSON component
             * and copy its content in an auxiliar string.
             *  Then, obtain the JSON object from the given string and
             * print each field of a book.
            */
            end_idx = i;
            book_data[0] = '\0';
            strncpy(book_data, response + start_idx, (end_idx - start_idx) + 1);
            book_data[(end_idx - start_idx) + 1] = '\0';

            JSON_Value* value = json_parse_string(book_data);
            JSON_Object* object = json_value_get_object(value);
            double result = json_object_get_number(object, "id");
            const char *title = json_object_get_string(object, "title");

            printf("Id: %d, Title: %s\n", (int) result, title);
            json_value_free(value);

            start_idx = -1;
        }
    }
}
/**
 * @brief From a given server message, retrieve the JSON element
 *        from the body and parse it into a JSON object.
 *        We then, retrieve and print each field of the corresponding book
 * 
 * @param response retrieved from server
 */
void JSON_parse_book(char *response) {
    int start_idx = -1, end_idx = -1;
    char book_data[100];
    for(int i = 0; i < strlen(response); i++) {
        if(response[i] == '{') {
            start_idx = i;
        }
        if(response[i] == '}') {
            end_idx = i;
            book_data[0] = '\0';
            strncpy(book_data, response + start_idx, (end_idx - start_idx) + 1);
            book_data[(end_idx - start_idx) + 1] = '\0';

            JSON_Value* value = json_parse_string(book_data);
            JSON_Object* object = json_value_get_object(value);
            double pages = json_object_get_number(object, "page_count");
            const char *genre = json_object_get_string(object, "genre");
            const char *publisher = json_object_get_string(object, "publisher");
            const char *author = json_object_get_string(object, "author");
            const char *title = json_object_get_string(object, "title");

            printf("Title: %s, Author: %s, Publisher: %s, Genre: %s, Page count: %d\n",
                            title,
                            author,
                            publisher,
                            genre,
                            (int) pages);

            json_value_free(value);

        }
    }
}

/**
 * @brief Method used for retrieving the error from a 
 * JSON object
 * 
 * @param error 
 */
void JSON_parse_error(char *error) {
    JSON_Value* value = json_parse_string(error);
    JSON_Object* object = json_value_get_object(value);
    printf("Error: %s\n", json_object_get_string(object, "error"));
    json_value_free(value);
}



