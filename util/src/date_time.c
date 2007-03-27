/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axis2_date_time.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <axis2_error.h>
#include <axis2_utils.h>
#include <axis2_date_time_util.h>

struct axis2_date_time
{
    int year;
    int mon;
    int day;
    int hour;
    int min;
    int sec;
    int msec;
};

AXIS2_EXTERN axis2_date_time_t * AXIS2_CALL
axis2_date_time_create_with_offset(const axis2_env_t *env, int offset)
{
    axis2_date_time_t *date_time = NULL;
    time_t t;
    struct tm* utc_time = NULL;
    /*struct tm* utc_time_ret = NULL;*/
    AXIS2_ENV_CHECK(env, NULL);

    date_time = (axis2_date_time_t *) AXIS2_MALLOC(env->
            allocator, sizeof(axis2_date_time_t));

    if (!date_time)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    t = time(NULL) + offset;

    utc_time = gmtime(&t);

    date_time->year = utc_time->tm_year;
    date_time->mon = utc_time->tm_mon;
    date_time->day = utc_time->tm_mday;
    date_time->hour = utc_time->tm_hour;
    date_time->min = utc_time->tm_min;
    date_time->sec = utc_time->tm_sec;
    date_time->msec = axis2_get_milliseconds(env);

    return date_time;
}


AXIS2_EXTERN axis2_date_time_t * AXIS2_CALL
axis2_date_time_create(const axis2_env_t *env)
{
    return axis2_date_time_create_with_offset(env, 0);
}


AXIS2_EXTERN void AXIS2_CALL
axis2_date_time_free(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (date_time)
    {
        AXIS2_FREE(env->allocator, date_time);
    }
    return;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_date_time_deserialize_time(axis2_date_time_t *date_time,
        const axis2_env_t *env,
        const axis2_char_t* time_str)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    sscanf(time_str, "%d:%d:%d:%dZ" , &date_time->hour, &date_time->min,
            &date_time->sec, &date_time->msec);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_date_time_deserialize_date(axis2_date_time_t *date_time,
        const axis2_env_t *env,
        const axis2_char_t* date_str)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    sscanf(date_str, "%d-%d-%d" , &date_time->year, &date_time->mon,
            &date_time->day);
    date_time->year -= 1900;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_date_time_deserialize_date_time(axis2_date_time_t *date_time,
        const axis2_env_t *env,
        const axis2_char_t* date_time_str)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    sscanf(date_time_str, "%d-%d-%dT%d:%d:%d.%dZ", &date_time->year, &date_time->mon,
            &date_time->day, &date_time->hour, &date_time->min,
            &date_time->sec, &date_time->msec);
    date_time->year -= 1900;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_date_time_set_date_time(axis2_date_time_t* date_time,
        const axis2_env_t *env,
        int year, int month, int day,
        int hour, int min, int second, int milliseconds)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (year > -1)date_time->year = year - 1900;
    if (month > -1)date_time->mon = month;
    if (day > -1)date_time->day = day;
    if (hour > -1)date_time->hour = hour;
    if (min > -1)date_time->min = min;
    if (second > -1)date_time->sec = second;
    if (second > -1)date_time->msec = milliseconds;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_date_time_serialize_time(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    axis2_char_t* time_str = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    time_str = (axis2_char_t*)AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * 32);

    sprintf(time_str, "%d:%d:%d.%dZ" , date_time->hour, date_time->min, date_time->sec,date_time->msec);
    return time_str;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_date_time_serialize_date(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    axis2_char_t* date_str = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    date_str = (axis2_char_t*)AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * 32);

    sprintf(date_str, "%d-%d-%d" , date_time->year + 1900,
            date_time->mon,
            date_time->day);
    return date_str;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_date_time_serialize_date_time(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    axis2_char_t* date_time_str = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    date_time_str = AXIS2_MALLOC(env->allocator, sizeof(char) * 32);
    sprintf(date_time_str, "%d-%02d-%02dT%02d:%02d:%02d.%03dZ" , date_time->year + 1900,
            date_time->mon + 1, date_time->day, date_time->hour, date_time->min,
            date_time->sec, date_time->msec);
    return date_time_str;
}

AXIS2_EXTERN int AXIS2_CALL
axis2_date_time_get_year(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return (date_time->year + 1900);
}

AXIS2_EXTERN int AXIS2_CALL
axis2_date_time_get_month(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    return (date_time->mon);
}

AXIS2_EXTERN int AXIS2_CALL
axis2_date_time_get_date(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    return (date_time->day);
}

AXIS2_EXTERN int AXIS2_CALL
axis2_date_time_get_hour(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    return (date_time->hour);
}

AXIS2_EXTERN int AXIS2_CALL
axis2_date_time_get_minute(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    return (date_time->min);
}

AXIS2_EXTERN int AXIS2_CALL
axis2_date_time_get_second(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    return (date_time->sec);
}

AXIS2_EXTERN int AXIS2_CALL
axis2_date_time_get_msec(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    return (date_time->msec);
}

