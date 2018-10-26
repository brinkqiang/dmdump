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
dmdump -n gameserver

```cpp

#include "dmdump.h"
#include "dmflags.h"

#include <iostream>

DEFINE_string(n, "zsh", "name");

// dmdump -n zsh
int main(int argc, char **argv) 
{   
    DMFLAGS_INIT(argc, argv);
    DMGenDumpFile(FLAGS_n);
    std::cout << "name = " << FLAGS_n << std::endl;
    return 0;   
}

```
## Contacts
[![Join the chat](https://badges.gitter.im/brinkqiang/dmdump/Lobby.svg)](https://gitter.im/brinkqiang/dmdump)

## Thanks
