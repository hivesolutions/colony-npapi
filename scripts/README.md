## Example usage

There are only two parameters to the calling:

* `target_path` - The target directory that is created in case it does not exists already.
* `source_path` - The source path is base project relative path to the binaries

An example call to the creation of all the plugin package files would be:

* `make_all all_files bin/colony_npapi/i386/win32/Release`