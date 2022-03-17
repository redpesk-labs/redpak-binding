# Redpak-binding usage

## Used as redpesk RPM package

When used as a redpesk RPM package, the service will not autostart and need to be start manually with the framework `afm-util`.

```bash
afmutil start redpak-binding
```

## Built from source

### afb-binder

```bash
afb-binder --port=9999 --name=redpak --workdir=/home/devel/gitsources/redpesk_OS_Common_Components/redpak-binding/build/package --binding=/home/devel/gitsources/redpesk_OS_Common_Components/redpak-binding/build/package/lib/afb-redpak-binding.so  --tracereq=common --ws-server=unix:@redpak -vvv
```

### afb-client

```bash
$ afb-client --human 'localhost:9999/api'
redpak ${VERB}
```

### binding info

```bash
$ afb-client --human 'localhost:9999/api'
redpak info
ON-REPLY 1:redpak/info: OK
{
  "jtype":"afb-reply",
  "request":{
    "status":"success",
    "code":0
  },
  "response":{
    "metadata":{
      "uid":"redpak-binding",
      "info":"Service which manage redpak.",
      "version":"1.0.0",
      "author":"Valentin Lefebvre"
    },
    "groups":[
      {
        "uid":"ADMIN",
        "info":"ADMIN related verbs",
        "verbs":[
          {
            "uid":"ping",
            "info":"Check if binding running well by ping-pong request",
            "verb":"ping"
          },
          {
            "uid":"info",
            "info":"Get current redpak-binding configuration",
            "verb":"info"
          },
          {
            "uid":"getConfig",
            "info":"Get the config of a Node",
            "verb":"getConfig"
          },
          {
            "uid":"createNodeRpm",
            "info":"Create a rednode by installing an rpm",
            "verb":"createNodeRpm"
          },
          {
            "uid":"deleteNodeRpm",
            "info":"Delete rpm installed rednode by its package name",
            "verb":"deleteNodeRpm"
          }
        ]
      },
      {
        "uid":"USER",
        "info":"USER related verbs",
        "verbs":[
          {
            "uid":"createNode",
            "info":"Create a rednode by its redpath",
            "verb":"createNode"
          },
          {
            "uid":"deleteNode",
            "info":"Delete a rednode by its redpath",
            "verb":"deleteNode"
          },
          {
            "uid":"installApp",
            "info":"Install an app by app name in a node by redpath",
            "verb":"installApp"
          },
          {
            "uid":"updateNode",
            "info":"Update an app by app name in a node by redpath",
            "verb":"updateNode"
          },
          {
            "uid":"removeApp",
            "info":"Remove an app by app name in a node by redpath",
            "verb":"removeApp"
          }
        ]
      }
    ]
  }
}

```

## api's verb

| verb | definition | arguments | example | output |
|:----:|------------|-----------|---------|--------|
| createNode | Create a rednode by its redpath. It will use lib redwrap to create a node and copy a repo file into it. | {"redPath": "/path/to/node", "repoPath": "/path/to/redpesk.repo"} | redpak createNode {"redPath":"/var/redpesk/toto", "repoPath": "/etc/yum.repos.d/redpesk-sdk.repo"} | "response":"Rednode /var/redpesk/toto created with success" |
| deleteNode | Delete a rednode by its redpath. | "/path/to/node" | redpak deleteNode /var/redpesk/tata | "response":"Rednode /var/redpesk/tata has been deleted with success" |
| getConfig | Get the config of a Node. | "/path/to/node" | redpak getConfig /var/redpesk/test | "response":"headers:\n  alias: test\n  name: test\n  info: Node created by devel(redpak-devel) the 16-Mar-2022 Mar:42 (UTC)\nexports:\n- mode: Private\n  mount: /nodes/_private\n  path: $NODE_PATH/private\n- mode: Private\n  mount: /var/lib/rpm\n  path: $NODE_PATH/var/lib/rpm\n- mode: Restricted\n  mount: /nodes/test/usr\n  path: $NODE_PATH/usr\n- mode: Public\n  mount: /nodes/test/var\n  path: $NODE_PATH/var\nenviron:\n- mode: Default\n  key: XDG_RUNTIME_DIR\n  value: /run/user/$UID\n- mode: Static\n  key: AFB_LDPATHS\n  value: /nodes/agl-core/usr/lib64/afb\nconfig:\n  persistdir: /var/redpesk/test/var/lib/dnf\n  rpmdir: /var/redpesk/test/var/lib/rpm\n  path: /nodes/test/usr/bin\n  ldpath: /nodes/test/usr/lib:/nodes/test/usr/lib64\n  verbose: 1\n  maxage: 0\n  gpgcheck: false\n  inherit: true\n  unsafe: false\n  die-with-parent: Unset\n  new-session: Unset\n  share_all: Unset\n  share_user: Unset\n  share_cgroup: Unset\n  share_net: Unset\n  share_pid: Unset\n  share_ipc: Unset" |
| installApp | Install an app by app name in a node by redpath. | {"redPath": "/path/to/node", "appName":"name_app"} | redpak installApp {"redPath": "/var/redpesk/redpesk-core", "appName":"afb-binder"} | "response":"App afb-binder has been well installed on node /var/redpesk/redpesk-core" |
| removelApp | Remove an app by app name in a node by redpath. | {"redPath": "/path/to/node", "appName":"name_app"} | redpak removeApp {"redPath": "/var/redpesk/redpesk-core", "appName":"afb-binder"} | "response":"App afb-binder has been well removed on node /var/redpesk/redpesk-core" |
| updateApp | Update an app by app name in a node by redpath. | {"redPath": "/path/to/node", "appName":"name_app"} | redpak updateApp {"redPath": "/var/redpesk/redpesk-core", "appName":"afb-binder"} | "response":"App afb-binder has been well updated on node /var/redpesk/redpesk-core" |
