
static const char _afb_description_redpak[] =
	"{\"openapi\":\"4.0.0\",\"info\":{\"description\":\"Service which man"
	"age redpak.\",\"title\":\"redpak-binding\",\"version\":\"1.0\",\"x-b"
	"inding-c-generator\":{\"api\":\"redpak\",\"version\":4,\"prefix\":\""
	"\",\"postfix\":\"\",\"start\":null,\"onevent\":null,\"mainctl\":\"bi"
	"nding_ctl\",\"scope\":\"\",\"private\":false}},\"servers\":[{\"url\""
	":\"ws://{host}:{port}/api/monitor\",\"description\":\"Service which "
	"manage redpak.\",\"variables\":{\"host\":{\"default\":\"localhost\"}"
	",\"port\":{\"default\":\"1234\"}},\"x-afb-events\":[{\"$ref\":\"#/co"
	"mponents/schemas/afb-event\"}]}],\"components\":{\"schemas\":{\"afb-"
	"reply\":{\"$ref\":\"#/components/schemas/afb-reply-v2\"},\"afb-event"
	"\":{\"$ref\":\"#/components/schemas/afb-event-v2\"},\"afb-reply-v2\""
	":{\"title\":\"Generic response.\",\"type\":\"object\",\"required\":["
	"\"jtype\",\"request\"],\"properties\":{\"jtype\":{\"type\":\"string\""
	",\"const\":\"afb-reply\"},\"request\":{\"type\":\"object\",\"require"
	"d\":[\"status\"],\"properties\":{\"status\":{\"type\":\"string\"},\""
	"info\":{\"type\":\"string\"},\"token\":{\"type\":\"string\"},\"uuid\""
	":{\"type\":\"string\"},\"reqid\":{\"type\":\"string\"}}},\"response\""
	":{\"type\":\"object\"}}},\"afb-event-v2\":{\"type\":\"object\",\"req"
	"uired\":[\"jtype\",\"event\"],\"properties\":{\"jtype\":{\"type\":\""
	"string\",\"const\":\"afb-event\"},\"event\":{\"type\":\"string\"},\""
	"data\":{\"type\":\"object\"}}}},\"x-permissions\":{\"getConfig\":{\""
	"permission\":\"urn:AGL:permission::platform:redpak:getConfig\"}},\"r"
	"esponses\":{\"200\":{\"description\":\"A complex object array respon"
	"se\",\"content\":{\"application/json\":{\"schema\":{\"$ref\":\"#/com"
	"ponents/schemas/afb-reply\"}}}}}},\"paths\":{\"/ping\":{\"descriptio"
	"n\":\"Check if binding running well by ping-pong request\",\"respons"
	"es\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}},\"/info\":"
	"{\"description\":\"Get current redpak-binding configuration\",\"resp"
	"onses\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}},\"/getC"
	"onfig\":{\"description\":\"Get the config of a Node\",\"get\":{\"x-p"
	"ermissions\":{\"$ref\":\"#/components/x-permissions/getConfig\"},\"r"
	"esponses\":{\"200\":{\"$ref\":\"#/components/responses/200\"}}},\"pa"
	"rameters\":[{\"in\":\"query\",\"name\":\"path\",\"required\":true,\""
	"schema\":{\"type\":\"string\"}}]}}}"
;

static const struct afb_auth _afb_auths_redpak[] = {
	{ .type = afb_auth_Permission, .text = "urn:AGL:permission::platform:redpak:getConfig" }
};

void ping(afb_req_t request, unsigned argc, afb_data_t const argv[]);
void info(afb_req_t request, unsigned argc, afb_data_t const argv[]);
void getConfig(afb_req_t request, unsigned argc, afb_data_t const argv[]);

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
        .verb = "getConfig",
        .callback = getConfig,
        .auth = &_afb_auths_redpak[0],
        .info = "Get the config of a Node",
        .vcbdata = NULL,
        .session = AFB_SESSION_NONE,
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

int binding_ctl(afb_api_t api, afb_ctlid_t ctlid, afb_ctlarg_t ctlarg, void *userdata);

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

