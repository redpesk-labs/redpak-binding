# Errata

Some errors are already knows

## Cannot execute successively two app/cmd

When a command from a verb's app finish it will be necessary to reload the binding. (**app/install**, **app/remove**, **app/update**).

> **Example**:
> app/install goes wrong because of a lake of node config, or the app already exists
>
> logs: 
> 
> ```bash
> Package "afb-binder-4.0.3-12.AGL.rpdarz.x86_64" is already installed.
> Nothing to do.
>  ```
>
> Installing another app will not succeed
> 
> logs:
> 
> ```bash
> DEBUG: received websocket request for redpak/app/install: {"redpath":"/var/redpesk/test/helloworld", "appname":"strace"}
> HOOK: [req-000002:redpak/app/install] BEGIN uid=99=99 gid=99 pid=0 label=NoLabel id=NoLabel
> No redpath set! Aborting...
> ```
>
> Synchro has been lost
