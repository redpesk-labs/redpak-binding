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

//////////////////////////////////////////////////////////////
//                   INCLUDE                                //
//////////////////////////////////////////////////////////////

#include <utils/utils.h>

// //////////////////////////////////////////////////////////////
// //                   PRIVATE FUNCTIONS                      //
// //////////////////////////////////////////////////////////////

// /**
//  * @brief Create a json_object into the user data to store data for info verb
//  * 
//  * @param userData pointer to the user data
//  */
// static void _set_info(userData_t *userData) {
//     json_object *metadataJ, *groupsJ;
//     wrap_json_pack(&metadataJ, "{s:s s:s s:s s:s}",
//         "uid", UID,
//         "info", BINDING_INFO,
//         "version", VERSION,
//         "author", AUTHOR
//     );
//     groupsJ = json_object_new_array();
//     userData->info = json_object_new_object();
//     json_object_object_add(userData->info, "metadata", metadataJ);
//     json_object_object_add(userData->info, "groups", groupsJ);
//     return;
// }

//////////////////////////////////////////////////////////////
//                   PUBLIC FUNCTIONS                       //
//////////////////////////////////////////////////////////////

void utils_add_verb(afb_api_t api, struct afb_verb_v4 afb_verb, char *group, char *usage, char *vcbdata) {
    json_object *groupsJ, *groupJ, *verbsJ, *verbJ, *uidJ;
    char *infoG;
    int ret = 0;
    bool ok = false;
    if (afb_api_add_verb(api, afb_verb.verb, afb_verb.info, afb_verb.callback, vcbdata, NULL, AFB_SESSION_NONE, 0)) {
        AFB_API_ERROR(api, "Initialization for verb %s failed", afb_verb.verb);
        return;
    }

    if(!group)
        return;

    ret = wrap_json_pack(&verbJ, "{s:s s:s s:s s:s*}",
        "uid", afb_verb.verb,
        "info", afb_verb.info,
        "verb", afb_verb.verb,
        "usage", usage);
    if(ret < 0) {
        AFB_API_ERROR(api, "[%s]Failed to pack verb %s info", __func__, afb_verb.verb);
        return;
    }
    if(!json_object_object_get_ex(binding_data.info_json, "groups", &groupsJ)) {
        AFB_API_ERROR(api, "[%s] no key \'groups\' in data %s ", __func__, json_object_get_string(binding_data.info_json));
        return;
    }
    for(int idx = 0; idx < json_object_array_length(groupsJ); idx ++) {
        groupJ = json_object_array_get_idx(groupsJ, idx);
        json_object_object_get_ex(groupJ, "uid", &uidJ);
        if(!strcmp(group, json_object_get_string(uidJ))) {
            if(!json_object_object_get_ex(groupJ, "verbs", &verbsJ)) {
                AFB_API_ERROR(api, "[%s] no key \'verbs\' in data %s ", __func__, json_object_get_string(groupJ));
                return;
            }
            json_object_array_add(verbsJ, verbJ);
            ok = true;
            break;
        }
    }
    if(!ok) {
        infoG = (char *)malloc((strlen(group) + strlen(GROUP_INFO) + 2)*sizeof(char));
        sprintf(infoG, "%s %s", group, GROUP_INFO);
        verbsJ = json_object_new_array();
        json_object_array_add(verbsJ, verbJ);
        ret = wrap_json_pack(&groupJ, "{s:s s:s s:o}",
            "uid", group,
            "info", infoG,
            "verbs", verbsJ);
        free(infoG);
        if(ret < 0) {
            AFB_API_ERROR(api, "[%s] Failed to pack group %s in object json", __func__, group);
            return;
        }
        json_object_array_add(groupsJ, groupJ);
    }
    return;
}

// //////////////////////////////////////////////////////////////
// //                   UTIL VERBS                             //
// //////////////////////////////////////////////////////////////

// json_object *utils_info(afb_req_t request)
// {
//     userData_t *userData;
//     userData = _get_userdata(request);
//     //json_object_get(userData->info);
//     return userData->info;
// }

// int utils_ping(afb_req_t request) {
//     userData_t *userData;
//     userData = _get_userdata(request);
//     userData->count++;
//     return userData->count;
// }
