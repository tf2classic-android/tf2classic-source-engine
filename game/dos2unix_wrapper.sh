#!/bin/bash

WHORE=*

find {client,shared,server}/$WHORE | xargs dos2unix
