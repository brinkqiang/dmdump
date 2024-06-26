# dmdump

Copyright (c) 2013-2018 brinkqiang (brink.qiang@gmail.com)

[dmdump GitHub](https://github.com/brinkqiang/dmdump)

## Build status
| [Linux][lin-link] | [MacOSX][osx-link] | [Windows][win-link] |
| :---------------: | :----------------: | :-----------------: |
| ![lin-badge]      | ![osx-badge]       | ![win-badge]        |

[lin-badge]: https://travis-ci.org/brinkqiang/dmdump.svg?branch=master "Travis build status"
[lin-link]:  https://travis-ci.org/brinkqiang/dmdump "Travis build status"
[osx-badge]: https://travis-ci.org/brinkqiang/dmdump.svg?branch=master "Travis build status"
[osx-link]:  https://travis-ci.org/brinkqiang/dmdump "Travis build status"
[win-badge]: https://ci.appveyor.com/api/projects/status/github/brinkqiang/dmdump?branch=master&svg=true "AppVeyor build status"
[win-link]:  https://ci.appveyor.com/project/brinkqiang/dmdump "AppVeyor build status"

## Intro
dmdump -n=gameserver

windows命令行闪退进程处理
```
procdump -accepteula -64 -w -ma -h server.exe
```

linux 直接打印最小进程ID调用堆栈
```
sudo pstack $(pgrep -uroot -o server)
```
linux 直接打印最大进程ID调用堆栈
```
sudo pstack $(pgrep -uroot -n server)
```

```cpp

#include "dmdump.h"
#include "dmflags.h"

#include <iostream>
#include "dmstrtk.hpp"

DEFINE_string(n, "zsh", "name");
DEFINE_bool(isWin32, false, "WIN32 / WIN64");

// dmdump -n zsh
int main(int argc, char **argv) 
{   
    DMFLAGS_INIT(argc, argv);
    DMGenDumpFile(FLAGS_n, FLAGS_isWin32);
    return 0;
}

```
## Contacts

## Thanks
