# Linux Automaton

This project shows how multiple Linux processes can communicate using as study case a set of automatons. The starting point will be a YAML file with the description of the automaton. Each state of the automaton will be processed, and the connections between states will be pipelines.

## Requierements
The programm was tested in two distributions:

### fedora 16:

GCC >= 4.7.2.
YAML >=  0.2
Glib >= 2.0
make >= 3.82

### ubuntu 13.04:

GCC >= 4.7.3.
YAML >=  0.2
Glib >= 2.0
make >= 3.81


### Libraries:

- yaml.h
- stdio.h
- stdlib.h
- glib.h
- string.h
- signal.h
- unistd.h
- string.h
- pthread.h
- semaphore.h

## Install

Steps to install

1. Initially locate in the root folder 

2. Enter the command to compile
```
$ make
```
3. The executable will be located in the src folder, to execute the program enter
```
$ ./sisctrl <archivoYAML>
```
Where fileYAML is the configuration file with the automata, in the folder YAML files are located some examples. An example of execution is the following:
```
$ ./src/sisctrl src/archivosYaml/automatas.yaml 
```
or being inside the src folder
```
$ ./sisctrl archivosYaml/automatas.yaml 
```
4. To clean the executable folder and let the original state
```
$ make clean
```
## Example of automata in YAML format
```
- automata: One
  description: "First automata"
  alpha: [a,b,c]
  states: [A,B,C]
  start: A
  final: [C]
  delta:
   - node: A
     trans:
      - in: a
        next: B
      - in: b
        next: A
   - node: B
     trans:
      - in: a
        next: B
      - in: b
        next: A
      - in: c
        next: C
   - node: C
     trans:
```
## Then to evaluate a string:
select the automata
```
{ cmd: info, msg: "One" }
```
send the string:
```
{ cmd: send, msg: "bbbbbb" }
```


## Useful Sources

http://www.csc.villanova.edu/~mdamian/threads/posixsem.html
http://www.chuidiang.com/clinux/herramientas/makefile.php
http://rpmfind.net/linux/RPM/fedora/19/x86_64/y/yaml-cpp-devel-0.3.0-3.fc19.x86_64.html

YAML and its parser
http://wpsoftware.net/andrew/pages/libyaml.html

Compilation of glib
https://developer.gnome.org/glib/2.30/glib-compiling.html

Files management with stdio
http://icecube.wisc.edu/~dglo/c_class/stdio.html
http://www.cplusplus.com/reference/cstdio/fscanf/

Headers management
http://stackoverflow.com/questions/7109964/creating-your-own-header-file-in-c

gslists
https://developer.gnome.org/gtk-tutorial/2.22/x2055.html

pipes
http://systemadmin.es/2010/05/como-funcionan-las-llamadas-al-sistema-dup-open-close
http://www.tldp.org/LDP/lpg/node11.html

Proccess
http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/create.html

Malloc
http://chuwiki.chuidiang.org/index.php?title=Arrays_en_C

Proccess groups
http://support.sas.com/documentation/onlinedoc/sasc/doc/lr2/setpgid.htm

The page that showed me how I was creating zombies O_o
http://www.webmasterworld.com/forum40/1032.htm

Reading strings with unknow size
http://stackoverflow.com/questions/16870485/how-can-i-read-a-an-input-string-of-unknown-length

Threads
http://www.yolinux.com/TUTORIALS/LinuxTutorialPosixThreads.html
