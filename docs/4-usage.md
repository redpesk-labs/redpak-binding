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
            "uid":"getroot",
            "info":"Get the redpath root for all Node",
            "verb":"getroot"
          },
          {
            "uid":"gettree",
            "info":"Get the tree of a node and this children according to the depth",
            "verb":"gettree"
          },
          {
            "uid":"getconfig",
            "info":"Get the config of a Node",
            "verb":"getconfig"
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
          },
          {
            "uid":"app/list",
            "info":"list all apps in a node by redpath",
            "verb":"app/list"
          }
        ]
      }
    ]
  }
}
```

## api's verb

### gettree

* **definition**
  Get the node tree with this children (-1 -> all tree)
* **arguments**
  ```json {"redpath": "/path/to/node", "depth": int_depth}```
  * `redpath`: can be optionnal and in that case return the tree from the root path
  * `depth` [optionnal]: depth of the search
* **exemple:**

  ```bash
  redpak gettree {"redpath":"/var/redpak/test", "depth": -1}
  ON-REPLY 9:redpak/gettree: OK
  {
    "jtype":"afb-reply",
    "request":{
      "status":"success",
      "code":0
    },
    "response":{
      "node":"test",
      "redpath":"/var/redpak/test",
      "children":[
        {
          "node":"test1",
          "redpath":"/var/redpak/test/test1",
          "children":[
            {
              "node":"test4",
              "redpath":"/var/redpak/test/test1/test4",
              "children":[
              ]
            },
            {
              "node":"test5",
              "redpath":"/var/redpak/test/test1/test5",
              "children":[
                {
                  "node":"test6",
                  "redpath":"/var/redpak/test/test1/test5/test6",
                  "children":[
                  ]
                },
                {
                  "node":"test7",
                  "redpath":"/var/redpak/test/test1/test5/test7",
                  "children":[
                    {
                      "node":"test8",
                      "redpath":"/var/redpak/test/test1/test5/test7/test8",
                      "children":[
                      ]
                    }
                  ]
                }
              ]
            },
            {
              "node":"test3",
              "redpath":"/var/redpak/test/test1/test3",
              "children":[
              ]
            }
          ]
        },
        {
          "node":"titi",
          "redpath":"/var/redpak/test/titi",
          "children":[
          ]
        },
        {
          "node":"test2",
          "redpath":"/var/redpak/test/test2",
          "children":[
          ]
        }
      ]
    }
  }
  ```

### getroot

* **definition**
  Get the redpath root for all Node.
* **arguments**
  none
* **exemple:**

  ```bash
  redpak getroot
  ON-REPLY 2:redpak/getroot: OK
  {
    "jtype":"afb-reply",
    "request":{
      "status":"success",
      "code":0
    },
    "response":"/var/redpak"
  }
  ```

### getconfig

* **definition**
  Get the config of a Node.
* **arguments**
  ```json {"redpath": "/var/redpak/test", "merged": 1, "expand": 1} ```
  * `merged` [optionnal]: 1 to have the ciinfg merged, 0 for the simple node config
  * `expand` [optionnal]: 1 to expand env variable on config, 0 to not exapnd
* **exemple:**

  ```bash
  redpak getconfig {"redpath":"/var/redpak/test", "merged": 1, "expand": 1}
  ON-REPLY 4:redpak/getconfig: OK
  {
    "jtype":"afb-reply",
    "request":{
      "status":"success",
      "code":0
    },
    "response":"headers:\n  alias: test\n  name: \n  info: Node created by devel(redpak-devel) the 21-Mar-2022 Mar:10 (UTC)\nexports:\n- mode: Restricted\n  mount: /lib64\n  path: /usr/lib64\n  info: /var/redpak/\n- mode: Restricted\n  mount: /lib\n  path: /usr/lib\n  info: /var/redpak/\n- mode: Restricted\n  mount: /bin\n  path: /usr/bin\n  info: /var/redpak/\n- mode: Restricted\n  mount: /etc/resolv.conf\n  path: /etc/resolv.conf\n  info: /var/redpak/\n- mode: Symlink\n  mount: /home/test\n  path: /nodes/_private\n  info: /var/redpak/\n- mode: Anonymous\n  mount: /var\n  info: /var/redpak/\n- mode: Execfd\n  mount: /etc/passwd\n  path: getent passwd  65534\n  info: /var/redpak/\n- mode: Execfd\n  mount: /etc/group\n  path: ! \"getent group 1000\\n 65534\"\n  info: /var/redpak/\n- mode: Procfs\n  mount: /proc\n  info: /var/redpak/\n- mode: Devfs\n  mount: /dev\n  info: /var/redpak/\n- mode: Tmpfs\n  mount: /tmp\n  info: /var/redpak/\n- mode: Anonymous\n  mount: /run\n  info: /var/redpak/\n- mode: Symlink\n  mount: /usr/lib/rpm\n  path: /lib/rpm\n  info: /var/redpak/\n- mode: Restricted\n  mount: /usr/share\n  path: /usr/share\n  info: /var/redpak/\n- mode: Private\n  mount: /nodes/_private\n  path: /var/redpak/test/private\n  info: /var/redpak/test\n- mode: Private\n  mount: /var/lib/rpm\n  path: /var/redpak/test/var/lib/rpm\n  info: /var/redpak/test\n- mode: Restricted\n  mount: /nodes/test/usr\n  path: /var/redpak/test/usr\n  info: /var/redpak/test\n- mode: Public\n  mount: /nodes/test/var\n  path: /var/redpak/test/var\n  info: /var/redpak/test\nenviron:\n- mode: Default\n  key: PS1\n  value: Rednode(test)>\n  info: /var/redpak/\n- mode: Remove\n  key: SHELL_SESSION_ID\n  info: /var/redpak/\n- mode: Default\n  key: HOME\n  value: /home/test\n  info: /var/redpak/\n- mode: Default\n  key: XDG_RUNTIME_DIR\n  value: /run/user/\n  info: /var/redpak/test\n- mode: Static\n  key: AFB_LDPATHS\n  value: /nodes/agl-core/usr/lib64/afb\n  info: /var/redpak/test\nconfig:\n  cachedir: /var/redpak/var/cache/dnf\n  umask: 027\n  verbose: 1\n  maxage: 0\n  gpgcheck: false\n  inherit: false\n  unsafe: false\n  die-with-parent: Enabled\n  new-session: Unset\n  share_all: Disabled\n  share_user: Unset\n  share_cgroup: Unset\n  share_net: Unset\n  share_pid: Unset\n  share_ipc: Unset\n  hostname: test\n  chdir: /home/test"
  }
  ```

### node/create

* **definition**
  Create a rednode by its redpath. It will use lib redwrap to create a node and copy a repo file into it.
* **arguments**
  ```json {"redpath": "/path/to/node", "repopath": "/path/to/redpesk.repo"}```
* **exemple:**

  ```bash
  redpak node/create {"redpath":"/var/redpak/test/test1", "repopath":"/home/devel/tmp/redpesk-core_bf3c02c6.repo"}
  ON-REPLY 1:redpak/node/create: OK
  {
    "jtype":"afb-reply",
    "request":{
      "status":"success",
      "code":0
    },
    "response":"Rednode /var/redpak/test/test1 created with success"
  }
  ```

### node/delete

* **definition**
  Delete a rednode by its redpath.
* **arguments**
  ```json {"redpath": "/path/to/node"}```
* **exemple:**

  ```bash
  redpak node/delete {"redpath":"/var/redpak/test/helloworld"}
  ON-REPLY 2:redpak/node/delete: OK
  {
    "jtype":"afb-reply",
    "request":{
      "status":"success",
      "code":0
    },
    "response":"Rednode /var/redpak/test/helloworld has been deleted with success"
  }
  ```

### app/install
* **definition**
  Install an app by app name in a node by redpath.
* **arguments**
  ```json {“redpath”: “/path/to/node”, “appname”:”name_app”}```
* **exemple:**

  ```bash
  redpak app/install {"redpath":"/var/redpak/test/helloworld", "appname":"strace"}
  ON-REPLY 1:redpak/app/install: OK
  {
    "jtype":"afb-reply",
    "request":{
      "status":"success",
      "code":0
    },
    "response":"App strace has been well installed on node /var/redpak/test/helloworld"
  }
  ```

### app/update

* **definition**
  Update an app by app name in a node by redpath.
* **arguments**
  ```json {“redpath”: “/path/to/node”, “appname”:”name_app”}```
* **exemple:**

  ```bash
  ```

### app/remove

* **definition**
  Remove an app by app name in a node by redpath.
* **arguments**
  ```json {“redpath”: “/path/to/node”, “appname”:”name_app”}```
* **exemple:**

  ```bash
  redpak app/remove {"redpath":"/var/redpak/test/helloworld", "appname":"strace"}
  ON-REPLY 1:redpak/app/remove: OK
  {
    "jtype":"afb-reply",
    "request":{
      "status":"success",
      "code":0
    },
    "response":"App strace has been well removed on node /var/redpak/test/helloworld"
  }
  ```

### app/list

* **definition**
  List app installed in a node by redpath.
* **arguments**
  ```json {“redpath”: “/path/to/node”}```
* **exemple:**

  ```bash
  redpak app/list {"redpath":"/var/redpak/test/helloworld"}
  ON-REPLY 2:redpak/app/list: OK
  {
    "jtype":"afb-reply",
    "request":{
      "status":"success",
      "code":0
    },
    "response":"strace-5.1-1.el8.x86_64\n"
  }
  ```