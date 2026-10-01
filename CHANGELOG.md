# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

* Loading of fonts on Windows for the printing of the documents, without installing them in the system - [#29](https://github.com/hivesolutions/colony-print/issues/29)

### Changed

*

### Fixed

*

## [1.7.0] - 2026-10-02

### Added

* Windows XP compatible wheels for x86 (Python 2.7 and 3.4) published to PyPI on every release

### Fixed

* Build of the Python module for Python 2.7 on Windows, Visual C++ 2008 has no `stdint.h`
* Windows printing tests hanging on 32 bit Python, consecutive jobs to the same output file opened the "Save Print Output As" dialog

## [1.6.0] - 2026-10-02

### Added

* Linux wheels for x64 and ARM64 (Python 3.9 to 3.14) published to PyPI on every release

## [1.5.0] - 2026-10-02

### Added

* macOS universal wheels for Intel and Apple Silicon (Python 3.9 to 3.14) published to PyPI on every release

## [1.4.0] - 2026-10-02

### Added

* Custom paper sizes accepted by each Linux printer, with their margins, in the listing of the printers - [#26](https://github.com/hivesolutions/colony-print/issues/26)
* Windows wheels for x64 (Python 3.6 to 3.14) and ARM64 (Python 3.11 to 3.14) published to PyPI on every release

## [1.3.0] - 2026-09-29

### Added

* Printing on a chosen Linux printer with the job title, paper size and scaling - [#24](https://github.com/hivesolutions/colony-print/issues/24)
* Print to file on Linux, used by the email mode - [#24](https://github.com/hivesolutions/colony-print/issues/24)
* Printable area of each printer in the device listing - [#24](https://github.com/hivesolutions/colony-print/issues/24)
* Review of every pull request by Claude

### Fixed

* Crash when printing on a Linux system without printers - [#24](https://github.com/hivesolutions/colony-print/issues/24)
* File descriptor leak on every Linux print job - [#24](https://github.com/hivesolutions/colony-print/issues/24)
* Failed prints reported as successful - [#24](https://github.com/hivesolutions/colony-print/issues/24)
* Memory leak when listing the printers
* Leak of graphics resources on every Windows print job
* Crash or memory corruption when printing empty or invalid data
* Crash when listing printers with accented names on Windows
* Windows printers with accented names not found when printing
* Crash when printing an invalid Binie document on Windows
* Windows jobs for unknown printers or failed by the spooler reported as printed
* Windows jobs for the default printer not printed
* Wrong default printer on Windows systems without one

## [1.2.10] - 2026-02-24

### Fixed

* Upload only sdist to PyPI to avoid rejected `linux_x86_64` wheel platform tag

## [1.2.9] - 2026-02-24

### Fixed

* pytest execution in deploy workflow for modern Python versions

## [1.2.8] - 2026-02-24

### Changed

* Skip `setup.py build` for modern Python versions in deploy workflow

## [1.2.7] - 2026-02-24

### Added

* Support for Python 3.13 and 3.14 in CI

### Changed

* Updated CI workflows with archived Debian repos fix for EOL distributions
* Migrated deploy workflow to use `python -m build` instead of `setup.py sdist`

### Fixed

* Fixed `mkstemp` implicit declaration error on GCC 14+ by defining `_GNU_SOURCE`
* Fixed `setuptools` compatibility for newer Python versions

## [1.2.6] - 2024-05-29

### Fixed

* Removed extra line with *.h inclusion

## [1.2.5] - 2024-05-29

### Fixed

* Final fix for missing headers using `MANIFEST.in`

## [1.2.4] - 2024-05-29

### Fixed

* New strategy for source (c) and header files (h) in `setup.py`

## [1.2.3] - 2024-05-29

### Fixed

* New fix for header info in package data

## [1.2.2] - 2024-05-29

### Fixed

* Issue with inclusion of header files in `setup.py`

## [1.2.1] - 2024-05-01

### Fixed

* Small fix with GitHub Actions

## [1.2.0] - 2024-05-01

### Added

* PyPi package support
* Support for detecting the default printer in win32 (`get_devices()`)
* Support for the `get_format()` Python extension method
