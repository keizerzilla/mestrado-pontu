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

void util_errormsg(const char* msg)
{
    fprintf(stderr, msg);
}

void util_error(const char* msg)
{
    fprintf(stderr, "erro:\t%s\n", msg);
    exit(1);
}

#endif
