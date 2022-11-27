# CoolWheels

## Git ettiquette

**DO NOT UPLOAD CODE THAT YOU HAVEN'T COMPILED**

Ensure all of your commits to the `master' branch at least (preferrably all branches) compile correctly before pushing.
This will prevent your work from stopping the work of everybody else.

### Merging branches
You can commit working branches if necessary.
I will not have the time to verify and merge pull requests, if you create branches and want your working code merged to master you are responsible for doing it.

Please be sure to fetch from master, merge, AND verify you merged correctly before pushing a branch merge to master.
Take care that your merge doesn't destroy working code.  As with pull requests, I won't have time to fix merges that stomp on other code -- anyone can fix a push that goes wrong, it's easy to get in over your head when a merge is bad (the easiest is to roll back master to the point before the merge, but you lose everything from there on, and you can orphan HEAD in the github repo; this is not particularly hard, but it can be hard to figure out if you've never done it).

If your code is not in the `master' branch it's not going to be code used on the actual bot.

## Git assistance
Most things you'll need to know can be found on google; it's not an easy tool to learn if you don't already have a workflow with it though.  If you're having trouble finding what you need ask on Discord and somebody will be able to help you.

## Cloning Instructions

1. Create a base folder or directory for the repo, ie. C:\base\
2. Clone the repository into this folder
	1. With a terminal (cmd or bash): cd base
	2. git clone https://github.com/DanielANewby/CoolWheels.git
	3. This should create the folder base\CoolWheels
3. In mbed studio, go to File->Open Workspace
4. Select your **base folder** as the workspace

You should now have a workspace that contains the CoolWheels folder and source code.

Build the project to make sure that it works

## Building instructions

1. Set the Active program to CoolWheels (this should be the default)
2. Set the Target to NUCLEO-F411RE
3. The project should build and create BUILD and mbed-os subfolders.

If you run into errors at this point ask for help on Discord