/**
 * @file message_parser.h
 * @date 2022-05-21
 * 
 * @copyright Copyright Dumitrescu Alexandra (c) 2022
 *      Methods used for retrieving cookie/JWT_Token, for
 *      treating errors, for formatting and for printing
 *      output messages.
 */
#ifndef _MESSAGE_PARSER_H

/** Method used for removing endl, C is dumb */
void remove_endl(char *command);

/** Method used for retrieving token */
void retrieve_token(char *response, char *token);

/** Method used for retrieving cookie */
void retrieve_cookie(char *response, char *cookie);

/** Method used for checking errors in received response */
int check_error(char *response);

/** Method used for printing REGISTER output */
void REGISTER_print_output(char *response);

/** Method used for printing LOGIN output */
int LOGIN_print_output(char *response);

/** Method used for printing ADD_BOOK output */
void ADD_BOOK_print_output(char *response);

/** Method used for printing DELETE output */
void DELETE_print_output(char *response);

/** Method used for printing LOGOUT output */
void LOGOUT_print_output(char *response);

#endif