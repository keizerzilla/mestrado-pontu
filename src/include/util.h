/**
 * \file util.h
 * \author Artur Rodrigues Rocha Neto
 * \date 2018
 * \brief Arquivo com funções utilitárias genéricas.
 */

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

/**
 * @brief Mensagem de nível de erro
 * @param msg A mensagem a ser exibida
 */
void util_error(const char* msg, ...)
{
    char buffer[80];
    va_list arglist;

    va_start(arglist, msg);
    vsprintf(buffer, msg, arglist);
    va_end(arglist);

    fprintf(stderr, "%s[ERRO]:\t%s\n", KRED, buffer);
}

/**
 * @brief Mensagem de nível de informação/aviso
 * @param msg A mensagem a ser exibida
 */
void util_info(const char* msg, ...)
{
    char buffer[80];
    va_list arglist;

    va_start(arglist, msg);
    vsprintf(buffer, msg, arglist);
    va_end(arglist);

    fprintf(stdout, "%s[INFO]:\t%s\n", KGRN, buffer);
}

/**
 * @brief Mensagem de nível de programa
 * @param msg A mensagem a ser exibida
 */
void util_prompt(const char* msg, ...)
{
    char buffer[80];
    va_list arglist;

    va_start(arglist, msg);
    vsprintf(buffer, msg, arglist);
    va_end(arglist);

    fprintf(stdout, "%s[TUTU]:\t%s\n", KWHT, buffer);
}

/**
 * @brief Mensagem de nível de segmentação
 * @param msg A mensagem a ser exibida
 */
void util_seg(const char* msg, ...)
{
    char buffer[80];
    va_list arglist;

    va_start(arglist, msg);
    vsprintf(buffer, msg, arglist);
    va_end(arglist);

    fprintf(stdout, "%s[>>>>]:\t%s\n", KGRN, buffer);
}

#endif
