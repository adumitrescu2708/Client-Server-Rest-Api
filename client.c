#include <stdio.h>
#include <stdlib.h>
#include "parser/parson.h"
#include "utils/helpers.h"
#include "utils/request.h"
#include "utils/constants.h"
#include "parser/json_parser.h"
#include "parser/message_parser.h"

/**
 * @file client.c
 * @date 2022-05-20
 * @copyright Copyright Dumitrescu Alexandra 2022
 * 
 * @brief - Treats all possible commands described in @utils/constants.h
 *          and communicates with server via HTTP requests. 
 */

int main(void) {
    /** Current user's cookie */
    char *cookie = NULL;

    /** Current user's access token to library */
    char *JWT_token = NULL;

    /** Message for server */
    char *message = NULL;

    /** Response from server */
    char *response = NULL;

    while(1) {
        /** Read the command from stdin */
        char command[COMMAND_LEN];
        fgets(command, COMMAND_LEN, stdin);

        /** Open connection with server */
        int socket_fd = open_connection("34.241.4.235", 8080, AF_INET, SOCK_STREAM, 0);

        /** Register command */
        if(strcmp(command, REGISTER_COMMAND) == 0) {

            /** Obtain username and password from stdin */
            char username[USERNAME_LEN];
            char password[PASSWORD_LEN];

            printf("username=");
            fgets(username, USERNAME_LEN, stdin);
            remove_endl(username);

            printf("password=");
            fgets(password, PASSWORD_LEN, stdin);
            remove_endl(password);

            /** Parse the input into JSON format and obtain a pretty format string */
            char *register_data = JSON_compute_register_data(username, password);

            /** Compute get request */
            message = compute_post_request("34.241.4.235", "/api/v1/tema/auth/register", "application/json", &register_data, 1, NULL, 1, NULL);
            
            /** Send it to server and receive the response */
            send_to_server(socket_fd, message);
            response = receive_from_server(socket_fd);

            /** Print the output and treat errors */
            REGISTER_print_output(response);

            /** Free the memory */
            json_free_serialized_string(register_data);
            free(message);
            free(response);
        }

        /** Login command */
        if(strcmp(command, LOGIN_COMMAND) == 0) {

            /** Obtain username and password from stdin */
            char username[USERNAME_LEN];
            char password[PASSWORD_LEN];

            printf("username=");
            fgets(username, USERNAME_LEN, stdin);
            remove_endl(username);

            printf("password=");
            fgets(password, PASSWORD_LEN, stdin);
            remove_endl(password);

            /** Parse the input into JSON format and obtain a pretty format string */
            char *login_data = JSON_compute_register_data(username, password);
            message = compute_post_request("34.241.4.235", "/api/v1/tema/auth/login", "application/json", &login_data, 1, NULL, 1, NULL);
            
            /** Send it to server and receive the response */
            send_to_server(socket_fd, message);
            response = receive_from_server(socket_fd);

            /** If a client is already connected, free the cookie and
             *  realloc memory for a new cookie.
            */
            if(cookie == NULL) {
                cookie = malloc(COOKIE_LEN);
            } else {
                free(cookie);
                cookie = malloc(COOKIE_LEN);
            }

            /** Print the output and treat errors */
            if(LOGIN_print_output(response) == 1) {
                retrieve_cookie(response, cookie);
            }

            /** Free the memory */
            json_free_serialized_string(login_data);
            free(message);
            free(response);
        }

        /** Enter Library */
        if(strcmp(command, ENTER_LIBRARY_COMMAND) == 0) {

            /** If a client isn't connected, send a NULL cookie, otherwise send the cookie
             *  and compute the get request for the server
             */
            if(cookie == NULL) {
                message = compute_get_request("34.241.4.235", "/api/v1/tema/library/access", NULL, NULL, 0, NULL);
            } else {
                message = compute_get_request("34.241.4.235", "/api/v1/tema/library/access", NULL, &cookie, 1, NULL);
            }
            
            /** Send it to server and receive the response */
            send_to_server(socket_fd, message);
            response = receive_from_server(socket_fd);

            /** If no errors occur, retrieve the token from the response and
             *  print a success message. If a token was already set, free its memory
             *  and realloc memory for a new token.
            */
            if(check_error(response) == 0) {
                if(JWT_token == NULL) {
                    JWT_token = malloc(BUFLEN);
                } else {
                    free(JWT_token);
                    JWT_token = malloc(BUFLEN);
                }
                retrieve_token(response, JWT_token);
                printf("Entered library!\n");
                free(message);
                free(response);
            } else {
                free(message);
                free(response);                
            }
        }

        /** Get Books */
        if(strcmp(command, GET_BOOKS_COMMAND) == 0) {
            /** Treat cases of NULL cookie or NULL token and compute the get request */
            if(cookie == NULL) {
                message = compute_get_request("34.241.4.235", "/api/v1/tema/library/books", NULL, NULL, 0, NULL);
            } else {
                if(JWT_token== NULL) {
                    message = compute_get_request("34.241.4.235", "/api/v1/tema/library/books", NULL, &cookie, 1, NULL);
                } else {
                    message = compute_get_request("34.241.4.235", "/api/v1/tema/library/books", NULL, &cookie, 1, JWT_token);
                }
            }

            /** Send it to server and receive the response */
            send_to_server(socket_fd, message);
            response = receive_from_server(socket_fd);

            /** If no errrors occur, format the received books into a JSON
             *  and print its correpsonding fields.
            */
            if(check_error(response) == 0) {
                JSON_parse_books(response);
                free(message);
                free(response);
            } else {
                free(message);
                free(response);                
            }
        }

        /** Get book */
        if(strcmp(command, GET_BOOK_COMMAND) == 0) {

            /** Format the ID received from stdin */
            int id;
            printf("id=");
            scanf("%d", &id);

            /** Format the URL with the given ID */
            char URL[URL_LEN], ID[10];
            strcpy(URL, "/api/v1/tema/library/books/");
            sprintf(ID, "%d", id);
            strcat(URL, ID);
            
            /** Compute get request and treat cases of NULL cookie or NULL token */
            if(cookie == NULL) {
                message = compute_get_request("34.241.4.235", URL, NULL, NULL, 0, NULL);
            } else {
                if(JWT_token == NULL) {
                    message = compute_get_request("34.241.4.235", URL, NULL, &cookie, 1, NULL);
                } else {
                    message = compute_get_request("34.241.4.235", URL, NULL, &cookie, 1, JWT_token);
                }
                
            }

            /** Send it to server and receive the response */
            send_to_server(socket_fd, message);
            response = receive_from_server(socket_fd);

            /** If no errors occur, parse the given book into JSON format and
             *  print its fields.
            */
            if(check_error(response) == 0) {
                JSON_parse_book(response);
                free(response);
                free(message);
            } else {
                free(response);
                free(message);                
            }

        }

        /** Add book */
        if(strcmp(command, ADD_BOOK_COMMAND) == 0) {
            /** Obtain the fields of the book from stdin */
            char title[TITLE_LEN];
            char author[AUTHOR_LEN];
            char genre[GENRE_LEN];
            char publisher[PUBLISHER_LEN];
            int page_count = 0;

            printf("title=");
            fgets(title, TITLE_LEN, stdin);
            remove_endl(title);

            printf("author=");
            fgets(author, AUTHOR_LEN, stdin);
            remove_endl(author);

            printf("genre=");
            fgets(genre, GENRE_LEN, stdin);
            remove_endl(genre);

            printf("publisher=");
            fgets(publisher, PUBLISHER_LEN, stdin);
            remove_endl(publisher);

            /** Check if received page count is valid (a number) and parse it
             * into an integer value
            */
            char pages[100];
            int check_number = 1;
            printf("page_count=");
            fgets(pages, 100, stdin);

            for(int i = 0; i < strlen(pages) - 1; i++) {
                if(pages[i] > '9' || pages[i] < '0') {
                    check_number = -1;
                } else {
                    page_count = page_count * 10 + (pages[i] - '0');
                }
            }

            if(check_number == -1) {
                printf("Enter a valid number!\n");
                continue;
            }


            /** Parse the given input into JSON format and retrieve the message in pretty JSON format */
            char *new_book = JSON_compute_book(title, author, genre, publisher, page_count);
            
            /** Compute post request and treat cases of NULL cookie or NULL token */
            if(cookie == NULL) {
                message = compute_post_request("34.241.4.235", "/api/v1/tema/library/books", "application/json", &new_book, 1, NULL, 0, NULL);
            } else {
                if(JWT_token == NULL) {
                    message = compute_post_request("34.241.4.235", "/api/v1/tema/library/books", "application/json", &new_book, 1, &cookie, 1, NULL);
                } else {
                    message = compute_post_request("34.241.4.235", "/api/v1/tema/library/books", "application/json", &new_book, 1, &cookie, 1, JWT_token);
                }
            }

            /** Send it to server and receive the response */
            send_to_server(socket_fd, message);
            response = receive_from_server(socket_fd);

            /** Print output and treat errors */
            ADD_BOOK_print_output(response);

            /** Free the memory */
            json_free_serialized_string(new_book);
            free(message);
            free(response);
        }

        /** Delete */
        if(strcmp(command, DELETE_COMMAND) == 0) {

            /** Obtain the ID of the book from stdin */
            int id;
            printf("id=");
            scanf("%d", &id);

            /** Compute the URL with the given ID */
            char URL[URL_LEN], ID[10];
            strcpy(URL, "/api/v1/tema/library/books/");
            sprintf(ID, "%d", id);
            strcat(URL, ID);

            /** Compute the get request and treat cases of NULL cookie or NULL token */
            if(cookie == NULL) {
                message = compute_delete_request("34.241.4.235", URL, NULL, NULL, 0, NULL);
            } else {
                if(JWT_token == NULL) {
                    message = compute_delete_request("34.241.4.235", URL, NULL, &cookie, 1, NULL);
                } else {
                    message = compute_delete_request("34.241.4.235", URL, NULL, &cookie, 1, JWT_token);
                }
            }

            /** Send it to server and receive response */
            send_to_server(socket_fd, message);
            response = receive_from_server(socket_fd);

            /** Print output and treat errors */
            DELETE_print_output(response);

            /** Free memory */
            free(message);
            free(response);
        }

        /** Logout */
        if(strcmp(command, LOGOUT_COMMAND) == 0) {
            /** Compute the get request and treat case of a NULL cookie */
            if(cookie == NULL) {
                message = compute_get_request("34.241.4.235", "/api/v1/tema/auth/logout", NULL, NULL, 0, NULL);
            } else {
                message = compute_get_request("34.241.4.235", "/api/v1/tema/auth/logout", NULL, &cookie, 1, NULL);
            }
            
            /** Send it to server and receive the response */
            send_to_server(socket_fd, message);
            response = receive_from_server(socket_fd);

            /** Print output and treat errors */
            LOGOUT_print_output(response);

            /** Free memory and set token and cookie back to NULL values */
            free(message);
            free(response);
            free(cookie);
            free(JWT_token);
            JWT_token = NULL;
            cookie = NULL;
        }

        /** Exit */
        if(strcmp(command, EXIT_COMMAND) == 0) {

            /** Close the connection with the server */
            close(socket_fd);

            /** Free memory */
            if(cookie != NULL) {
                free(cookie);
            }
            if(JWT_token != NULL) {
                free(JWT_token);
            }
            break;
        }
    }

    return 0;
}