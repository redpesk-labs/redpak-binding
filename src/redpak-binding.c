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

//////////////////////////////////////////////////////////////////////////////
//                             INCLUDES                                     //
//////////////////////////////////////////////////////////////////////////////

#define _GNU_SOURCE

// --- Standard includes
#include <string.h>
#include <stdio.h>
#include <signal.h>

// --- JSON includes
#include <json-c/json.h>
#include <rp-utils/rp-jsonc.h>

// --- Binding includes
#define AFB_BINDING_VERSION 4
#include <afb/afb-binding.h>

// --- Project include
#include <redconf-node.h>
#include <redconf-merge.h>
#include "utils.h"

//////////////////////////////////////////////////////////////////////////////
//                             DEFINE                                       //
//////////////////////////////////////////////////////////////////////////////

#define STATUS_SUCCESS          0
#define STATUS_ERROR            -1
#define WRONG_ARG_WARNING       "Number or type of arguments is invalid"

//////////////////////////////////////////////////////////////////////////////
//                             GLOBAL VARIABLES                             //
//////////////////////////////////////////////////////////////////////////////

#include "redpak-binding-apidef.h"

//////////////////////////////////////////////////////////////////////////////
//                             PRIVATE FUNCIONS                             //
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief Send an error reply for the request
 *
 * @param[in] request       Verb request
 * @param[in] func_name     Calling function for logging
 * @param[in] error_str     Error subject
 */
static void _error_response(afb_req_t request, const char *func_name, const char *error_str) {
    afb_data_t reply;
    AFB_ERROR("[%s] %s", func_name, error_str);
    afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, error_str, strlen(error_str) + 1, NULL, NULL);
    afb_req_reply(request, STATUS_ERROR, 1, &reply);
    return;
}

/**
 * @brief Signal handler to free correctly memory
 *
 * @param signum Id of the signal caught
 * @param arg global userData struct
 */
static void _signal_handler(const int signum) {
    switch (signum)
    {
    case SIGINT:
        printf("/!\\ SIGINT caught\n");
        break;

    case SIGSEGV:
        printf("/!\\ SIGSEGV caught\n");
        break;

    default:
        printf("/!\\ UNKNOWN signal caught\n");
        break;
    }
    exit(signum);
}

static const char *getMergeConfig(const char *redpath, size_t *len, int expand) {
    char *output = NULL;
    redNodeT *redleaf = RedNodesScan(redpath, 0, 0);
    if (!redleaf)
        goto OnExit;

    redNodeT *mergedNode = mergeNode(redleaf, NULL, expand, 1);
    if (!mergedNode)
        goto OnExitFreeLeaf;

    if(RedGetConfigYAML(&output, len, mergedNode->config)) {
        fprintf(stderr, "Issue getting yaml string merged config");
    }

    freeRedLeaf(mergedNode);
OnExitFreeLeaf:
    freeRedLeaf(redleaf);
OnExit:
    return output;
}

/**
 * @brief Main function for all verb related to app in node
 *
 * @param[in] request   Verb request
 * @param[in] argc      Count of args
 * @param[in] argv      Array of args
 * @param[in] action    Kind of action to apply on the app
 * @return none
 */
static void _app_node_manager(afb_req_t request, unsigned argc, afb_data_t const argv[], utils_action_app_t action) {
    afb_data_t reply;
    afb_data_t arg_data;
    json_object * args_json = NULL;
    char *red_path = NULL;
    char *app_name = NULL;
    char *response_msg = NULL;
    char *error_msg = NULL;
    int response_length = 0;
    int error_length = 0;
    int ret = 0;

    // Get args - need one argument
    if (argc != 1)
        goto errorArgsExit;
    if (afb_data_convert(argv[0], AFB_PREDEFINED_TYPE_JSON_C, &arg_data) < 0)
        goto errorArgsExit;

    // convert the data
    args_json = (json_object *) afb_data_ro_pointer(arg_data);
    if (!args_json)
        goto errorArgsExit;

    ret = rp_jsonc_unpack(args_json, "{s:s s?s*}"
                , "redpath", &red_path
                , "appname", &app_name);
    if (ret < 0)
        goto errorArgsExit;
    if (red_path == NULL)
        goto errorArgsExit;
    afb_data_unref(arg_data);

    // call the util function and send response
    ret = utils_manage_app(red_path, app_name, action);
    if(ret) {
        error_length = asprintf(&error_msg, "%s", utils_parse_error(ret));
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, error_msg, (size_t) error_length+1, free, error_msg);
        afb_req_reply(request, ret, 1, &reply);
        return;
    }
    switch (action) {
        case APP_ACTION_INSTALL:
            response_length = asprintf(&response_msg, "App %s has been well installed on node %s", app_name, red_path);
            break;
        case APP_ACTION_UPDATE:
            response_length = asprintf(&response_msg, "App %s has been well updated on node %s", app_name, red_path);
            break;
        case APP_ACTION_REMOVE:
            response_length = asprintf(&response_msg, "App %s has been well removed on node %s", app_name, red_path);
            break;
        case APP_ACTION_LIST:
            break;
    }

    afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, response_msg, (size_t) response_length + 1 , free, response_msg);
    afb_req_reply(request, STATUS_SUCCESS, 1, &reply);
    return;

errorArgsExit:
    AFB_ERROR("[%s] %s", __func__, WRONG_ARG_WARNING);
    afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, WRONG_ARG_WARNING, sizeof(WRONG_ARG_WARNING) + 1, NULL, NULL);
    afb_req_reply(request, STATUS_ERROR, 1, &reply);
    return;
}

//////////////////////////////////////////////////////////////////////////////
//                             VERB FUNCTIONS                               //
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief Check if binding running well by ping-pong request
 *
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
static void ping(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    char *response_msg;
    int response_length = 0;
    static char error_msg[] = "Failed with the ping request due to asprintf";
    static int pong = 0;
    afb_data_t reply;

    pong = pong + 1;
    response_length = asprintf(&response_msg, "Pong = %d", pong);
    if ( response_length < 0) {
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, error_msg, sizeof(error_msg)+1, NULL, NULL);
        afb_req_reply(request, STATUS_ERROR, 1, &reply);
        return;
    }
    afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, response_msg, (size_t) response_length + 1 , free, response_msg);
    afb_req_reply(request, STATUS_SUCCESS, 1, &reply);
}

/**
 * @brief Get the redpath root for all Node
 *
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
static void getroot(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    int ret = 0;
    afb_data_t reply;
    static char errorMsg[] = "Failed to get redRoot";

    // No function in redpak to give the ROOT_REDPATH - HArdcode this one in waiting
    afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, ROOT_REDPATH_DEFAULT, sizeof(ROOT_REDPATH_DEFAULT)+1, NULL, NULL);
    afb_req_reply(request, STATUS_SUCCESS, 1, &reply);

    return;
}

/**
 * @brief Get the tree of a node and this children according to the depth
 *
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
static void gettree(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    afb_data_t arg_data;
    afb_data_t reply;
    json_object *args_json = NULL;
    json_object *response_json = json_object_new_object();
    char *redpath = NULL;
    int depth = 0;
    char *error_msg = NULL;
    int error_length = 0;
    int ret = 0;

    // Get args - need one argument
    if (argc != 1)
        goto errorArgsExit;
    if (afb_data_convert(argv[0], AFB_PREDEFINED_TYPE_JSON_C, &arg_data) < 0)
        goto errorArgsExit;

    // convert the data
    args_json = (json_object *) afb_data_ro_pointer(arg_data);
    if (!args_json)
        goto errorArgsExit;

    AFB_DEBUG("json args: %s", json_object_get_string(args_json));
    ret = rp_jsonc_unpack(args_json, "{s?s s?i}"
                , "redpath", &redpath
                , "depth", &depth);
    if (ret < 0)
        goto errorArgsExit;
    afb_data_unref(arg_data);

    // call the util function and send response
    ret = utils_get_tree(response_json, (redpath)?redpath:ROOT_REDPATH_DEFAULT, depth);
    if( ret < 0) {
        error_length = asprintf(&error_msg, "%s", utils_parse_error(ret));
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, error_msg, (size_t) error_length+1, free, error_msg);
        afb_req_reply(request, ret, 1, &reply);
    } else {
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_JSON_C, response_json, 0, (void *)(void *)json_object_put, response_json);
        afb_req_reply(request, STATUS_SUCCESS, 1, &reply);
    }

    return;

errorArgsExit:
    _error_response(request, __func__, WRONG_ARG_WARNING);
}

/**
 * @brief Get the config of a Node
 *
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
static void getconfig(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    afb_data_t reply;
    afb_data_t arg_data;
    json_object *args_json = NULL;
    char *red_path = NULL;
    int isMerged = 0;
    int isExpanded = 1;
    char *error_msg = NULL;
    int error_length = 0;
    char *conf_str = NULL;
    size_t conf_len;
    int ret = 0;

    // Get args - need one argument
    if (argc != 1)
        goto errorArgsExit;
    if (afb_data_convert(argv[0], AFB_PREDEFINED_TYPE_JSON_C, &arg_data) < 0)
        goto errorArgsExit;

    // convert the data
    args_json = (json_object *) afb_data_ro_pointer(arg_data);
    if (!args_json)
        goto errorArgsExit;

    ret = rp_jsonc_unpack(args_json, "{s:s s?i s?i}"
                , "redpath", &red_path
                , "merged", &isMerged
                , "expand", &isExpanded);
    if (ret < 0)
        goto errorArgsExit;
    if (red_path == NULL)
        goto errorArgsExit;
    afb_data_unref(arg_data);

    if (isMerged) {
        conf_str = (char *) getMergeConfig(red_path, &conf_len, (int) isExpanded);
        if (!conf_str)
            ret = -1;
    } else {
        redNodeT *red_node = RedNodesScan(red_path, 0, 1);
        if (red_node == NULL) {
            error_length = asprintf(&error_msg, "Failed to scan node %s", red_path);
            afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, error_msg, (size_t) error_length+1, free, error_msg);
            afb_req_reply(request, STATUS_ERROR, 1, &reply);
            return;
        }
        ret = RedGetConfigYAML(&conf_str, &conf_len, red_node->config);
        freeRedLeaf(red_node);
    }

    if ( ret < 0) {
        error_length = asprintf(&error_msg, "Failed to get the config of the node %s", red_path);
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, error_msg, (size_t) error_length+1, free, error_msg);
        afb_req_reply(request, STATUS_ERROR, 1, &reply);
    } else {
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, conf_str, conf_len, free, conf_str);
        afb_req_reply(request, STATUS_SUCCESS, 1, &reply);
    }
    return;

errorArgsExit:
    _error_response(request, __func__, WRONG_ARG_WARNING);
}

/**
 * @brief Create a rednode by installing an rpm
 *
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
static void node_createRpm(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    afb_data_t reply;
    static char todo_msg[] = "Will be available soon";
    afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, todo_msg, sizeof(todo_msg) + 1, NULL, NULL);
    afb_req_reply(request, STATUS_SUCCESS, 1, &reply);
    return;
}

/**
 * @brief Delete rpm installed rednode by its package name
 *
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
static void node_deleteRpm(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    afb_data_t reply;
    static char todo_msg[] = "Will be available soon";
    afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, todo_msg, sizeof(todo_msg) + 1, NULL, NULL);
    afb_req_reply(request, STATUS_SUCCESS, 1, &reply);
    return;
}

/**
 * @brief Create a rednode by its redpath
 *
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
static void node_create(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    afb_data_t reply;
    afb_data_t arg_data;
    json_object * args_json = NULL;
    char *red_path = NULL;
    char *repo_path = NULL;
    char *response_msg = NULL;
    char *error_msg = NULL;
    int response_length = 0;
    int error_length = 0;
    int ret = 0;

    // Get args - need one argument
    if (argc != 1)
        goto errorArgsExit;
    if (afb_data_convert(argv[0], AFB_PREDEFINED_TYPE_JSON_C, &arg_data) < 0)
        goto errorArgsExit;

    // convert the data
    args_json = (json_object *) afb_data_ro_pointer(arg_data);
    if (!args_json)
        goto errorArgsExit;

    ret = rp_jsonc_unpack(args_json, "{s:s ss*}"
                , "redpath", &red_path
                , "repopath", &repo_path);
    if (ret < 0)
        goto errorArgsExit;
    if (red_path == NULL)
        goto errorArgsExit;
    afb_data_unref(arg_data);

    // call the util function and send response
    ret = utils_create_node(red_path, repo_path);
    if( ret < 0) {
        error_length = asprintf(&error_msg, "%s", utils_parse_error(ret));
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, error_msg, (size_t) error_length+1, free, error_msg);
        afb_req_reply(request, ret, 1, &reply);
        return;
    }

    response_length = asprintf(&response_msg, "Rednode %s created with success", red_path);
    afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, response_msg, (size_t) response_length + 1 , free, response_msg);
    afb_req_reply(request, STATUS_SUCCESS, 1, &reply);
    return;

errorArgsExit:
    _error_response(request, __func__, WRONG_ARG_WARNING);
}

/**
 * @brief Delete a rednode by its redpath
 *
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
static void node_delete(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    afb_data_t reply;
    afb_data_t arg_data;
    json_object * args_json = NULL;
    char *red_path = NULL;
    char *response_msg = NULL;
    char *error_msg = NULL;
    int response_length = 0;
    int error_length = 0;
    int ret = 0;

    // Get args - need one argument
    if (argc != 1)
        goto errorArgsExit;
    if (afb_data_convert(argv[0], AFB_PREDEFINED_TYPE_JSON_C, &arg_data) < 0)
        goto errorArgsExit;

    // convert the data
    args_json = (json_object *) afb_data_ro_pointer(arg_data);
    if (!args_json)
        goto errorArgsExit;

    ret = rp_jsonc_unpack(args_json, "{s:s}"
                , "redpath", &red_path);
    if (ret < 0)
        goto errorArgsExit;
    if (red_path == NULL)
        goto errorArgsExit;
    afb_data_unref(arg_data);

    // call the util function and send response
    ret = utils_delete_node(red_path);
    if( ret < 0) {
        error_length = asprintf(&error_msg, "%s", utils_parse_error(ret));
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, error_msg, (size_t) error_length+1, free, error_msg);
        afb_req_reply(request, ret, 1, &reply);
        return;
    }

    response_length = asprintf(&response_msg, "Rednode %s has been deleted with success", red_path);
    afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, response_msg, (size_t) response_length + 1 , free, response_msg);
    afb_req_reply(request, STATUS_SUCCESS, 1, &reply);
    return;

errorArgsExit:
    _error_response(request, __func__, WRONG_ARG_WARNING);
}


/**
 * @brief Install an app by app name in a node by redpath
 *
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
static void app_install(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    _app_node_manager(request, argc, argv, APP_ACTION_INSTALL);
}

/**
 * @brief Update an app by app name in a node by redpath
 *
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
static void app_update(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    _app_node_manager(request, argc, argv, APP_ACTION_UPDATE);
}

/**
 * @brief Remove an app by app name in a node by redpath
 *
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
static void app_remove(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    _app_node_manager(request, argc, argv, APP_ACTION_REMOVE);
}

/**
 * @brief List all apps in a node by redpath
 *
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
static void app_list(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    afb_data_t reply;
    afb_data_t arg_data;
    json_object * args_json = NULL;
    char * installed_apps = NULL;
    char *red_path = NULL;
    char *error_msg = NULL;
    int error_length = 0;
    int ret = 0;

    // Get args - need one argument
    if (argc != 1)
        goto errorArgsExit;
    if (afb_data_convert(argv[0], AFB_PREDEFINED_TYPE_JSON_C, &arg_data) < 0)
        goto errorArgsExit;

    // convert the data
    args_json = (json_object *) afb_data_ro_pointer(arg_data);
    if (!args_json)
        goto errorArgsExit;

    ret = rp_jsonc_unpack(args_json, "{s:s}"
                , "redpath", &red_path);
    if (ret < 0)
        goto errorArgsExit;
    if (red_path == NULL)
        goto errorArgsExit;
    afb_data_unref(arg_data);

    // call the util function and send response
    ret = utils_list_apps(red_path, &installed_apps);
    if( ret < 0) {
        error_length = asprintf(&error_msg, "%s", utils_parse_error(ret));
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, error_msg, (size_t) error_length+1, free, error_msg);
        afb_req_reply(request, ret, 1, &reply);
        return;
    }

    // response_length = asprintf(&response_msg, "%s", red_path);
    afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, installed_apps, (size_t) installed_apps + 1 , free, installed_apps);
    afb_req_reply(request, STATUS_SUCCESS, 1, &reply);
    return;

errorArgsExit:
    _error_response(request, __func__, WRONG_ARG_WARNING);
}

//////////////////////////////////////////////////////////////////////////////
//                             BINDING FUNCTIONS                            //
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief Initialize the binding
 *
 * @param main api afb api struct
 * @return 0 in success, negative otherwise
 */
static int _init_binding(afb_api_t api) {
    int ret = 0;
    return ret;
}

//////////////////////////////////////////////////////////////////////////////
//                             BINDING DEFINITION                           //
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief Binding Callback
 *
 * @param api       the api that receive the callback
 * @param ctlid     identifier of the reason of the call (@see afb_ctlid)
 * @param ctlarg    data associated to the call
 * @param userdata  the userdata of the api (@see afb_api_get_userdata)
 */
int binding_ctl(afb_api_t api, afb_ctlid_t ctlid, afb_ctlarg_t ctlarg, void *userdata) {
    switch(ctlid) {
        case afb_ctlid_Root_Entry:
            AFB_API_NOTICE(api, "Unexpected Root Entry at path=%s", ctlarg->root_entry.path);
            break;

        case afb_ctlid_Pre_Init:
            break;

        case afb_ctlid_Init:
            AFB_API_NOTICE(api, "Initialization");
            signal(SIGSEGV, _signal_handler);
            signal(SIGINT, _signal_handler);
            if ( _init_binding(api) < 0) {
                AFB_API_ERROR(api, "Failed during Initialization");
                return -1;
            }
            AFB_API_NOTICE(api, "Initialization finished");
            break;

        case afb_ctlid_Class_Ready:
            AFB_API_NOTICE(api, "Required classes are ready");
            break;

        case afb_ctlid_Orphan_Event:
            AFB_API_NOTICE(api, "The event %s is not handled", ctlarg->orphan_event.name);
            break;

        case afb_ctlid_Exiting:
            AFB_API_NOTICE(api, "Shuting down was called wide code = %i", ctlarg->exiting.code);
            break;

        default:
            break;
    }
    return 0;
}
