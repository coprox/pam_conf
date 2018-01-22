#! /bin/bash


file=/lib/security/pam_module.so
session=$(sudo sed -n '$p' /etc/pam.d/common-session)
account=$(sudo sed -n '$p' /etc/pam.d/common-account)
password=$(sudo sed -n '$p' /etc/pam.d/common-password)
auth=$(sudo sed -n '$p' /etc/pam.d/common-auth)
check=0


if [ ! -e "$file" ]; then
    sudo cp pam_module.so /lib/security/pam_module.so
    check=1
else
    check=0
fi

if [ "$auth" !=  "auth  optional  pam_module.so" ]
then
    check=1
    sudo echo "auth  optional  pam_module.so" >> /etc/pam.d/common-auth
fi

if [ "$session" != "session optional  pam_module.so" ]
then
    check=1
   sudo echo "session optional  pam_module.so" >> /etc/pam.d/common-session
fi

if [ "$account" != "account optional  pam_module.so" ]
then
    check=1
    sudo echo "account optional  pam_module.so" >> /etc/pam.d/common-account
fi

if [ "$password" !=  "password  optional  pam_module.so" ]
then
    check=1
   sudo echo  "password  optional  pam_module.so" >> /etc/pam.d/common-password
fi

if [ $check = 0 ]
then
    echo "PAM is already configured"
else
    echo "PAM has been successfully configured"
fi
