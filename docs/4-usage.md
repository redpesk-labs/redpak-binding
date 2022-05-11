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
            "uid":"node/createRpm",
            "info":"Create a rednode by installing an rpm",
            "verb":"node/createRpm"
          },
          {
            "uid":"node/deleteRpm",
            "info":"Delete rpm installed rednode by its package name",
            "verb":"node/deleteRpm"
          }
        ]
      },
      {
        "uid":"USER",
        "info":"USER related verbs",
        "verbs":[
          {
            "uid":"node/create",
            "info":"Create a rednode by its redpath",
            "verb":"node/create"
          },
          {
            "uid":"node/delete",
            "info":"Delete a rednode by its redpath",
            "verb":"node/delete"
          },
          {
            "uid":"app/install",
            "info":"Install an app by app name in a node by redpath",
            "verb":"app/install"
          },
          {
            "uid":"app/update",
            "info":"Update an app by app name in a node by redpath",
            "verb":"app/update"
          },
          {
            "uid":"app/remove",
            "info":"Remove an app by app name in a node by redpath",
            "verb":"app/remove"
          }
        ]
      }
    ]
  }
}

```

## api's verb

### getTree

* **definition**
  Get the node tree with this children (-1 -> all tree)
* **arguments**
  ```json {"redPath": "/path/to/node", "depth": int_depth}```
* **exemple:**

  ```bash
  redpak getTree {"redPath":"/var/redpesk/test", "depth": -1}
  ON-REPLY 9:redpak/getTree: OK
  {
    "jtype":"afb-reply",
    "request":{
      "status":"success",
      "code":0
    },
    "response":{
      "node":"test",
      "redpath":"/var/redpesk/test",
      "children":[
        {
          "node":"test1",
          "redpath":"/var/redpesk/test/test1",
          "children":[
            {
              "node":"test4",
              "redpath":"/var/redpesk/test/test1/test4",
              "children":[
              ]
            },
            {
              "node":"test5",
              "redpath":"/var/redpesk/test/test1/test5",
              "children":[
                {
                  "node":"test6",
                  "redpath":"/var/redpesk/test/test1/test5/test6",
                  "children":[
                  ]
                },
                {
                  "node":"test7",
                  "redpath":"/var/redpesk/test/test1/test5/test7",
                  "children":[
                    {
                      "node":"test8",
                      "redpath":"/var/redpesk/test/test1/test5/test7/test8",
                      "children":[
                      ]
                    }
                  ]
                }
              ]
            },
            {
              "node":"test3",
              "redpath":"/var/redpesk/test/test1/test3",
              "children":[
              ]
            }
          ]
        },
        {
          "node":"titi",
          "redpath":"/var/redpesk/test/titi",
          "children":[
          ]
        },
        {
          "node":"test2",
          "redpath":"/var/redpesk/test/test2",
          "children":[
          ]
        }
      ]
    }
  }
  ```

### getRoot

* **definition**
  Get the redpath root for all Node.
* **arguments**
  none
* **exemple:**

  ```bash
  redpak getRoot
  ON-REPLY 2:redpak/getRoot: OK
  {
    "jtype":"afb-reply",
    "request":{
      "status":"success",
      "code":0
    },
    "response":"/var/redpak"
  }
  ```

### getConfig

* **definition**
  Get the config of a Node.
* **arguments**
  ```json {"redPath": "/var/redpesk/test"} ```
* **exemple:**

  ```bash
  redpak getConfig {"redPath": "/var/redpesk/test"}
  ON-REPLY 1:redpak/getConfig: OK
  {
    "jtype":"afb-reply",
    "request":{
      "status":"success",
      "code":0
    },
    "response":"headers:\n  alias: test\n  name: \n  info: Node created by devel(redpak-devel) the 21-Mar-2022 Mar:10 (UTC)\nexports:\n- mode: Private\n  mount: /nodes/_private\n  path: $NODE_PATH/private\n- mode: Private\n  mount: /var/lib/rpm\n  path: $NODE_PATH/var/lib/rpm\n- mode: Restricted\n  mount: /nodes/test/usr\n  path: $NODE_PATH/usr\n- mode: Public\n  mount: /nodes/test/var\n  path: $NODE_PATH/var\nenviron:\n- mode: Default\n  key: XDG_RUNTIME_DIR\n  value: /run/user/$UID\n- mode: Static\n  key: AFB_LDPATHS\n  value: /nodes/agl-core/usr/lib64/afb\nconfig:\n  persistdir: /var/redpesk/test/var/lib/dnf\n  rpmdir: /var/redpesk/test/var/lib/rpm\n  path: /nodes/test/usr/bin\n  ldpath: /nodes/test/usr/lib:/nodes/test/usr/lib64\n  verbose: 1\n  maxage: 0\n  gpgcheck: false\n  inherit: true\n  unsafe: false\n  die-with-parent: Unset\n  new-session: Unset\n  share_all: Unset\n  share_user: Unset\n  share_cgroup: Unset\n  share_net: Unset\n  share_pid: Unset\n  share_ipc: Unset"
  }
  ```

### node/create

* **definition**
  Create a rednode by its redpath. It will use lib redwrap to create a node and copy a repo file into it.
* **arguments**
  ```json {"redPath": "/path/to/node", "repoPath": "/path/to/redpesk.repo"}```
* **exemple:**

  ```bash
  redpak node/create {"redPath":"/var/redpesk/test/test1", "repoPath":"/home/devel/tmp/redpesk-core_bf3c02c6.repo"}
  ON-REPLY 1:redpak/node/create: OK
  {
    "jtype":"afb-reply",
    "request":{
      "status":"success",
      "code":0
    },
    "response":"Rednode /var/redpesk/test/test1 created with success"
  }
  ```

### node/delete

* **definition**
  Delete a rednode by its redpath.
* **arguments**
  ```“/path/to/node”```
* **exemple:**

  ```bash
  ```

### app/install
* **definition**
  Install an app by app name in a node by redpath.
* **arguments**
  ```json {“redPath”: “/path/to/node”, “appName”:”name_app”}```
* **exemple:**

  ```bash
  ```

### app/update

* **definition**
  Update an app by app name in a node by redpath.
* **arguments**
  ```json {“redPath”: “/path/to/node”, “appName”:”name_app”}```
* **exemple:**

  ```bash
  ```

### app/remove

* **definition**
  Remove an app by app name in a node by redpath.
* **arguments**
  ```json {“redPath”: “/path/to/node”, “appName”:”name_app”}```
* **exemple:**

  ```bash
  ```