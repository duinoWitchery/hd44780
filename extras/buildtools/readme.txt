This directory contains scripts to aid in the building of a library release.
Their main purpose is the update revision information in the files and to
automatically create a changelog.

The scripts are bash shell scripts.
If you are using a Wimpy/Crappy OS like Windows you are SOL. 

To create a new release:
- commit all file changes
- run mkrelease script (optionally pass in version string like 1.0.5 etc...)

Note: use "grip" tool to view README file as html

mkrelease can be run from the commandline or from a gui file manager

The mkrelease script will:
- check to see if all files have been commited
- prompt for a version string if not passed in
- massasge version string to make it SemVer compliant
- call mkreleasePrePatcher (if it exists)
	- this potentially does things that need to be done before the patcher runs
		currently this does nothing.
- call mkreleasePatcher:
	- patches all the necessary files to update version information.
	- calls mkChangeLog to insert an auto generated changelog from git log info into Readme.md
- call mkreleasePostPatcher (if it exists)
	- this potentially does things that need to be after the patcher runs
		currently this converts the patched README.md to html stored in the "docs" directory 

- optionally commit all modified files & tag repo with specified tag
