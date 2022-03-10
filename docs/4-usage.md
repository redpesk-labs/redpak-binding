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

### api's verbs

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
      "version":"1.0",
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
          }
        ]
      }
    ]
  }
}
```
