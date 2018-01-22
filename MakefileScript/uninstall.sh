#! /bin/bash

file=/lib/security/pam_module.so
auth=$(sudo sed -n '$p' /etc/pam.d/common-auth)
session=$(sudo sed -n '$p' /etc/pam.d/common-session)
account=$(sudo sed -n '$p' /etc/pam.d/common-account)
password=$(sudo sed -n '$p' /etc/pam.d/common-password)
check=0

if [ ! -e "$file" ]; then
    check=0
else
    check=1
    sudo rm -rf /lib/security/pam_module.so
fi

if [ "$auth" = "auth  optional  pam_module.so" ]
then
   check=1
   sudo sed -i '$d' /etc/pam.d/common-auth
fi

if [ "$session" = "session optional  pam_module.so" ]
then
    check=1
    sudo sed -i '$d' /etc/pam.d/common-session
fi

if [ "$account" = "account optional  pam_module.so" ]
then
    check=1
   sudo sed -i '$d' /etc/pam.d/common-account
fi

if [ "$password" =  "password  optional  pam_module.so" ]
then
    check=1
   sudo sed -i '$d' /etc/pam.d/common-password
fi

if [ $check = 0 ]
then
    echo "PAM is already uninstalled"
else
    echo "PAM has been successfully uninstalled"
fi
