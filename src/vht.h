#ifndef VHT_H
#define VHT_H
#include <string>

#define HOSTS_FP "/etc/hosts"
#define APACHE_VHOST_DIR "/etc/apache2/sites-available"
#define VHOST_TEMPLATE "<VirtualHost *:80>\n\n" \
	"\tServerName [hostname]\n" \
	"\tDocumentRoot [directory]\n\n" \
	"\t<Directory [directory]>\n" \
		"\t\tOptions Indexes FollowSymLinks\n" \
		"\t\tAllowOverride All\n" \
		"\t\tRequire all granted\n" \
		"\t\tAllow from all\n" \
	"\t</Directory>\n\n" \
"</VirtualHost>"

struct vht_s {
	std::string hostname;
	std::string directory;
};

static void usage();
bool vht_replace(std::string& str, const std::string& from, const std::string& to);
static void vht_process(const vht_s &session);

#endif