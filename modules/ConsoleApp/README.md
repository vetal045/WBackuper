# ConsoleApp

### About

Represents command driven console application to use backup functions.

### Commands

{Name} - Required variable

(ProfileName) - Can be ignored if default profile is set.

##### 1. q (or quit)

```
q
```

Exists from the program.

##### 2. def

```
def {ProfileName}
```

Sets the default profile name. So you don't have to enter profile name in other commands.

##### 3. def_rem

```
def_rem
```

Removes the default profile name.

##### 4. add

```
add (ProfileName)
```

Adds a new empty profile to program memory.

##### 5. view_all

```
view_all
```

Prints on the screen settings about all profiles.

##### 6. prof_compr

```
prof_compr (ProfileName) {Compression}
```

Compression must be in range:
  - -1 Default
  - 0 Disabled
  - 1-10 Compression level

Sets the compression level to backups.

##### 7. prof_chunk

```
prof_chunk (ProfileName) {Size}
```

Size is number > 0

Sets the size of one data chunk in MB.

##### 8. prof_res

```
prof_res (ProfileName) {Path}
```

Sets the path to result backup file.

##### 9. prof_file

```
prof_file (ProfileName) {Path}
```

Adds a file to backup list.
After this command the file haven't be moved.

##### 10. prof_dir

```
prof_dir (ProfileName) {Path}
```

Adds a directory to backup list (all directory's files and folders are added to  backup list).
After this command the directory haven't be moved. 

##### 11. save

```
save (ProfileName)
```

Saves the profile's settings to a file.


##### 12. load

```
load (ProfileName)
```

Loads the profile's settings from a file.

##### 13. backup

```
backup (ProfileName)
```

Backups files in the list.

##### 14. restore

```
restore (ProfileName)
```

Restores files in the list from the backup file.
