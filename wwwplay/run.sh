#!/bin/bash

RUN chmod -R 777 /wwwplay/BGWDSP_DEV/
#chmod -R 777 /opt/ti/ccsv5/eclipse/
RUN chmod -R 777 /opt/ti/
# set eclipse configuration dir
# /opt/ti/ccsv5/eclipse/eclipse.ini
# -configuration
# /opt/ti/ccsv5/eclipse/configuration

httpd -DFOREGROUND
# debug for the case that httpd server starts failed 
tail -f /dev/null

