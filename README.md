# redpak-binding

This service aims to manage Rednode on target.

![Architecture](./docs/images/radpak-binding-architecture.png)

## docs

* [architecture](./docs/1-architecture.md)
* [installation](./docs/2-installation.md)
* [configuration](./docs/3-configuration.md)
* [usage](./docs/4-usage.md)
* [ui](./docs/5-ui.md)
* [errata](./docs/6-errata.md)

## Valgrind

For developpers who will helping on this project, thank you very much ! But take care to use **Valgrind** before to push/request merge, your output will need to be:

```bash
==83485== 
==83485== HEAP SUMMARY:
==83485==     in use at exit: 100,693 bytes in 265 blocks
==83485==   total heap usage: 1,906 allocs, 1,641 frees, 261,826 bytes allocated
==83485== 
==83485== 0 bytes in 1 blocks are definitely lost in loss record 1 of 105
==83485==    at 0x4C360A5: malloc (vg_replace_malloc.c:380)
==83485==    by 0x50BAA96: afb_extend_get_options (in /usr/lib64/libafb.so.4.0)
==83485==    by 0x112164: afb_binder_opts_parse_final (in /usr/bin/afb-binder)
==83485==    by 0x10D8E5: main (in /usr/bin/afb-binder)
==83485== 
==83485== 0 bytes in 1 blocks are definitely lost in loss record 2 of 105
==83485==    at 0x4C360A5: malloc (vg_replace_malloc.c:380)
==83485==    by 0x50BAAA4: afb_extend_get_options (in /usr/lib64/libafb.so.4.0)
==83485==    by 0x112164: afb_binder_opts_parse_final (in /usr/bin/afb-binder)
==83485==    by 0x10D8E5: main (in /usr/bin/afb-binder)
==83485== 
==83485== LEAK SUMMARY:
==83485==    definitely lost: 0 bytes in 2 blocks
==83485==    indirectly lost: 0 bytes in 0 blocks
==83485==      possibly lost: 0 bytes in 0 blocks
==83485==    still reachable: 100,673 bytes in 262 blocks
==83485==         suppressed: 20 bytes in 1 blocks
```

For each basics commmands.
