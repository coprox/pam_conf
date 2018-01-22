#! /bin/bash

check=0
file=/lib/security/pam_module.so
auth=$(sudo sed -n '$p' /etc/pam.d/common-auth)
session=$(sudo sed -n '$p' /etc/pam.d/common-session)
account=$(sudo sed -n '$p' /etc/pam.d/common-account)
password=$(sudo sed -n '$p' /etc/pam.d/common-password)


if [ "$auth" != "auth  optional  pam_module.so" ]
then
    echo "missing configuration for auth"
    check=1
fi

if [ "$session" != "session optional  pam_module.so" ]
then
    echo "missing configuration for session"
    check=1
fi

if [ "$account" != "account optional  pam_module.so" ]
then
    echo "missing configuration for account"
    check=1
fi

if [ "$password" !=  "password  optional  pam_module.so" ]
then
    echo "missing configuration for password"
    check=1
fi


if [ ! -e "$file" ]; then
    echo "/lib/security/pam_module.so does not exist"
    check=1
fi

if [ $check = 0 ]
then
    echo "PAM module is well configured"
fi
