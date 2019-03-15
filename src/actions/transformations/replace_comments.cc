/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */

#include "src/actions/transformations/replace_comments.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <cstring>

#include "modsecurity/transaction.h"
#include "src/actions/transformations/transformation.h"


namespace modsecurity {
namespace actions {
namespace transformations {


void ReplaceComments::execute(Transaction *t,
    ModSecString &in,
    ModSecString &out) {
    uint64_t i, j, incomment;

    char *input = reinterpret_cast<char *>(
        malloc(sizeof(char) * in.size() + 1));
    memcpy(input, in.c_str(), in.size() + 1);
    input[in.size()] = '\0';

    i = j = incomment = 0;
    while (i < in.size()) {
        if (incomment == 0) {
            if ((input[i] == '/') && (i + 1 < in.size())
                && (input[i + 1] == '*')) {
                incomment = 1;
                i += 2;
            } else {
                input[j] = input[i];
                i++;
                j++;
            }
        } else {
            if ((input[i] == '*') && (i + 1 < in.size())
                && (input[i + 1] == '/')) {
                incomment = 0;
                i += 2;
                input[j] = ' ';
                j++;
            } else {
                i++;
            }
        }
    }

    if (incomment) {
        input[j++] = ' ';
    }

    out.append(reinterpret_cast<char *>(input), j);

    free(input);
}

}  // namespace transformations
}  // namespace actions
}  // namespace modsecurity
