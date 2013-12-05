# [Colony Gateway (NPAPI Plugin)](http://getcolony.com)

NPAPI based plugin that provides a series of native features like:

* Printing using direct access to the printing infra-structure of the operative system
* Creation of native based GUI elements

The current implementation supports only x86/x64 architectures and Windows, Linux and Mac OS X.

## Building

In order to build both the CRX (Goolgle Chrome) and the XPI (Mozilla Firefox) files for the plugin
one should use the scripts created for such effect under the [scripts](scripts) directory. The
automated process assumes the Windows build process using the Visual Studio IDE.

### Windows

Use Visual Studio 2008 Express for compilation with no additional dependencies. The build process should
create the plugins files in the output.

### Mac OS X

Download the [Gecko SDK 1.9 32 bit](https://developer.mozilla.org/en-US/docs/Gecko_SDK) from the official 
website.

Use the `/Library/Internet Plugins/` directory to test the plugin while the proper extension package file
is not created.

### Linux

Use the `/` directory to test te `*.so` based plugin file for development purposes.

## Packaging

### Google Chrome

Create a package file that contains the references to the complete set of plugins files for the various
platforms as defined here:

```javascript
"plugins" : [
    { "path" : "plugin-windows.dll" },
    { "path" : "plugin-linux.so" },
    { "path" : "plugin-mac.plugin" }
]
```
