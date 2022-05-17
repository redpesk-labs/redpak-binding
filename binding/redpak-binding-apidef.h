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

/**
 * @brief Binding specification
 * 
 */
static const char _afb_description_redpak[] =
	"{\"openapi\":\"4.0.0\",\"info\":{\"description\":\"Service which man"
	"age redpak.\",\"title\":\"redpak-binding\",\"version\":\"1.0.0-3e070"
	"2f-dirty\",\"x-binding-c-generator\":{\"api\":\"redpak\",\"version\""
	":4,\"prefix\":\"\",\"postfix\":\"\",\"start\":null,\"onevent\":null,"
	"\"mainctl\":\"binding_ctl\",\"scope\":\"\",\"private\":false}},\"ser"
	"vers\":[{\"url\":\"ws://{host}:{port}/api/monitor\",\"description\":"
	"\"Service which manage redpak.\",\"variables\":{\"host\":{\"default\""
	":\"localhost\"},\"port\":{\"default\":\"1234\"}},\"x-afb-events\":[{"
	"\"$ref\":\"#/components/schemas/afb-event\"}]}],\"components\":{\"sc"
	"hemas\":{\"afb-reply\":{\"$ref\":\"#/components/schemas/afb-reply-v2"
	"\"},\"afb-event\":{\"$ref\":\"#/components/schemas/afb-event-v2\"},\""
	"afb-reply-v2\":{\"title\":\"Generic response.\",\"type\":\"object\","
	"\"required\":[\"jtype\",\"request\"],\"properties\":{\"jtype\":{\"ty"
	"pe\":\"string\",\"const\":\"afb-reply\"},\"request\":{\"type\":\"obj"
	"ect\",\"required\":[\"status\"],\"properties\":{\"status\":{\"type\""
	":\"string\"},\"info\":{\"type\":\"string\"},\"token\":{\"type\":\"st"
	"ring\"},\"uuid\":{\"type\":\"string\"},\"reqid\":{\"type\":\"string\""
	"}}},\"response\":{\"type\":\"object\"}}},\"afb-event-v2\":{\"type\":"
	"\"object\",\"required\":[\"jtype\",\"event\"],\"properties\":{\"jtyp"
	"e\":{\"type\":\"string\",\"const\":\"afb-event\"},\"event\":{\"type\""
	":\"string\"},\"data\":{\"type\":\"object\"}}}},\"x-permissions\":{\""
	"gettree\":{\"permission\":\"urn:AGL:permission::platform:redpak:getT"
	"ree\"},\"getRoot\":{\"permission\":\"urn:AGL:permission::platform:re"
	"dpak:getRoot\"},\"getconfig\":{\"permission\":\"urn:AGL:permission::"
	"platform:redpak:getconfig\"},\"createNodeRpm\":{\"permission\":\"urn"
	":AGL:permission::platform:redpak:createNodeRpm\"},\"deleteNodeRpm\":"
	"{\"permission\":\"urn:AGL:permission::platform:redpak:deleteNodeRpm\""
	"},\"user\":{\"LOA\":1}},\"responses\":{\"200\":{\"description\":\"A "
	"complex object array response\",\"content\":{\"application/json\":{\""
	"schema\":{\"$ref\":\"#/components/schemas/afb-reply\"}}}}}},\"paths\""
	":{\"/ping\":{\"description\":\"Check if binding running well by ping"
	"-pong request\",\"responses\":{\"200\":{\"$ref\":\"#/components/resp"
	"onses/200\"}}},\"/info\":{\"description\":\"Get current redpak-bindi"
	"ng configuration\",\"responses\":{\"200\":{\"$ref\":\"#/components/r"
	"esponses/200\"}}},\"/getRoot\":{\"description\":\"Get the redpath ro"
	"ot for all Node\",\"get\":{\"x-permissions\":{\"$ref\":\"#/component"
	"s/x-permissions/getRoot\"},\"responses\":{\"200\":{\"$ref\":\"#/comp"
	"onents/responses/200\"}}}},\"/gettree\":{\"description\":\"Get the t"
	"ree with the node and this x children according the depth\",\"get\":{\""
	"x-permissions\":{\"$ref\":\"#/components/x-permissions/gettree\"},\""
	"responses\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}},\"p"
	"arameters\":[{\"in\":\"query\",\"name\":\"redpath\",\"required\":tru"
	"e,\"schema\":{\"type\":\"string\"}},{\"in\":\"query\",\"name\":\"dep"
	"th\",\"required\":true,\"schema\":{\"type\":\"int\"}}]},\"/getconfig"
	"\":{\"description\":\"Get the config of a Node\",\"get\":{\"x-permis"
	"sions\":{\"$ref\":\"#/components/x-permissions/getconfig\"},\"respon"
	"ses\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}},\"paramet"
	"ers\":[{\"in\":\"query\",\"name\":\"path\",\"required\":true,\"schem"
	"a\":{\"type\":\"string\"}}]},\"/createNodeRpm\":{\"description\":\"C"
	"reate a rednode by installing an rpm\",\"get\":{\"x-permissions\":{\""
	"$ref\":\"#/components/x-permissions/createNodeRpm\"},\"responses\":{"
	"\"200\":{\"$ref\":\"#/components/responses/200\"}}},\"parameters\":["
	"{\"in\":\"query\",\"name\":\"rpmurl\",\"required\":true,\"schema\":{"
	"\"type\":\"string\"}}]},\"/deleteNodeRpm\":{\"description\":\"Delete"
	" rpm installed rednode by its package name\",\"get\":{\"x-permission"
	"s\":{\"$ref\":\"#/components/x-permissions/deleteNodeRpm\"},\"respon"
	"ses\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}},\"paramet"
	"ers\":[{\"in\":\"query\",\"name\":\"pkgname\",\"required\":true,\"sc"
	"hema\":{\"type\":\"string\"}}]},\"/createNode\":{\"description\":\"C"
	"reate a rednode by its redpath\",\"get\":{\"x-permissions\":{\"$ref\""
	":\"#/components/x-permissions/user\"},\"responses\":{\"200\":{\"$ref"
	"\":\"#/components/responses/200\"}}},\"parameters\":[{\"in\":\"query"
	"\",\"name\":\"redpath\",\"required\":true,\"schema\":{\"type\":\"str"
	"ing\"}},{\"in\":\"query\",\"name\":\"repoPath\",\"required\":true,\""
	"schema\":{\"type\":\"string\"}}]},\"/deleteNode\":{\"description\":\""
	"Delete a rednode by its redpath\",\"get\":{\"x-permissions\":{\"$ref"
	"\":\"#/components/x-permissions/user\"},\"responses\":{\"200\":{\"$r"
	"ef\":\"#/components/responses/200\"}}},\"parameters\":[{\"in\":\"que"
	"ry\",\"name\":\"redpath\",\"required\":true,\"schema\":{\"type\":\"s"
	"tring\"}}]},\"/installApp\":{\"description\":\"Install an app by app"
	" name in a node by redpath\",\"get\":{\"x-permissions\":{\"$ref\":\""
	"#/components/x-permissions/user\"},\"responses\":{\"200\":{\"$ref\":"
	"\"#/components/responses/200\"}}},\"parameters\":[{\"in\":\"query\","
	"\"name\":\"appname\",\"required\":true,\"schema\":{\"type\":\"string"
	"\"}},{\"in\":\"query\",\"name\":\"redpath\",\"required\":true,\"sche"
	"ma\":{\"type\":\"string\"}}]},\"/updateApp\":{\"description\":\"Upda"
	"te an app by app name in a node by redpath\",\"get\":{\"x-permission"
	"s\":{\"$ref\":\"#/components/x-permissions/user\"},\"responses\":{\""
	"200\":{\"$ref\":\"#/components/responses/200\"}}},\"parameters\":[{\""
	"in\":\"query\",\"name\":\"appname\",\"required\":true,\"schema\":{\""
	"type\":\"string\"}},{\"in\":\"query\",\"name\":\"redpath\",\"require"
	"d\":true,\"schema\":{\"type\":\"string\"}}]},\"/removeApp\":{\"descr"
	"iption\":\"Remove an app by app name in a node by redpath\",\"get\":"
	"{\"x-permissions\":{\"$ref\":\"#/components/x-permissions/user\"},\""
	"responses\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}},\"p"
	"arameters\":[{\"in\":\"query\",\"name\":\"appname\",\"required\":tru"
	"e,\"schema\":{\"type\":\"string\"}},{\"in\":\"query\",\"name\":\"red"
	"path\",\"required\":true,\"schema\":{\"type\":\"string\"}}]}}}"
;

/**
 * @brief Array of different verb authentications
 * 
 */
static const struct afb_auth _afb_auths_redpak[] = {
    { .type = afb_auth_Permission, .text = "urn:AGL:permission::platform:redpak:getRoot" },
	{ .type = afb_auth_Permission, .text = "urn:AGL:permission::platform:redpak:gettree" },
	{ .type = afb_auth_Permission, .text = "urn:AGL:permission::platform:redpak:getconfig" },
	{ .type = afb_auth_Permission, .text = "urn:AGL:permission::platform:redpak:createNodeRpm" },
	{ .type = afb_auth_Permission, .text = "urn:AGL:permission::platform:redpak:deleteNodeRpm" }
};

/**
 * @brief Check if binding running well by ping-pong request
 * 
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
void ping(afb_req_t request, unsigned argc, afb_data_t const argv[]);

/**
 * @brief Get the redpath root for all Node
 * 
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
void getRoot(afb_req_t request, unsigned argc, afb_data_t const argv[]);

/**
 * @brief Get the tree of a node and this children according to the depth
 * 
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
void gettree(afb_req_t request, unsigned argc, afb_data_t const argv[]);

/**
 * @brief Get current redpak-binding configuration
 * 
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
void info(afb_req_t request, unsigned argc, afb_data_t const argv[]);

/**
 * @brief Get the config of a Node
 * 
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
void getconfig(afb_req_t request, unsigned argc, afb_data_t const argv[]);

/**
 * @brief Create a rednode by installing an rpm
 * 
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
void createNodeRpm(afb_req_t request, unsigned argc, afb_data_t const argv[]);

/**
 * @brief Delete rpm installed rednode by its package name
 * 
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
void deleteNodeRpm(afb_req_t request, unsigned argc, afb_data_t const argv[]);

/**
 * @brief Create a rednode by its redpath
 * 
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
void createNode(afb_req_t request, unsigned argc, afb_data_t const argv[]);

/**
 * @brief Delete a rednode by its redpath
 * 
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
void deleteNode(afb_req_t request, unsigned argc, afb_data_t const argv[]);

/**
 * @brief Install an app by app name in a node by redpath
 * 
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
void installApp(afb_req_t request, unsigned argc, afb_data_t const argv[]);

/**
 * @brief Update an app by app name in a node by redpath
 * 
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
void updateApp(afb_req_t request, unsigned argc, afb_data_t const argv[]);

/**
 * @brief Remove an app by app name in a node by redpath
 * 
 * @param request   Client request
 * @param argc      Arguments count
 * @param argv      array of arguments
 */
void removeApp(afb_req_t request, unsigned argc, afb_data_t const argv[]);

/**
 * @brief List of api's verbs
 * 
 */
static const struct afb_verb_v4 _afb_verbs_redpak[] = {
    {
        .verb = "ping",
        .callback = ping,
        .auth = NULL,
        .info = "Check if binding running well by ping-pong request",
        .vcbdata = NULL,
        .session = AFB_SESSION_NONE,
        .glob = 0
    },
    {
        .verb = "info",
        .callback = info,
        .auth = NULL,
        .info = "Get current redpak-binding configuration",
        .vcbdata = NULL,
        .session = AFB_SESSION_NONE,
        .glob = 0
    },
    {
        .verb = "getRoot",
        .callback = getRoot,
        .auth = &_afb_auths_redpak[0],
        .info = "Get the redpath root for all Node",
        .vcbdata = NULL,
        .session = AFB_SESSION_NONE,
        .glob = 0
    },
    {
        .verb = "gettree",
        .callback = gettree,
        .auth = &_afb_auths_redpak[1],
        .info = "Get the tree of a node and this children according to the depth",
        .vcbdata = NULL,
        .session = AFB_SESSION_NONE,
        .glob = 0
    },
    {
        .verb = "getconfig",
        .callback = getconfig,
        .auth = &_afb_auths_redpak[2],
        .info = "Get the config of a Node",
        .vcbdata = NULL,
        .session = AFB_SESSION_NONE,
        .glob = 0
    },
    {
        .verb = "node/createRpm",
        .callback = createNodeRpm,
        .auth = &_afb_auths_redpak[3],
        .info = "Create a rednode by installing an rpm",
        .vcbdata = NULL,
        .session = AFB_SESSION_NONE,
        .glob = 0
    },
    {
        .verb = "node/deleteRpm",
        .callback = deleteNodeRpm,
        .auth = &_afb_auths_redpak[4],
        .info = "Delete rpm installed rednode by its package name",
        .vcbdata = NULL,
        .session = AFB_SESSION_NONE,
        .glob = 0
    },
    {
        .verb = "node/create",
        .callback = createNode,
        .auth = NULL,
        .info = "Create a rednode by its redpath",
        .vcbdata = NULL,
        .session = AFB_SESSION_LOA_1,
        .glob = 0
    },
    {
        .verb = "node/delete",
        .callback = deleteNode,
        .auth = NULL,
        .info = "Delete a rednode by its redpath",
        .vcbdata = NULL,
        .session = AFB_SESSION_LOA_1,
        .glob = 0
    },
    {
        .verb = "app/install",
        .callback = installApp,
        .auth = NULL,
        .info = "Install an app by app name in a node by redpath",
        .vcbdata = NULL,
        .session = AFB_SESSION_LOA_1,
        .glob = 0
    },
    {
        .verb = "app/update",
        .callback = updateApp,
        .auth = NULL,
        .info = "Update an app by app name in a node by redpath",
        .vcbdata = NULL,
        .session = AFB_SESSION_LOA_1,
        .glob = 0
    },
    {
        .verb = "app/remove",
        .callback = removeApp,
        .auth = NULL,
        .info = "Remove an app by app name in a node by redpath",
        .vcbdata = NULL,
        .session = AFB_SESSION_LOA_1,
        .glob = 0
    },
    {
        .verb = NULL,
        .callback = NULL,
        .auth = NULL,
        .info = NULL,
        .vcbdata = NULL,
        .session = 0,
        .glob = 0
    }
};

/**
 * @brief Binding Callback
 * 
 * @param api       the api that receive the callback
 * @param ctlid     identifier of the reason of the call (@see afb_ctlid)
 * @param ctlarg    data associated to the call
 * @param userdata  the userdata of the api (@see afb_api_get_userdata)
 */
int binding_ctl(afb_api_t api, afb_ctlid_t ctlid, afb_ctlarg_t ctlarg, void *userdata);

/**
 * @brief Strictire of the binding
 * 
 */
const struct afb_binding_v4 afbBindingV4 = {
    .api = "redpak",
    .specification = _afb_description_redpak,
    .info = "Service which manage redpak.",
    .verbs = _afb_verbs_redpak,
    .mainctl = binding_ctl,
    .userdata = NULL,
    .provide_class = NULL,
    .require_class = NULL,
    .require_api = NULL,
    .noconcurrency = 0
};

