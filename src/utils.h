/*
 * Copyright (C) 2022-2026 IoT.bzh Company
 *
 * Author: Valentin Lefebvre
 * Author: IoT.bzh Dev team <dev-team@iot.bzh>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#define _GNU_SOURCE

#define ROOT_REDPATH_DEFAULT    "/var/redpak"

//////////////////////////////////////////////////////////////////////////////
//                             INCLUDES                                     //
//////////////////////////////////////////////////////////////////////////////

#include <json-c/json.h>

//////////////////////////////////////////////////////////////////////////////
//                             STRUCTURES                                   //
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief List all potential error from utils lib
 *
 */
typedef enum utils_error_e {
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
} utils_error_t;

typedef enum utils_action_app_e {
    APP_ACTION_INSTALL = 0,
    APP_ACTION_UPDATE,
    APP_ACTION_REMOVE,
    APP_ACTION_LIST
} utils_action_app_t;

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
