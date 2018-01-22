#pam_conf

Work in pam configuration (authentfication system for UNIX) and crypted/uncrypted partition

LINUX only

## Install

make install

## Uninstall

make uninstall




## Technical information :


## /*

## Add a new user {user}
``
sudo adduser {user}
``


## Download cryptsetup
``
sudo apt-get install cryptsetup
``

## Launch the script container.sh to create container

## Add a new cipher of {number} MiB directory for {user}
### Creation of a file that will be converted into cipher container
``
dd if=/dev/urandom bs=1M count={number} of=/home/{user}/{container}
``

## the password of the container is the name of the user

### Creation of a cipher container
``
sudo cryptsetup luksFormat /home/{user}/{container}
``

### Opening of the cipher container
``
sudo cryptsetup luksOpen /home/{user}/{container} {container}
``

### Format the container on Ext4
``
sudo mkfs.ext4 /dev/mapper/{container}
``

### Create the mount point
``
mkdir /home/{user}/{mount_point}
``

## Modify the /etc/fstab file
### Add this line to your /etc/fstab file :
``
/dev/mapper/{container}  /home/{user}/{mount_point} ext4  defaults,noauto 0 0
``

sudo chown -R {user}:{user} {mount_point} ; sudo chmod 700 -R {mount_point}
; sudo chown {user}:{user} {container} ; sudo chmod 600 {container}

### libpam
``
sudo apt-get install libpam0g-dev
``
## */


then you can test it ! :)