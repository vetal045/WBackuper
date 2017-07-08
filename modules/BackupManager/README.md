# BackupManager

### About

The library that contains backup functionality.

### Interface classes

##### BackupManager

Manages backup profiles and contains backup/restore logic.

##### BackupProfile

Stores backup settings and saves/restores them to a file.

##### ConfigManager

Saves/restores key-value pairs from a file to a std::map.

The file syntax should be like:

```
key1 value1
key2 value2
key3
```

