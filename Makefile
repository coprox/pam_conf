all :
	gcc -c -fpic -W -Wall -Wextra -Werror pam_module.c -fno-stack-protector
	gcc -shared -o pam_module.so pam_module.o

clean :
	rm -f pam_module.o
	rm -f pam_module.so

install : all
	sudo MakefileScript/install.sh


uninstall : clean
	sudo MakefileScript/uninstall.sh


test :
	@echo "no unit test"
check :
	sudo MakefileScript/check.sh

re : uninstall install

.PHONY : clean uninstall all install check re test
