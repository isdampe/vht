#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <limits.h>
#include <unistd.h>
#include "vht.h"

static void usage()
{
	std::cout << "Usage: vht [www.hostname.site] [directory = $pwd]" <<
		std::endl;
	exit(1);
}

bool vht_replace(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);

    return vht_replace(str, from, to);
}

static void vht_process(const vht_s &session)
{
	std::string buffer = std::string(VHOST_TEMPLATE);
	vht_replace(buffer, "[hostname]", session.hostname);
	vht_replace(buffer, "[directory]", session.directory);

	std::string out_fp = std::string(APACHE_VHOST_DIR);
	out_fp += "/" + session.hostname + ".conf";

	//Write the virtual host file.
	std::cout << "Writing virtualhost configuration to " << out_fp << std::endl;
	std::ofstream vhfh(out_fp);
	vhfh << buffer;
	vhfh.close();

	//Does the entry exist in /etc/hosts?
	std::ifstream hfh(HOSTS_FP);
	std::string hosts((std::istreambuf_iterator<char>(hfh)), 
		std::istreambuf_iterator<char>());

	if ((int)hosts.find(session.hostname) > 0) {
		std::cout << "Hostname entry for " << session.hostname << " already" \
			" exists in " << HOSTS_FP << ". Skipping." << std::endl;
	} else {
		hosts += "\n127.0.0.1\t" + session.hostname;
		std::ofstream hfho(HOSTS_FP);
		hfho << hosts;
		hfho.close();

		std::cout << "Added entry for hostname " << session.hostname << " to " \
			HOSTS_FP << std::endl;
	}

	std::string apache_cmd = "a2ensite " + session.hostname + ".conf && " \
		"service apache2 reload";
	system(apache_cmd.c_str());
	std::cout << "Service apache2 reloaded." << std::endl << \
		"Browse to http://" << session.hostname << " in your browser." << std::endl;

}

int main(const int argc, char **argv)
{
    if (getuid()) {
		std::cout << "You must be root." << std::endl;
		return 1;
	}

	char directory[PATH_MAX];
	if (argc < 2)
		usage();

	vht_s session;
	session.hostname = std::string(argv[1]);

	if (argc < 3) {
		if (getcwd(directory, sizeof(directory)) == NULL) {
			std::cerr << "getcwd() error" << std::endl;
			return 1;
		}
	} else {
		session.directory = std::string(argv[argc -1]);
	}

	session.directory = std::string(directory);
	if (session.directory[session.directory.length() -1] != '/')
		session.directory += "/";

	vht_process(session);

	return 0;
}