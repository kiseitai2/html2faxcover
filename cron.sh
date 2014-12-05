#!/bin/bash

usr=www-data
grp=www-data
h2fax=/var/www/avantfax/faxes/recvd/

echo $grp.$usr
chown -v -R $grp.$usr $h2fax
chmod -v -R a+rx $h2fax
