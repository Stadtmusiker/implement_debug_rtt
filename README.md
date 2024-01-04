# implement_debug_rtt
Implementation of debug interface using RTT

## Usage
This repository is intended to be used as a submodule or library
* `cd /[main-project-path]`
* `mkdir submodules`
* `cd submodules`
* `git submodule add <url>`

This repository its own nested submodules -- when adding this repo, initialize these submodules
* `cd /[main-project-path]/[submodules/]implement_debug_rtt`
* `git submodule init`
* `git submodule sync`
* `git submodule update`

Once established, only one header file is needed fir project use: the interface itself, `debug.h`.
