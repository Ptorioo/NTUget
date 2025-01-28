#ifndef API_PARSER_H
#define API_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "json.h"
#include "request.h"
#include "utils.h"

#define UNREAD_COUNT_URL "https://cool.ntu.edu.tw/api/v1/conversations/unread_count"
#define COURSE_URL "https://cool.ntu.edu.tw/api/v1/courses/"
#define DASHBOARD_URL "https://cool.ntu.edu.tw/api/v1/dashboard/dashboard_cards"
#define PLANNER_URL "https://cool.ntu.edu.tw/api/v1/planner/items"

void parser_unreadCount(void);

void parser_courseInfo(const int id);

void parser_dashboard(void);

void parser_planner(void);

#endif
