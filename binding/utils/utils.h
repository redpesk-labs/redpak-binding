/*
 * Copyright (C) 2022 "IoT.bzh"
 *
 * Author: Valentin Lefebvre <valentin.lefebvre@iot.bzh>
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

//////////////////////////////////////////////////////////////////////////////
//                             DEFINE                                       //
//////////////////////////////////////////////////////////////////////////////

#define _GNU_SOURCE
#define AFB_BINDING_VERSION     4
#define STATUS_SUCCESS          0
#define STATUS_ERROR            -1
#define GROUP_ADMIN             "ADMIN"
#define GROUP_USER              "USER"
#define INFO_GROUP              "related verbs"
#define WRONG_ARG_WARNING       "Number or type of arguments is invalid"

#define ROOT_REDPATH_DEFAULT    "/var/redpak"

//////////////////////////////////////////////////////////////////////////////
//                             INCLUDES                                     //
//////////////////////////////////////////////////////////////////////////////

// --- Binding includes
#include <afb/afb-binding.h>

// --- Project include
// #include "redwrap-cmd.h"
#include <redwrap-cmd.h>
#include <redconf-utils.h>
#include <redconf-schema.h>

// --- JSON includes
#include <json-c/json.h>
#include <wrap-json.h>

// --- Standard includes
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <dirent.h> 
#include <libgen.h>
#include <sys/wait.h> // waitpid()

//////////////////////////////////////////////////////////////////////////////
//                             STRUCTURES                                   //
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief List all potential error from utils lib
 * 
 */
typedef enum utils_error_e{
    ERROR_UTILS = 1,
    ERROR_UTILS_MALFORMATED_PATH,
    ERROR_UTILS_PARSED_REDWRAP,
    ERROR_UTILS_NO_DIR,
    ERROR_UTILS_NO_FILE,
    ERROR_UTILS_OPEN_SOURCE,
    ERROR_UTILS_OPEN_TARGET,
    ERROR_UTILS_FORBIDDEN,
    ERROR_UTILS_REMOVE,
    ERROR_UTILS_WRONG_PATH,
    ERROR_UTILS_DOWN_SCAN,
    ERROR_UTILS_NO_APPNAME,
}utils_error_t;

typedef enum utils_action_app_e{
    APP_ACTION_INSTALL = 0,
    APP_ACTION_UPDATE,
    APP_ACTION_REMOVE,
    APP_ACTION_LIST
}utils_action_app_t;

/**
 * @brief Hold binding data information
 * 
 */
typedef struct {
    json_object *info_json;
} binding_data_t;

//////////////////////////////////////////////////////////////////////////////
//                             GLOBAL VARIABLES                             //
//////////////////////////////////////////////////////////////////////////////

extern binding_data_t binding_data;

//////////////////////////////////////////////////////////////////////////////
//                             PUBLIC FUNCTIONS                             //
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief Stringify the utils error code
 * 
 * @param no_error  error code
 * @return error strindified
 */
const char *utils_parse_error(utils_error_t no_error);

/**
 * @brief Add a verb to a binding as well as fill the info json in binding data
 * 
 * @param api       Binding api
 * @param afb_verb  Afb verb struct all primary verb information (name, info, callback, ...)
 * @param usage     What the callback expect to work with
 * @param group     The group the verb belongs to. If NULL, will set automatically according Verb SESSION LEVEL
 * @param vcbdata   Data for the verb callback, available through req
 */
void utils_add_verb(afb_api_t api, struct afb_verb_v4 afb_verb, char *group, char *usage, char *vcbdata) ;

//////////////////////////////////////////////////////////////////////////////
//                             UTIL VERBS                                   //
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief Using redconf lib to get tree of a Node and store into a json object in specific depth
 * 
 * @param[out]  output_json child tree of a node (need to be a json object initialized)
 * @param[in]   red_path    Path to the rednode
 * @param[in]   depth       limit depth to child research
 * @return 0 in success negative otherwise
 */
utils_error_t utils_get_tree(json_object *output_json, const char *red_path, int depth);

/**
 * @brief Using redwrap lib to create Node and copy the repo file into the created node
 * 
 * @param[in] red_path Path to the rednode
 * @param[in] repo_path Path to the repo file to copy in node
 * @return 0 in success negative otherwise
 */
utils_error_t utils_create_node(const char *red_path, const char *repo_path);

/**
 * @brief remove the folder on redpath
 * 
 * @param[in] red_path Path to the rednode
 * @return 0 in success negative otherwise
 */
utils_error_t utils_delete_node(const char *red_path);

/**
 * @brief Manage an app in a node
 * 
 * @param[in] red_path  Path to the rednode
 * @param[in] app_name  Name of the App to install
 * @param[in] action    Kinf of action to apply to an app (install/update/remove)
 * @return 0 in success negative otherwise
 */
utils_error_t utils_manage_app(const char *red_path, const char *app_name, utils_action_app_t action);

/**
 * @brief list apps from a node thanks this redpath
 * 
 * @param[in]   red_path Path to the rednode
 * @param[out]  response list of apps installed, need to be free by the user
 * @return 0 in success negative otherwise
 */
utils_error_t utils_list_apps(const char *red_path, char **response);
