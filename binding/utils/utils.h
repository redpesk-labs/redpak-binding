/*
 * Copyright (C) 2022 "IoT.bzh"
 *
 * Author: Valentin Lefebvre <valentin.lefebvre@iot.bzh
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

//////////////////////////////////////////////////////////////
//                     DEFINE                               //
//////////////////////////////////////////////////////////////

#define _GNU_SOURCE
#define AFB_BINDING_VERSION     4
#define STATUS_SUCCESS          0
#define STATUS_ERROR            -1
#define ADMIN_GROUP             "ADMIN"
#define GROUP_INFO              "related verbs"
#define WRONG_ARG_WARNING       "Number or type of arguments is invalid"

//////////////////////////////////////////////////////////////
//                     INCLUDES                             //
//////////////////////////////////////////////////////////////

// Binding includes
#include <afb/afb-binding.h>

// JSON includes
#include <json-c/json.h>
#include <wrap-json.h>

// Standard includes
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////
//                   STRUCTURES                             //
//////////////////////////////////////////////////////////////

/**
 * @brief Hold binding data information
 * 
 */
typedef struct {
    json_object *info_json;
} binding_data_t;

//////////////////////////////////////////////////////////////
//                   GLOBAL VARIABLES                       //
//////////////////////////////////////////////////////////////

extern binding_data_t binding_data;

//////////////////////////////////////////////////////////////
//                   PUBLIC FUNCTIONS                       //
//////////////////////////////////////////////////////////////

/**
 * @brief Add a verb to a binding as well as fill the info json in binding data
 * 
 * @param api       Binding api
 * @param afb_verb  Afb verb struct all primary verb information (name, info, callback, ...)
 * @param usage     What the callback expect to work with
 * @param group     The group the verb belongs to
 * @param vcbdata   Data for the verb callback, available through req
 */
void utils_add_verb(afb_api_t api, struct afb_verb_v4 afb_verb, char *group, char *usage, char *vcbdata) ;

// //////////////////////////////////////////////////////////////
// //                   UTIL VERBS                             //
// //////////////////////////////////////////////////////////////

// /**
//  * @brief function linked to info callback
//  * 
//  * @param request User request
//  * @return json_object* - Binding info json
//  */
// json_object * utils_info(afb_req_t request);

// /**
//  * @brief function linked to ping callback
//  * 
//  * @param request User request
//  * @return int - number of count
//  */
// int utils_ping(afb_req_t request);
