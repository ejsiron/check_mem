# check_mem

Local memory check module for Nagios-compatible monitoring applications on UNIX-compatible systems. Does not currently work with Windows systems.

## Description

Outputs current free memory in relation to total memory.  
Formatted for human-readability and performance data gathering.  
Optionally includes warning and critical state.

## Parameters

`-w, --warning=VALUE`: fixed number or percent representing low-point warning level

`-c, --critical=VALUE`: fixed number of percent representing low-point critical level

`-h, --help`: show this help; overrides all other settings

## Examples

These examples show possible ways to use check_mem.

### Simple usage

```bash
check_mem -w 20% -c 1048576
```

Outputs free memory, warning below 20% and critical below 1 megabyte. Either field accepts a percentage or a fixed number.

### Usage with prefixes

```bash
check_mem -w 20% -c 1MB
```

Same as previous. check_mem understands common prefixes from (k)ilo to (y)otta.

### Case-insensitive example

```bash
check_mem -w 20% -c 1mb
```

Same as previous; -w and -c are case-insensitive.

## Installation Instructions

check_mem ships as source code, so you will need to compile it. Any C++11-complaint compiler should work.

### Unpack the tarball

If you downloaded the tarball, begin by unpacking it:

```bash
tar xzf check_mem.tar.gz
```

### Compile and install check_mem on Nagios systems

These instructions assume that you just unpacked the tarball using the previous instructions. If you obtained the source another way, use your own method to switch to the location of those files instead of using the first `cd` command.

```bash
cd check_mem
make
sudo make Install
```

### Manual installation

If you do not use Nagios, then you can install the program yourself.

```bash
make
cp check_mem ${LOCATION_OF_YOUR_MONITORING_SYSTEM_CHECK_FILES}
```

### Automatically uninstall check_mem

The installer creates a copy of check_mem in Nagios' libexec folder (`/usr/local/nagios/libexec`). Use the following to remove it:

```bash
sudo make Uninstall
```

### Manually uninstall check_mem

If you'd like to remove check_mem manually, simply delete it from its installed location.

```bash
sudo rm -f /usr/local/nagios/libexec/check_mem
```

### Other Make Instructions

Delete compiled and linked files: `make clean`  
Create compressed tarball: `make tar`

## Current Release Tarball Download Location

https://github.com/ejsiron/check_mem/releases

## Sample Nagios Command

You can use the following, or something similar, in your Nagios configuration files to define the command that calls check_mem. If you're not certain where to put it, `commands.cfg` is a good place to start.

```text
# 'check_local_memory' command definition

define command{
command_name check_local_memory
command_line $USER1$/check_mem -w $ARG1$ -c $ARG2$
}
```

## Sample Nagios Service

You can use the following, or something similar, in your Nagios configuration files to define the service that invokes check_mem for the local system. You will most likely want to place this in `localhost.cfg`.

```text
# Define a service to check local memory usage
# "perf-service" in this sample is a service template that sets process_perf_data to 1
# if process_perf_data is not set, the performance data is ignored

define service{
    use                    perf-service,local-service
    host_name              localhost
    service_description    Memory Usage
    check_command          check_local_memory!15%!10%
}
```
