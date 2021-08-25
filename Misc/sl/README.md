# sl
New and improved version of the cool sl project.  This project was not really done for fun, but rather as part of an interview process (still had fun coding it!).  The task was to make certain changes to the **sl** project created by **mtoyoda** in any language of my choosing, however I decided to stick with C and as a bonus, ported it to Rust as well.  It was my first project coding in Rust.   

Steps performed:

1. Cloned the original sl repo (assumes git is installed):  
	git clone https://github.com/mtoyoda/sl.git  
	cd sl  
	
2. Installed dependencies (RedHat, CentOS, etc):  
	dnf install ncurses-devel ncurses  

3. Built the project (Makefile comes with original project; assumes make is installed):  
	make  
   
4. Tested the project:  
	./sl  

5. Updated make file to output to bin directory.  

6. Built the project:  
	make  

7. Tested the project (assumes you are still in the repo root directory):  
	bin/sl  

8. Edited the header/source file as per instructions.  

9. Built the projcet:  
	make  

10. Tested the project (will pick an animation at random):  
	bin/sl  

11. Tested the project (choose the 2nd animation):  
	bin/sl -n 2  

12. Tested the project (choose to load ASCII art from file):  
	bin/sl -f cat.txt  

13. Tested the project (dynamically generate ASCII art; requires Internet connection):  
	bin/sl -t "Hello World!"  

14. Tested the project (should indicate an error and display usage info):  
	bin/sl -s foo  
	bin/sl -n 555  
	bin/sl -n  

15. Tested install of RPM package (RedHat, CentOS, etc):  
	rpm -i dist/sl-1.0-1.el8.x86_64.rpm  

16. Tested sl from any location:  
	cd /  
	sl  

17. Tested Rust wrapper for sl (assumes rustc/cargo is installed):  
	cd rust  
	cargo run -- -t "Hello Rustaceans!"  

NOTE: you may need to install 'openssl-dev' and/or 'libncurses5-dev libncursesw5-dev' if not already installed.  

