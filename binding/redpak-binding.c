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

//////////////////////////////////////////////////////////////////////////////
//                             DEFINES                                      //
//////////////////////////////////////////////////////////////////////////////

#define INFO_GROUP              "related verbs"
#define ROOT_REDPATH_DEFAULT    "/var/redpak"

//////////////////////////////////////////////////////////////////////////////
//                             INCLUDES                                     //
//////////////////////////////////////////////////////////////////////////////

// --- Project includes 
#include "utils.h"
#include "redpak-binding-apidef.h"

// --- Standard includes
#include <signal.h>

// --- AFb includes

//////////////////////////////////////////////////////////////////////////////
//                             GLOBAL VARIABLES                             //
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief Hold binding global data 
 * 
 */
binding_data_t binding_data = {
    .info_json = NULL,
};

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
    json_object_put(binding_data.info_json);
    exit(signum);
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

    ret = wrap_json_unpack(args_json, "{s:s ss*}"
                , "redPath", &red_path
                , "appName", &app_name);
    if (ret < 0)
        goto errorArgsExit;
    if (red_path == NULL)
        goto errorArgsExit;
    afb_data_unref(arg_data);
    
    // call the util function and send response
    ret = utils_manage_app(red_path, app_name, action);
    if( ret < 0) {
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

void ping(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
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

void info(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    afb_data_t reply;
    static char errorMsg[] = "Failed to get binding information";

    // No function in redpak to give the ROOT_REDPATH - HArdcode this one in waiting

    if (binding_data.info_json) {
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_JSON_C, binding_data.info_json, 0, NULL, NULL);
        afb_req_reply(request, STATUS_SUCCESS, 1, &reply);
    } else {
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, errorMsg, sizeof(errorMsg)+1, NULL, NULL);
        afb_req_reply(request, STATUS_ERROR, 1, &reply);
    }

    return;
}

void getRoot(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    int ret = 0;
    afb_data_t reply;
    static char errorMsg[] = "Failed to get redRoot";

    // No function in redpak to give the ROOT_REDPATH - HArdcode this one in waiting
    afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, ROOT_REDPATH_DEFAULT, sizeof(ROOT_REDPATH_DEFAULT)+1, NULL, NULL);
    afb_req_reply(request, STATUS_SUCCESS, 1, &reply);

    return;
}

void getTree(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
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
    ret = wrap_json_unpack(args_json, "{s:s s:i}"
                , "redPath", &redpath
                , "depth", &depth);
    if (ret < 0)
        goto errorArgsExit;
    if (!redpath)
        goto errorArgsExit;
    afb_data_unref(arg_data);

    // call the util function and send response
    ret = utils_get_tree(response_json, redpath, depth);
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

void getConfig(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    afb_data_t reply;
    afb_data_t arg_data;
    char *red_path = NULL;
    char *error_msg = NULL;
    int error_length = 0;
    char *conf_str = NULL;
    size_t conf_len;

    // Get args - need one argument
    if (argc != 1)
        goto errorArgsExit;
    if (afb_data_convert(argv[0], AFB_PREDEFINED_TYPE_STRINGZ, &arg_data) < 0)
        goto errorArgsExit;
    
    // convert the data
    red_path = (char *) afb_data_ro_pointer(arg_data);
    if (!red_path || !strcasecmp(red_path, "null"))
        goto errorArgsExit;
    afb_data_unref(arg_data);

    //ret = RedConfToJson(&conf_json, "/var/redpesk/redpesk-core", 1);
    redNodeT *red_node = RedNodesScan(red_path, 1);
    if( red_node == NULL) {
        error_length = asprintf(&error_msg, "Failed to get the config of the node %s", red_path);
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, error_msg, (size_t) error_length+1, free, error_msg);
        afb_req_reply(request, STATUS_ERROR, 1, &reply);
        return;
    }

    if (RedGetConfig(&conf_str, &conf_len, red_node->config) < 0) {
        error_length = asprintf(&error_msg, "Failed to get the config of the node %s", red_path);
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, error_msg, (size_t) error_length+1, free, error_msg);
        afb_req_reply(request, STATUS_ERROR, 1, &reply);
    } else {
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, conf_str, conf_len, free, conf_str);
        afb_req_reply(request, STATUS_SUCCESS, 1, &reply);
    }
    freeRedLeaf(red_node);
    return;

errorArgsExit:
    _error_response(request, __func__, WRONG_ARG_WARNING);
}

void createNodeRpm(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    afb_data_t reply;
    static char todo_msg[] = "Will be available soon";
    afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, todo_msg, sizeof(todo_msg) + 1, NULL, NULL);
    afb_req_reply(request, STATUS_SUCCESS, 1, &reply);
    return;
}

void deleteNodeRpm(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    afb_data_t reply;
    static char todo_msg[] = "Will be available soon";
    afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, todo_msg, sizeof(todo_msg) + 1, NULL, NULL);
    afb_req_reply(request, STATUS_SUCCESS, 1, &reply);
    return;
}

void createNode(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
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

    ret = wrap_json_unpack(args_json, "{s:s ss*}"
                , "redPath", &red_path
                , "repoPath", &repo_path);
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

void deleteNode(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    afb_data_t reply;
    afb_data_t arg_data;
    char *red_path = NULL;
    char *response_msg = NULL;
    char *error_msg = NULL;
    int response_length = 0;
    int error_length = 0;
    int ret = 0;

    // Get args - need one argument
    if (argc != 1)
        goto errorArgsExit;
    if (afb_data_convert(argv[0], AFB_PREDEFINED_TYPE_STRINGZ, &arg_data) < 0)
        goto errorArgsExit;
    
    // convert the data
    red_path = (char *) afb_data_ro_pointer(arg_data);
    if (!red_path || !strcasecmp(red_path, "null"))
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

void installApp(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    _app_node_manager(request, argc, argv, APP_ACTION_INSTALL);
}

void updateApp(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    _app_node_manager(request, argc, argv, APP_ACTION_UPDATE);
}

void removeApp(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    _app_node_manager(request, argc, argv, APP_ACTION_REMOVE);
}

//////////////////////////////////////////////////////////////////////////////
//                             BINDING FUNCTIONS                            //
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief Preinitialization the binding
 * 
 * @param main api afb api struct
 * @return 0 in success, negative otherwise
 */
static int _preinit_binding(afb_api_t api) {
    int ret = 0;
    int idx = 0;

    // Initiaite info json 
    json_object *groups_json = json_object_new_array();
    json_object *metadata_json = NULL;
    wrap_json_pack(&metadata_json, "{s:s s:s s:s s:s}",
        "uid", PROJECT_NAME,
        "info", PROJECT_DESCRIPTION,
        "version", PROJECT_VERSION,
        "author", PROJECT_AUTHOR
    );
    binding_data.info_json = json_object_new_object();
    json_object_object_add(binding_data.info_json, "metadata", metadata_json);
    json_object_object_add(binding_data.info_json, "groups", groups_json);
    
    // Add verb to API and in info json
    while (_afb_verbs_redpak[idx].verb != NULL) {
        utils_add_verb(api, _afb_verbs_redpak[idx], NULL, NULL, NULL);
        idx ++;
    }

	return ret;
}

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

int binding_ctl(afb_api_t api, afb_ctlid_t ctlid, afb_ctlarg_t ctlarg, void *userdata) {
    switch(ctlid) {
        case afb_ctlid_Root_Entry:
            AFB_API_NOTICE(api, "Unexpected Root Entry at path=%s", ctlarg->root_entry.path);
            break;
        
        case afb_ctlid_Pre_Init:
            AFB_API_NOTICE(api, "Pre-initialization at path=%s", ctlarg->pre_init.path);
			if (_preinit_binding(api) < 0 ){
                AFB_API_ERROR(api, "Failed during pre-initialization");
                return -1;
            }
            afb_api_seal(api);
            AFB_API_NOTICE(api, "Preinitialization finished");
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