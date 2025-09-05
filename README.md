# cdl86

```
  ________  __   ___  ____
 / ___/ _ \/ /  ( _ )/ __/
/ /__/ // / /__/ _  / _ \ 
\___/____/____/\___/\___/ 
```
                          
`cdl86` - `Compact Detours Library 86`

> **Demo Projects** <br>
> [`gsctool`](https://github.com/lunar-sh/gsctool)
> [`cs2rec`](https://github.com/FASTCUP/cs2rec)

# Abstract
`cdl86` is a tiny, single file, cross platform `detours` library written in `C` in less than `1K` lines.

[`https://journal.lunar.sh/2022/linux-detours.html`](https://journal.lunar.sh/2022/linux-detours.html)

It allows for the interception of `x86` and `x86_64` `C/C++` binary functions in memory following code injection.

Currently the following platforms are supported:
- `Linux`
- `MS Windows`

The library currently supports two types of function hooks:
* `JMP` patch - patches origin function with a `JMP` to detour.
* `INT3` patch - places software breakpoint (`SWBP`) at origin address.<br>
Handles `control flow` to `detour`.

> Note: This project makes use of an internal `x86` instruction length disassembly engine.

# API
```
struct cdl_jmp_patch cdl_jmp_attach(void **target, void *detour);
struct cdl_swbp_patch cdl_swbp_attach(void **target, void *detour);
void cdl_jmp_detach(struct cdl_jmp_patch *jmp_patch);
void cdl_swbp_detach(struct cdl_swbp_patch *swbp_patch);
void cdl_jmp_dbg(struct cdl_jmp_patch *jmp_patch);
void cdl_swbp_dbg(struct cdl_swbp_patch *swbp_patch);
```
The `API` is documented in more detail in the corresponding `header` and `source`
files.

# Example Usage

Assuming we have a function `add()` that is defined as follows:
```
int add(
    __in int x,
    __in int y
)
{
    printf("Inside original function\n");
    return x + y;
}
```

Assuming we want to hook this function with `cdl86` the first step
is to define a function pointer `typedef` and declare our `detour` function:

```
typedef int add_t(
    __in int x,
    __in int y
);
add_t* addo = NULL;
```
```
// define detour function
int add_detour(
    __in int x,
    __in int y
)
{
    printf("Inside detour function\n");
    return addo(5,5);
}

```
Then in our `main()` function we create a `cdl_jmp_patch` struct and assign
a value to `addo` (our function pointer to the original function):
```
struct cdl_jmp_patch jmp_patch = {};
addo = (add_t*)add;
```

Finally call `cdl_jmp_attach` as follows:
```
jmp_patch = cdl_jmp_attach((void**)&addo, add_detour);
```

The original function `add` has now been hooked!

To dump `debug` info from the `cdl_jmp_patch` struct use `cdl_jmp_dbg`.

# Info

The compiler used for this project is [`tcc`](https://github.com/lunar-sh/tcc).

Files:
<br/>
* `cdl.c` - `C` source file for `CDL`.
* `cdl.h` - `CDL` header file to include.

Folders:
* `/tests` - `CDL` test suite.

# Signature

```
+---------------------------------------+
|   .-.         .-.         .-.         |
|  /   \       /   \       /   \        |
| /     \     /     \     /     \     / |
|        \   /       \   /       \   /  |
|         "_"         "_"         "_"   |
|                                       |
|  _   _   _ _  _   _   ___   ___ _  _  |
| | | | | | | \| | /_\ | _ \ / __| || | |
| | |_| |_| | .` |/ _ \|   /_\__ \ __ | |
| |____\___/|_|\_/_/ \_\_|_(_)___/_||_| |
|                                       |
|                                       |
| Lunar RF Labs                         |
| https://lunar.sh                      |
|                                       |
| Research Laboratories                 |
| Copyright (C) 2022-2025               |
|                                       |
+---------------------------------------+
```

