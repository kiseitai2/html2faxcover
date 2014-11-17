Introduction:
 This project came about when I tried setting up a fax server at my aunt's business. 
Unfortunately, AvantFax's faxcover.php was having trouble creating a database object 
and connecting to the database. AvantFax connects to the MySQL database in order to 
obtain the name and e-mail of the sending user and putting it in the coversheet 
automatically. I decided that I could have a better control over the problem if I 
created a C++ program that would connect to this database directly. I decided to call 
the program html2faxcover, because I want this program to be a replacement for AvantFax's 
or any other fax server system's html to coversheet conversion utility. Currently, 
this program is not a completely standalone utility. 

Requirements:
 This program requires that html2ps or a similar utility is available in your system.
 
Installation:
 To install this utility program, download the source code. Open up the Makefile and
review the first couple of lines. Make changes to these lines if you think they are 
appropriate. Open up settings and make sure its information is accurate. Save both 
files. If by any chance you changed the CONF_DIR variable in the Makefile, edit the 
first path you'll see in main() in main.cpp from /etc/... to /the/path/in/conf_dir.
Such a change is not recommendable, but it is not an issue if you set permissions
correctly.
 Once these files are configured appropriately, run sudo make install in a terminal.
When the process is done, you have to manually update HylaFax's configuration to 
let it know where to call html2faxcover when it needs to create a coversheet from
an html document. Do the same to AvantFax's /includes/config.php. You may want to 
add the -E # execution flag info to tell html2faxcover what execution mode to run.
Read below about the usage parameters. 
 Send a test fax using your html coversheet of choice and confirm that the system
works as intended.

Usage:
 This program accepts all of the options sent by AvantFax and used by HylaFax's 
faxcover (see http://linux.die.net/man/1/faxcover). However, since the program 
connects to your MySQL database of choice, I supplied a couple of extra command 
line options!
 -u Allows you to specify the username for the database.
 -U Allows you to specify the engine to be used. It will be removed in future releases
    unless I add support for other database types (sqlite, MariaDB, etc).
 -P Allows you to specify the password used to log in to the database.
 -d Allows you to specify the name of the database you wish to connect.
 -H Allows you to specify the hostname where the database is hosted (localhost, 
    google.com, mydomainname.pick).
 -E Allows you to specify the execution mode. Currently, the program has 4 modes
    (0, 1, 2, 3). They are STAND_ALONE, CALL_HYLAFAX_FAXCOVER, CALL_HYLAFAX_FAXCOVER_AS_IS, 
	and HASH_TEST. The last one is a simple test ran by developers to get a hexadecimal
	value from a hash function. This flag is only important if you are modifying 
	the code in cmd_processor.cpp. 0 to 2 specify the level of processing that 
	must be performed on the coversheet before handing it to HylaFax. 0 = do 
	everything. 1 = remove AvantFax's prefix on each field, convert the file to 
	a Post Script file and call HylaFax's faxcover utility with the other command
    line options. 2 = the file is a ps coversheet that only needs the AvantFax 
	prefixes removed and passed to HylaFax.
	
Status:
 The program is stable in the fax server. The current version is 1.0!
