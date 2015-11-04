/** @file BRO_spam_fists.h */
#ifndef __bro_headers_spam_fists_h
#define __bro_headers_spam_fists_h

/** @addtogroup BROSFists */
/* @{ */
#include "BRO_spam_client.h"

/** Decoding function for a set of BROFists.
 *
 *  This is the heart of the whole SPAM Client. It takes an array of
 *  BROFists, decodes them, then behave accordingly. After all the
 *  operations have been done the responses are used to create a number of
 *  BROFist to send back equal to that received containing all the data
 *  requested.
 *
 *  @param orders BROFist Array to be decoded.
 *  @param response BROFist Array with the results of the operations.
 *  @param motors The structure containing all the Servo Motors' Data.
 */
void decode_bro_fists (const bro_fist_t * orders, bro_fist_t * response, engines_t * motors);

/* @} */
#endif
