#ifndef API_PARSER_H
#define API_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "json.h"
#include "request.h"

#define UNREAD_COUNT_URL "https://cool.ntu.edu.tw/api/v1/conversations/unread_count"
#define COURSE_URL "https://cool.ntu.edu.tw/api/v1/courses/"

void parser_unreadCount();

#endif
