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
///////////////////////////////////////////////////////////////////////////////
//                             DEFINES                                       //
///////////////////////////////////////////////////////////////////////////////

#define GROUP_INFO "related verbs"

///////////////////////////////////////////////////////////////////////////////
//                             INCLUDES                                      //
///////////////////////////////////////////////////////////////////////////////

// --- Project includes 
#include "utils.h"
#include "redpak-binding-apidef.h"

// Standard includes
#include <signal.h>

///////////////////////////////////////////////////////////////////////////////
//                             GLOBAL VARIABLES                              //
///////////////////////////////////////////////////////////////////////////////

binding_data_t binding_data = {
    .info_json = NULL,
};

///////////////////////////////////////////////////////////////////////////////
//                             PRIVATE FUNCIONS                              //
///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////
//                             VERB FUNCTIONS                                //
///////////////////////////////////////////////////////////////////////////////

void ping(afb_req_t request, unsigned argc, afb_data_t const argv[]) {
    char *response_msg;
    int response_length = 0;
    static char error_msg[] = "Failled with the ping request due to asprintf";
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
    static char errorMsg[] = "[Failed to get binding information";

    if (binding_data.info_json) {
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_JSON_C, binding_data.info_json, 0, NULL, NULL);
        afb_req_reply(request, STATUS_SUCCESS, 1, &reply);
    } else {
        afb_create_data_raw(&reply, AFB_PREDEFINED_TYPE_STRINGZ, errorMsg, sizeof(errorMsg)+1, NULL, NULL);
        afb_req_reply(request, STATUS_ERROR, 1, &reply);
    }

    return;
}

void getConfig(afb_req_t request, unsigned argc, afb_data_t const argv[]) {

}

///////////////////////////////////////////////////////////////////////////////
//                             BINDING FUNCTIONS                             //
///////////////////////////////////////////////////////////////////////////////

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
        utils_add_verb(api
            , _afb_verbs_redpak[idx]
            , ADMIN_GROUP
            , NULL
            , NULL);
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

///////////////////////////////////////////////////////////////////////////////
//                             BINDING DEFINITION                            //
///////////////////////////////////////////////////////////////////////////////

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
            AFB_API_NOTICE(api, "Pre-initialization at path=%s", ctlarg->pre_init.path);
			if (_preinit_binding(api) < 0 ){
                AFB_API_ERROR(api, "Failled during pre-initialization");
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
                AFB_API_ERROR(api, "Failled during Initialization");
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