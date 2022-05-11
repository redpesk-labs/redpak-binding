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
//                             DEFINE                                       //
//////////////////////////////////////////////////////////////////////////////

#define REDMICRODNF_CMD "redmicrodnf"

//////////////////////////////////////////////////////////////////////////////
//                             INCLUDE                                      //
//////////////////////////////////////////////////////////////////////////////

#include <utils.h>

//////////////////////////////////////////////////////////////////////////////
//                             PRIVATE FUNCTIONS                            //
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief Check the validity of a path, if has '/' and dont finish by '/'
 * 
 * @param[in] path Path to check
 * @return 0 in success negative otherwise 
 */
static int _utils_check_path(const char *path) {
    if (!path)
        return -1;
    for (int index = 1; index < strlen(path); index ++) {
        if(path[index] == '/') {
            break;
        }
        if (index == strlen(path) -1 )
            return -1;
    }
    if (path[strlen(path) -1] == '/')
        return -1;
    return 0;
}

/**
 * @brief get the pointer to the string where filename/nodename begin
 * 
 * @param[in] red_path path to a file
 * @return pointer to the path
 */
static const char * _utils_get_last_name(const char *red_path){
    char separator = '/';
    int last_sep = 0;
    for (int index = 0; index < strlen(red_path); index ++) {
        if (red_path[index] == separator) {
            last_sep = index;
        }
    }
    return &red_path[last_sep + 1];
}

/**
 * @brief Get the directory path and the filename from a given file path
 * 
 * @param[in]   path        Path to the file
 * @param[out]  dir_path    directory path parsed
 * @param[out]  file_name   File name parsed
 * @return 0 in success negative otherwise
 */
static int _utils_break_path(const char *path, char **dir_path, char ** file_name) {
    int ret = 0;

    ret = _utils_check_path(path);
    if (ret < 0)
        return ret;
    
    char separator = '/';
    int last_sep = 0;
    for (int index = 0; index < strlen(path); index ++) {
        if (path[index] == separator) {
            last_sep = index;
        }
    }
    *dir_path = (char *) calloc(last_sep + 2, sizeof(char));
    *file_name = (char *) calloc(strlen(path) - last_sep + 1, sizeof(char));
    if (snprintf(*dir_path, last_sep + 1, "%s", path) < 0) {
        free(dir_path);
        free(file_name);
        return -1;
    }
    if (snprintf(*file_name, strlen(path) - last_sep, "%s", &path[last_sep + 1]) < 0) {
        free(dir_path);
        free(file_name);
        return -1;
    }
    return 0;
}

/**
 * @brief Check if a file exists
 * 
 * @param[in] path Path to the file to check
 * @return 0 if found negative with error code if not
 */
static int _utils_file_exists(const char *path) {
    int ret = 0;
    char *dir_path = NULL;
    char *file_name = NULL;
    ret = _utils_break_path(path, &dir_path, &file_name);
    if (ret < 0)
        return ret;
    
    DIR *directory = opendir(dir_path);
    if (!directory) {
        free(dir_path);
        free(file_name);
        return -ERROR_UTILS_NO_DIR;
    }
    struct dirent *dir_entry;
    bool found = false;
    while((dir_entry = readdir(directory))) {
        if (dir_entry->d_type != DT_DIR && strcmp(dir_entry->d_name, file_name) == 0) {
            found = true;
            break;
        }
    }
    free(dir_path);
    free(file_name);
    closedir(directory);
    if (!found) return -1;

    return 0;
}

/**
 * @brief Copy the contains of a file to another - please check before if source file exists
 * 
 * @param[in] source_path Path to the source file
 * @param[in] target_path Path to the Target file
 * @return 0 in success negative otherwise 
 */
static int _utils_copy_file(const char *source_path, const char *target_path) {
    char character;
    FILE *source, *target;
    
    source = fopen(source_path, "r");
    if (!source)
        return -ERROR_UTILS_OPEN_SOURCE;

    target = fopen(target_path, "w");
    if (!target) {
        fclose(source);
        return -ERROR_UTILS_OPEN_TARGET;
    }
    while((character = fgetc(source)) != EOF ) {
        fputc(character, target);
    }
    
    fclose(source);
    fclose(target);
    return 0;
}

/**
 * @brief Remove recursively all folder/file in a path (path included)
 * 
 * @param[in] path Path to the folder to remove
 * @return 0 in success negative otherwise 
 */
static int _utils_remove_folder(const char *path) {
    int ret = 0;
    char *full_path;
    size_t path_len;
    DIR *directory;
    struct stat stat_path, stat_entry;
    struct dirent *dir_entry;

    // stat for the path
    stat(path, &stat_path);
    // if path does not exists or is not dir
    if (S_ISDIR(stat_path.st_mode) == 0) {
        return -ERROR_UTILS_NO_DIR;
    }
    // if not possible to read the directory for this user
    if ( (directory = opendir(path)) == NULL) {
        return -ERROR_UTILS_FORBIDDEN;
    }
    // the length of the path
    path_len = strlen(path);
  
    while((dir_entry = readdir(directory))) {
        // skip entries "." and ".."
        if (!strcmp(dir_entry->d_name, ".") || !strcmp(dir_entry->d_name, ".."))
            continue;
        
        full_path = calloc(path_len + strlen(dir_entry->d_name) + 1 + 1, sizeof(char));
        strcpy(full_path, path);
        strcat(full_path, "/");
        strcat(full_path, dir_entry->d_name);

        // stat for the entry
        stat(full_path, &stat_entry);

        // recursively remove a nested directory
        if (S_ISDIR(stat_entry.st_mode) != 0) {
            _utils_remove_folder(full_path);
            free(full_path);
            continue;
        }
        // remove a file object
        if (unlink(full_path) != 0) {
            ret = -ERROR_UTILS_REMOVE;
            AFB_WARNING("Can't remove a file %s", full_path);
        } else {
            AFB_DEBUG("%s has been removed", full_path);
        }

        free(full_path);
    }
    rmdir(path);
    closedir(directory);
    return ret;
}

/**
 * @brief Create recursively the node tree with this children
 * 
 * @param[out]  tree_node_json  json containing the node tree 
 * @param[in]   node            the node where to start to scan
 * @param[in]   depth           Depth of the tree
 * @return 0 in success, negative otherwise
 */
int _utils_dump_tree(json_object *tree_node_json, redNodeT *node, int depth) {
    int ret = 0;

    if (!node || !node->redpath) {
        return -ERROR_UTILS_DOWN_SCAN;
    }

    ret = json_object_object_add(tree_node_json, "node", json_object_new_string(basename( (char*) node->redpath)));
    if (ret < 0) return ret;
    ret = json_object_object_add(tree_node_json, "redpath", json_object_new_string(node->redpath));
    if (ret < 0) return ret;

    redChildNodeT *children = node->childs;
    json_object *children_json = json_object_new_array();
    if (depth > 0 && children && children->child) {
        // Add the first child
        json_object *child_json = json_object_new_object();
        ret = _utils_dump_tree(child_json, children->child, depth-1);
        if (ret < 0) return ret;
        ret = json_object_array_add(children_json, child_json);
        if (ret < 0) return ret;

        // Add potential brother's child
        for (redChildNodeT *brother = children->brother; brother && brother->child; brother = brother->brother) {
            json_object *brother_child_json = json_object_new_object();
            ret = _utils_dump_tree(brother_child_json, brother->child, depth-1);
            if (ret < 0) return ret;
            ret = json_object_array_add(children_json, brother_child_json);
            if (ret < 0) return ret;
        }
    }
    ret = json_object_object_add(tree_node_json, "children", children_json);
    return ret;
}

//////////////////////////////////////////////////////////////////////////////
//                             PUBLIC FUNCTIONS                             //
//////////////////////////////////////////////////////////////////////////////

const char *utils_parse_error(utils_error_t no_error) {
    switch (no_error) {
    case -ERROR_UTILS_MALFORMATED_PATH:
        return "Redpath is malformated";
    case -ERROR_UTILS_PARSED_REDWRAP:
        return "Some arguments are malformated for Redwrap";
    case -ERROR_UTILS_NO_DIR:
        return "No directory found";
    case -ERROR_UTILS_NO_FILE:
        return "No file found";
    case -ERROR_UTILS_OPEN_SOURCE:
        return "Failed to open source file for duplicate";
    case -ERROR_UTILS_OPEN_TARGET:
        return "Failed to open target file for duplicate";
    case -ERROR_UTILS_FORBIDDEN:
        return "Forbidden action";
    case -ERROR_UTILS_REMOVE:
        return "Failed to remove";
    case -ERROR_UTILS_DOWN_SCAN:
        return "Failed to scan all node tree";
    default:
    case -ERROR_UTILS:
        return "Internal error";
    }
};

void utils_add_verb(afb_api_t api, struct afb_verb_v4 afb_verb, char *group, char *usage, char *vcbdata) {
    json_object *groupsJ, *groupJ, *verbsJ, *verbJ, *uidJ;
    char *infoG;
    int ret = 0;
    bool ok = false;
    bool custom_group = true;
    if (afb_api_add_verb(api, afb_verb.verb, afb_verb.info, afb_verb.callback, vcbdata, NULL, AFB_SESSION_NONE, 0)) {
        AFB_API_ERROR(api, "Initialization for verb %s failed", afb_verb.verb);
        return;
    }

    if(!group) {
        custom_group = false;
        if (afb_verb.session == AFB_SESSION_LOA_1) {
            if (asprintf(&group, GROUP_USER) < 0) {
                AFB_API_ERROR(api, "Failed on asprintf for set group");
                return;
            }
        } else {
            if (asprintf(&group, GROUP_ADMIN) < 0) {
                AFB_API_ERROR(api, "Failed on asprintf for set group");
                return;
            }
        }
    }

    ret = wrap_json_pack(&verbJ, "{s:s s:s s:s s:s*}",
        "uid", afb_verb.verb,
        "info", afb_verb.info,
        "verb", afb_verb.verb,
        "usage", usage);
    if(ret < 0) {
        AFB_API_ERROR(api, "[%s]Failed to pack verb %s info", __func__, afb_verb.verb);
        if (!custom_group) free(group);
        return;
    }
    if(!json_object_object_get_ex(binding_data.info_json, "groups", &groupsJ)) {
        AFB_API_ERROR(api, "[%s] no key \'groups\' in data %s ", __func__, json_object_get_string(binding_data.info_json));
        if (!custom_group) free(group);
        return;
    }
    for(int idx = 0; idx < json_object_array_length(groupsJ); idx ++) {
        groupJ = json_object_array_get_idx(groupsJ, idx);
        json_object_object_get_ex(groupJ, "uid", &uidJ);
        if(!strcmp(group, json_object_get_string(uidJ))) {
            if(!json_object_object_get_ex(groupJ, "verbs", &verbsJ)) {
                AFB_API_ERROR(api, "[%s] no key \'verbs\' in data %s ", __func__, json_object_get_string(groupJ));
                if (!custom_group) free(group);
                return;
            }
            json_object_array_add(verbsJ, verbJ);
            ok = true;
            break;
        }
    }
    if(!ok) {
        infoG = (char *)malloc((strlen(group) + strlen(INFO_GROUP) + 2)*sizeof(char));
        sprintf(infoG, "%s %s", group, INFO_GROUP);
        verbsJ = json_object_new_array();
        json_object_array_add(verbsJ, verbJ);
        ret = wrap_json_pack(&groupJ, "{s:s s:s s:o}",
            "uid", group,
            "info", infoG,
            "verbs", verbsJ);
        free(infoG);
        if(ret < 0) {
            AFB_API_ERROR(api, "[%s] Failed to pack group %s in object json", __func__, group);
            if (!custom_group) free(group);
            return;
        }
        json_object_array_add(groupsJ, groupJ);
    }
    if (!custom_group) free(group);
    return;
}

//////////////////////////////////////////////////////////////////////////////
//                             UTIL VERBS                                   //
//////////////////////////////////////////////////////////////////////////////

utils_error_t utils_get_tree(json_object *output_json, const char *red_path, int depth) {
    int ret = 0;
    redNodeT *main_node = RedNodesDownScan(red_path, 1);
    if (!main_node) {
        return -ERROR_UTILS_DOWN_SCAN;
    }
    //NOTE: mode depth=-1, means all tree
    if (depth == -1) {
        depth = 1000;
    }
    ret = _utils_dump_tree(output_json, main_node, depth);
    freeRedRoot(main_node);
    return ret;
}

utils_error_t utils_create_node(const char *red_path, const char *repo_path) {
    int ret = 0;
    char *red_path_arg = NULL;
    char *node_name_arg = NULL;
    
    // check if path is malformated
    if (_utils_check_path(red_path) < 0)
        return -ERROR_UTILS_MALFORMATED_PATH;
    
    // Create arguments for redwrap command
    if (asprintf(&red_path_arg, "--redpath=%s",red_path) < 0) {
        AFB_ERROR("[%s] asprintf error", __func__);
        return -ERROR_UTILS;
    }
    const char *node_name = _utils_get_last_name(red_path);
    if (asprintf(&node_name_arg, "--alias=%s",node_name) < 0) {
        AFB_ERROR("[%s] asprintf error", __func__);
        return -ERROR_UTILS;
    }

    // Call the redwrap command
    char *redwrap_args[4] = {"redwrap-dnf", red_path_arg, "manager", node_name_arg};
    ret = redwrap_dnf_cmd_exec(4, redwrap_args);
    AFB_DEBUG("redwrap cmd: %s %s %s %s %s", redwrap_args[0], redwrap_args[1], redwrap_args[2], redwrap_args[3], redwrap_args[4]);
    free(red_path_arg);
    free(node_name_arg);

    // Setup redpak.repo
    if (repo_path) {
        if (_utils_file_exists(repo_path) < 0)
            return -ERROR_UTILS_NO_FILE;

        // mkdir repo folder in node
        char *node_repo_path = NULL;
        struct stat st = {0};
        asprintf(&node_repo_path, "%s/etc/yum.repos.d", red_path);
        if (stat(node_repo_path, &st) == -1) {
            mkdir(node_repo_path, 0700);
        }

        // Copy the repo file to the node
        char *target_file_path = NULL;
        const char * repo_file_name = _utils_get_last_name(repo_path);
        asprintf(&target_file_path, "%s/%s", node_repo_path, repo_file_name);
        ret = _utils_copy_file(repo_path, target_file_path);

        free(target_file_path);
        free(node_repo_path);
    }

    return  ret;
}

utils_error_t utils_delete_node(const char *red_path) {
    int ret = 0;

    // verify to not delete the wrong folder
    if (strcasecmp(red_path, "/var/redpesk") == 0) {
        AFB_ERROR("It's look like we want to delete the root of all rednode !!!");
        return -ERROR_UTILS_FORBIDDEN;
    }

    // Verify if it's a node path by checking the presence of a .rednode.yaml file
    char *config_rednode = NULL;
    asprintf(&config_rednode, "%s/.rednode.yaml", red_path);
    ret = _utils_file_exists(config_rednode);
    free(config_rednode);
    if (ret < 0) {
        AFB_ERROR("It's look like we want to delete a no-node folder !!");
        return -ERROR_UTILS_FORBIDDEN;
    }

    // Remove recursively all folder/files in the  redpath of the node
    ret = _utils_remove_folder(red_path);

    return ret;
}

utils_error_t utils_manage_app(const char *red_path, const char *app_name, utils_action_app_t action) {
    int ret = 0;
    char *red_path_arg = NULL;
    
    // check if path is malformated
    if (_utils_check_path(red_path) < 0)
        return -ERROR_UTILS_MALFORMATED_PATH;
    
    // Verify if it's a node path by checking the presence of a .rednode.yaml file
    char *config_rednode = NULL;
    asprintf(&config_rednode, "%s/.rednode.yaml", red_path);
    ret = _utils_file_exists(config_rednode);
    free(config_rednode);
    if (ret < 0) {
        AFB_ERROR("It's look like we want to install an app not in a Node (%s) !!", red_path);
        return -ERROR_UTILS_WRONG_PATH;
    }

    // Create arguments for redwrap command
    if (asprintf(&red_path_arg, "--redpath=%s",red_path) < 0) {
        AFB_ERROR("[%s] asprintf error", __func__);
        return -ERROR_UTILS;
    }
    char *action_arg = NULL;
    switch (action) {
    case APP_ACTION_INSTALL:
        asprintf(&action_arg, "install");
        break;
    case APP_ACTION_UPDATE:
        asprintf(&action_arg, "update");
        break;
    case APP_ACTION_REMOVE:
        asprintf(&action_arg, "remove");
        break;
    }

    // Call the redwrap command
    char *redwrap_args[9] = {"redwrap", red_path_arg, "--force", "--admin", "--", "redmicrodnf", red_path_arg, action_arg, (char *)app_name};
    ret = redwrap_cmd_exec(9, redwrap_args);
    free(action_arg);
    free(red_path_arg);

    return ret;
}