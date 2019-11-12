#include <stdio.h>
#include <libstring.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include "vht.sch"

static void process(const struct vht::s *session)
{
	autofree string::string buffer = string::create(VHOST_TEMPLATE);
	string::replace(&buffer, "[hostname]", session->hostname.bytes);
	string::replace(&buffer, "[directory]", session->directory.bytes);

	//Write the virtual host file.
	autofree string::string out_fp = string::create(APACHE_VHOST_DIR);
	string::append(&out_fp, "/");
	string::append(&out_fp, session->hostname.bytes);
	string::append(&out_fp, ".conf");

	std::printf("Writing virtualhost configuration to %s\n", out_fp.bytes);
	FILE *fh = std::fopen(out_fp.bytes, "w");
	if (fh == NULL)
		std::printf("Error: Could not open file %s for writing\n", out_fp.bytes);

	std::fputs(buffer.bytes, fh);
	std::fclose(fh);

	//Check for hosts entry.
	fh = std::fopen(HOSTS_FP, "r");
	if (fh != NULL) {
		autofree string::string hosts = string::create(fh);
		fclose(fh);
		if (string::find(&hosts, session->hostname.bytes) > -1) {
			std::printf("Hostname entry for %s already exists in %s. Skipping\n",
				session->hostname.bytes, HOSTS_FP);
		} else {
			string::append(&hosts, "\n127.0.0.1\t");
			string::append(&hosts, session->hostname.bytes);

			fh = fopen(HOSTS_FP, "w");
			if (fh != NULL) {
				fputs(hosts.bytes, fh);
				std::printf("Added entry for hostname %s to %s\n", session->hostname.bytes,
					HOSTS_FP);
			} else {
				std::printf("Error writing to hosts file %s. Skipping.\n", HOSTS_FP);
			}
			fclose(fh);
		}
	} else {
		std::printf("Error reading hosts file from %s. Skipping.\n", HOSTS_FP);
		fclose(fh);
	}

	autofree string::string apache_cmd = string::create("a2ensite ");
	string::append(&apache_cmd, session->hostname.bytes);
	string::append(&apache_cmd, ".conf && service apache2 reload");

	std::printf("%s\n", apache_cmd.bytes);

	system(apache_cmd.bytes);
	std::printf("Service apache2 reloaded.\n Browse to http://%s in your browser.\n",
		session->hostname.bytes);
}

static void usage()
{
	std::printf("Usage: vht [www.hostname.site] [directory = $pwd]\n");
	exit(1);
}

int main(const int argc, char **argv)
{
	if (getuid()) {
		std::printf("You must be root.\n");
		return 1;
	}

	char directory[PATH_MAX];

	if (argc < 2)
		usage();

	struct vht::s session;
	session.hostname = string::create(argv[1]);

	if (argc < 3) {
		if (getcwd(directory, sizeof(directory)) == NULL) {
			std::printf("getcwd() error\n");
			string::free(&session.hostname);
			return 1;
		}
	} else {
		session.directory = string::create(argv[argc -1]);
	}

	session.directory = string::create(directory);
	if (session.directory.bytes[std::strlen(session.directory.bytes) -1] != '/')
		string::append(&session.directory, "/");

	process(&session);

	string::free(&session.hostname);
	string::free(&session.directory);

	return 0;
}