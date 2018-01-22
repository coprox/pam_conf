#define PAM_SM_AUTH
#define	BUFSIZE		2048

#include <security/pam_modules.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>


PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  (void)flags;
  (void)argc;
  (void)argv;
  (void)pamh;
   return (PAM_SUCCESS);
}

// Establishes the account's usability and the user's accessibility to the system
PAM_EXTERN int  pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
   (void)pamh;
   (void)argc;
   (void)argv;
   (void)flags;
  return (PAM_SUCCESS);
}
// Erase the memory at the end of pam use
void  cleanup(pam_handle_t *pamh, void *data, int error_status)
{
  char *xx;

  if ((xx = data)) {
    while (*xx) {
      *xx++ = '\0';
    }
    free(data);
  }
  (void)pamh;
  (void)error_status;
}


// Call when a user authenticate
PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  char*pass;
  // Retrieve the password
  pam_get_item(pamh, PAM_AUTHTOK, (const void **)&pass);
  // Save the password in memory so that you can access it when opening a session
  pam_set_data(pamh, "pam_module_pass", strdup(pass), &cleanup);
  (void)flags;
  (void)argc;
  (void)argv;
  return (PAM_SUCCESS);
}

// Called whenever a session is opening
PAM_EXTERN int  pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  char  *user;
  char	*pass;
  char  cmd[BUFSIZE];
  char  container[BUFSIZE];
  char	create[BUFSIZE];
  int	retval;

  // We retrieve the password
  pam_get_data(pamh, "pam_module_pass", (const void **)&pass);
  // We retrieve the username
  retval = pam_get_user(pamh, (const char **)&user, "Username: ");
  if (retval != PAM_SUCCESS) {
    return (retval);
  }
  sprintf(container, "/home/%s/container_%s", user, user);
  // Check if the user is root and quit if true, because root doesnt have container
  if (!(strcmp(user, "root")))
      return (PAM_SUCCESS);
  // Check if container exist in the current user, if not the container is created
  else if (access(container, F_OK) == -1)
    {
      printf("creating container..\n");
      sprintf(create, "dd if=/dev/urandom bs=1M count=10 of=/home/%s/container_%s ;echo \"%s\n%s\" | sudo cryptsetup luksFormat /home/%s/container_%s ;echo \"%s\" | sudo cryptsetup luksOpen /home/%s/container_%s container_%s ; sudo mkfs.ext4 /dev/mapper/container_%s ; mkdir /home/%s/secure_data-rw ; sudo echo \"/dev/mapper/container_%s /home/%s/secure_data-rw ext4  defaults,noauto 0 0\" >> /etc/fstab ; sudo chown -R %s:%s /home/%s/secure_data-rw ; sudo chmod 700 -R /home/%s/secure_data-rw ; sudo chown %s:%s /home/%s/container_%s ; sudo chmod 600 /home/%s/container_%s", user, user, pass, pass, user, user, user, user, user, user, user, user, user, user, user, user, user, user, user, user, user, user, user, user);
      system(create);
    }
  // We open and mount the containers if we have the password and ask if we have not
  printf("Open container container_%s and mount /dev/mapper/container_%s\n", user, user);
  sprintf(cmd, "echo \"%s\" | sudo cryptsetup luksOpen /home/%s/container_%s container_%s && sudo mount /dev/mapper/container_%s /home/%s/secure_data-rw && sudo chown -R %s:%s /home/%s/secure_data-rw && sudo chmod 700 -R /home/%s/secure_data-rw ; sudo chown %s:%s /home/%s/container_%s && sudo chmod 600 /home/%s/container_%s", pass, user, user, user, user, user, user, user, user, user, user, user, user, user, user, user);
  system(cmd);
  (void)flags;
  (void)argc;
  (void)argv;
  return (PAM_SUCCESS);
}

// Called whenever the password is changed
PAM_EXTERN int  pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  char*user;
  char*pass;
  char *passwd;
  char cmd[BUFSIZE];
  int retval;

  // We get the userName
  if ((retval = pam_get_user(pamh, (const char **)&user, "Username: ")) != PAM_SUCCESS)
    return (retval);
  // We get the password
  if ((retval = pam_get_item(pamh, PAM_AUTHTOK, (const void **)&pass)) != PAM_SUCCESS)
    return (retval);
  if ((retval = pam_get_item(pamh, PAM_OLDAUTHTOK, (const void **)&passwd)) != PAM_SUCCESS)
    return (retval);
  if (pass)
    {
      sprintf(cmd, "echo \"%s\n%s\n%s\" | sudo cryptsetup luksAddKey /home/%s/container_%s",passwd, pass, pass, user, user);
      /* printf("oldpasswd est %s\n", passwd); */
      /* printf("user est %s\n", user); */
      /* printf("le pass est %s\n", pass); */
      system(cmd);
    }
  (void)flags;
  (void)argc;
  (void)argv;
  return (PAM_SUCCESS);
}

// Called whenever a session is closing
PAM_EXTERN int  pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
  char	*user;
  char	cmd[BUFSIZE];
  int	retval;

  // We retrieve the userName
  retval = pam_get_user(pamh, (const char **)&user, "Username: ");
  if (retval != PAM_SUCCESS) {
    return (retval);
  }
  if (!(strcmp(user, "root")))
    return (PAM_SUCCESS);
  // We unmount and close the containers
   printf("Close container container_%s and umount it\n", user);
   sprintf(cmd, "sudo umount /home/%s/secure_data-rw ; sudo cryptsetup luksClose container_%s", user, user);
   system(cmd);
  (void)flags;
  (void)argc;
  (void)argv;
  return (PAM_SUCCESS);
}
