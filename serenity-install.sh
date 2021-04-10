#!/bin/bash

# This is just a temp installer script for testing
cp -rf /media/public/omnispeak.serenity/bin-serenity /media/public/serenity/Base/home/anon/
cd /media/public/serenity/Build/i686/ && ninja image
