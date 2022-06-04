/**
 * @file constants.h
 * @date 2022-05-21
 * 
 * @copyright Copyright Dumitrescu Alexandra (c) 2022
 */
#ifndef _CONSTANTS_H

#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

/** Length of common objects */
#define COMMAND_LEN         15
#define COOKIE_LEN          200
#define USERNAME_LEN        100
#define PASSWORD_LEN        100
#define TITLE_LEN           100
#define AUTHOR_LEN          100
#define GENRE_LEN           100
#define PUBLISHER_LEN       100
#define URL_LEN             200

/** Common Errors or messages retrieved from the server */
#define ERROR_MESSAGE       "error"
#define TOKEN_ERROR_MESSAGE "Error when decoding tokenn"
#define ID_ERROR_MESSAGE    "No book was found"
#define CREATED             "201 Created"
#define SUCCESS             "200 OK"
#define CONNECT             "connect.sid"
#define TOKEN               "token"

/** List of possible commands */
#define EXIT_COMMAND                "exit\n"
#define LOGIN_COMMAND               "login\n"
#define REGISTER_COMMAND            "register\n"
#define ENTER_LIBRARY_COMMAND       "enter_library\n"
#define GET_BOOKS_COMMAND           "get_books\n"
#define ADD_BOOK_COMMAND            "add_book\n"
#define DELETE_COMMAND              "delete_book\n"
#define LOGOUT_COMMAND              "logout\n"
#define GET_BOOK_COMMAND            "get_book\n"

#endif