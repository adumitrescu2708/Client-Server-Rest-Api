/**
 * @file message_parser.c
 * @date 2022-05-21
 * 
 * @copyright Copyright (c) Dumitrescu Alexandra 2022
 */
#include "message_parser.h"
#include "../utils/constants.h"
#include <stdlib.h>
#include <stdio.h>
#include "json_parser.h"

/** Method used for removing endl */
void remove_endl(char* command) {
    command[strlen(command) - 1] = '\0';
}

/**
 * @brief copies the content of the received cookie in @cookie
 * param. Checks in the received response from the server the
 * beggining of a cookie "connect.sid" and parses its content
 * 
 * @param response 
 * @param cookie 
 */
void retrieve_cookie(char *response, char *cookie) {
    char *payload = strstr(response, CONNECT);
    char *token = ";";
    char *cookie_select = strtok(payload, token);
    strcpy(cookie, cookie_select);
}

/**
 * @brief copies the content of the received JWT_token in
 * param @token. Searches for the key-word "token"
 * in the received message from server.
 * 
 * @param response 
 * @param token 
 */
void retrieve_token(char *response, char *token) {
    char *body = strstr(response, TOKEN);
    strcpy(token, body + strlen(TOKEN) + 3);
    token[strlen(token) - 2] = '\0';
}

/**
 * @brief Searches in a response for an ERROR message.
 * If an error is found, we compute a JSON formatted 
 * string from the response and send in to @json_parser
 * 
 * @param response 
 * @return int -1  Error handled
 *              0  No errors found  
 */
int check_error(char *response) {
    char *error = strstr(response, ERROR_MESSAGE);
    if(error != NULL) {
        char *tokens = "}";
        char *error_aux = strtok(error, tokens);
        char error_JSON[100];
        error_JSON[0] = '{';
        error_JSON[1] = '"';
        error_JSON[2] = '\0';
        strcat(error_JSON, error_aux);
        strcat(error_JSON, "}");
        JSON_parse_error(error_JSON);
        return -1;
    }
    return 0;
}

/**
 * @brief Validates the output (checks for possible errors)
 * and prints a success message if no error was found.
 * 
 * @param response - server's response to REGISTER command
 */
void REGISTER_print_output(char *response) {
    char *error = strstr(response, ERROR_MESSAGE);
    char *success = strstr(response, CREATED);

    if(error != NULL) {
        check_error(response);
    }

    if(success != NULL) {
        printf("User created successfully!\n");
    }
}

/**
 * @brief Validates the output (checks for possible errors)
 * and prints a success message if no error was found.
 * 
 * @param response - server's response to LOGIN command
 * @return int -1  - Error was found, Login failed
 *              0  - Login successfull
 */
int LOGIN_print_output(char *response) {
    char *error = strstr(response, ERROR_MESSAGE);
    char *success = strstr(response, SUCCESS);

    if(error != NULL) {
        check_error(response);
        return -1;
    }

    if(success != NULL) {
        printf("Logged in!\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Validates the output (checks for possible errors)
 * and prints a success message if no error was found.
 * 
 * @param response - server's response to ADD_BOOK command
 */
void ADD_BOOK_print_output(char *response) {
    char *success = strstr(response, SUCCESS);
    char *error = strstr(response, ERROR_MESSAGE);
    if(error != NULL) {
        check_error(response);
    }  

    if(success != NULL) {
        printf("Entered new book successfully!\n");
    }
}

/**
 * @brief Validates the output (checks for possible errors)
 * and prints a success message if no error was found.
 * 
 * @param response - server's response to DELETE command
 */
void DELETE_print_output(char *response) {
    char *success = strstr(response, SUCCESS);
    char *error = strstr(response, ERROR_MESSAGE);

    if(success != NULL) {
        printf("Deleted book!\n");
    }
    if(error != NULL) {
        check_error(response);
    }
}

/**
 * @brief Validates the output (checks for possible errors)
 * and prints a success message if no error was found.
 * 
 * @param response - server's response to LOGOUT command
 */
void LOGOUT_print_output(char *response) {
    char *error = strstr(response, "error");
    if(error != NULL) {
        check_error(response);
    } else {
        printf("Logged out!\n");
    }

}